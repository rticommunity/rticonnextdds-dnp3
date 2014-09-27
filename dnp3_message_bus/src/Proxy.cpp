/*******************************************************************************
 (c) 2005-2014 Copyright, Real-Time Innovations, Inc.  All rights reserved.
 RTI grants Licensee a license to use, modify, compile, and create derivative
 works of the Software.  Licensee has the right to distribute object form only
 for use with RTI products.  The Software is provided "as is", with no warranty
 of any type, including any warranty for fitness for any purpose. RTI is under
 no obligation to maintain or support the Software.  RTI shall not be liable for
 any incidental or consequential damages arising out of the use or inability to
 use the software.
 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <boost/ptr_container/detail/map_iterator.hpp>
#include "MessageBusImpl.h"

#include <DNP3/AsyncStackManager.h>

#include <APL/Log.h>
#include <APL/Loggable.h>
#include <APL/PhysLayerSettings.h>
#include <APL/SerialTypes.h>

namespace rti {
namespace dnp3 {

Proxy::Proxy(
		string& name, RTIDNP3_ProxyId_t id, RTIDNP3_ProxyConfig *config,
		apl::Logger *logger) :
		apl::Loggable(logger),
				name(name),
				id(id),
				config(*config),
				stackManager(NULL),
				connections(NULL),
				connectionsData(NULL),
				logLevel(apl::LEV_DEBUG) {

	this->config.isMaster = config->isMaster;
	this->config.localAddr = config->localAddr;
	this->config.logLevel = config->logLevel;
	this->config.physicalConnectionsNum = config->physicalConnectionsNum;

	this->config.physicalConnections =
			(RTIDNP3_PhysicalConnectionConfig**) calloc(
					this->config.physicalConnectionsNum,
					sizeof(RTIDNP3_PhysicalConnectionConfig*));
	for (int i = 0; i < this->config.physicalConnectionsNum; i++) {
		RTIDNP3_PhysicalConnectionConfig * origConfig =
				config->physicalConnections[i];
		RTIDNP3_PhysicalConnectionConfig * physConfig =
				(RTIDNP3_PhysicalConnectionConfig*) malloc(
						sizeof(RTIDNP3_PhysicalConnectionConfig));
		physConfig->name = strdup(origConfig->name);
		physConfig->logLevel = origConfig->logLevel;
		physConfig->physTimeout = origConfig->physTimeout;
		physConfig->type = origConfig->type;

		switch (physConfig->type) {
		case RTIDNP3_PHYSICAL_CONNECTION_SERIAL: {
			RTIDNP3_SerialConfig *serialConfig = (RTIDNP3_SerialConfig*) malloc(
					sizeof(RTIDNP3_SerialConfig));
			RTIDNP3_SerialConfig *origSerialConfig =
					(RTIDNP3_SerialConfig*) origConfig->config;

			serialConfig->baud = origSerialConfig->baud;
			serialConfig->dataBits = origSerialConfig->dataBits;
			serialConfig->stopBits = origSerialConfig->stopBits;
			serialConfig->device = strdup(origSerialConfig->device);
			serialConfig->flowType = origSerialConfig->flowType;
			serialConfig->parity = origSerialConfig->parity;

			physConfig->config = (void*) serialConfig;

			break;
		}
		case RTIDNP3_PHYSICAL_CONNECTION_TCP_CLIENT: {
			RTIDNP3_TCPClientConfig *clientConfig =
					(RTIDNP3_TCPClientConfig*) malloc(
							sizeof(RTIDNP3_TCPClientConfig));
			RTIDNP3_TCPClientConfig *origClientConfig =
					(RTIDNP3_TCPClientConfig*) origConfig->config;

			clientConfig->serverAddr = strdup(origClientConfig->serverAddr);
			clientConfig->serverPort = origClientConfig->serverPort;

			physConfig->config = (void*) clientConfig;

			break;
		}
		case RTIDNP3_PHYSICAL_CONNECTION_TCP_SERVER: {
			RTIDNP3_TCPServerConfig *serverConfig =
					(RTIDNP3_TCPServerConfig*) malloc(
							sizeof(RTIDNP3_TCPServerConfig));
			RTIDNP3_TCPServerConfig *origServerConfig =
					(RTIDNP3_TCPServerConfig*) origConfig->config;

			serverConfig->endpointAddr = strdup(origServerConfig->endpointAddr);
			serverConfig->endpointPort = origServerConfig->endpointPort;

			physConfig->config = (void*) serverConfig;

			break;
		}
		default: {
			LOG_BLOCK(apl::LEV_ERROR,
					"Unknown type of physical connection: "<< physConfig->type);
			break;
		}
		}

		this->config.physicalConnections[i] = physConfig;

	}

	RTIDNP3_ARRAY_ALLOCATE(
			this->connections,
			this->connectionsData,
			Connection*,
			RTIDNP3_ARRAY_DEFAULT_SIZE,
			RTIDNP3_ARRAY_DEFAULT_MAX,
			RTIDNP3_ARRAY_DEFAULT_INCR);

	RTIDNP3_LOG_LEV_CONVERT_TO_DNP3(this->config.logLevel, this->logLevel);

	this->CreateStackManager();
	this->CreatePhysicalConnections();

	const char* type = (this->config.isMaster)?"master":"slave";
}

Proxy::~Proxy() {

	RTIDNP3_ARRAY_FREE_DEEP(this->connections, this->connectionsData, {
			this->DeleteConnection((RTIDNP3_ConnectionId_t)_el_i);
			});

	this->DeletePhysicalConnections();
	this->DeleteStackManager();

	for (int i = 0; i < this->config.physicalConnectionsNum; i++) {
		RTIDNP3_PhysicalConnectionConfig * config =
				this->config.physicalConnections[i];
//		free(config->name);

		switch (config->type) {
		case RTIDNP3_PHYSICAL_CONNECTION_SERIAL: {
			RTIDNP3_SerialConfig *serialConfig =
					(RTIDNP3_SerialConfig*) config->config;
//			free(serialConfig->device);
			break;
		}
		case RTIDNP3_PHYSICAL_CONNECTION_TCP_CLIENT: {
			RTIDNP3_TCPClientConfig *clientConfig =
					(RTIDNP3_TCPClientConfig*) config->config;

//			free(clientConfig->serverAddr);
			break;
		}
		case RTIDNP3_PHYSICAL_CONNECTION_TCP_SERVER: {
			RTIDNP3_TCPServerConfig *serverConfig =
					(RTIDNP3_TCPServerConfig*) config->config;

//			free(serverConfig->endpointAddr);
			break;
		}
		}

		free(config->config);
		free(config);

	}

	free(this->config.physicalConnections);

}

bool Proxy::CreateStackManager() {

	if (this->stackManager == NULL) {
		apl::Logger* logger = this->mpLogger->GetSubLogger("DNP3");

		this->stackManager = new apl::dnp::AsyncStackManager(
				logger);

		LOG_BLOCK(apl::LEV_INFO, "DNP3-AsyncStackManager created.");

		return true;
	} else {

		LOG_BLOCK(apl::LEV_ERROR, "DNP3-AsyncStackManager already created.");

		return false;
	}
}

bool Proxy::DeleteStackManager() {
	if (this->stackManager != NULL) {
		delete this->stackManager;
		this->stackManager = NULL;

		LOG_BLOCK(apl::LEV_INFO, "DNP3-AsyncStackManager deleted.");
	}

	return true;
}

bool Proxy::CreatePhysicalConnections() {

	LOG_BLOCK(apl::LEV_INFO, "Creating physical ports.");

	RTIDNP3_PhysicalConnectionConfig* connConfig = NULL;
	int i = 0;
	for (i = 0; i < this->config.physicalConnectionsNum; i++) {
		connConfig = this->config.physicalConnections[i];

		apl::PhysLayerSettings physLayerSettings;
		RTIDNP3_LOG_LEV_CONVERT_TO_DNP3(connConfig->logLevel,
				physLayerSettings.LogLevel);
		physLayerSettings.RetryTimeout = connConfig->physTimeout;

		string connectionName(connConfig->name);

		switch (connConfig->type) {
		case RTIDNP3_PHYSICAL_CONNECTION_SERIAL: {
			RTIDNP3_SerialConfig* serialConfig =
					(RTIDNP3_SerialConfig*) connConfig->config;

			apl::SerialSettings serialSettings;

			RTIDNP3_SerialConfig_CONVERTO_TO_DNP3(serialConfig,
					&serialSettings);

			this->stackManager->AddSerial(
					connectionName,
					physLayerSettings,
					serialSettings);

			LOG_BLOCK(apl::LEV_INFO,
					"Created serial port: "<< connectionName);

			break;
		}
		case RTIDNP3_PHYSICAL_CONNECTION_TCP_CLIENT: {
			RTIDNP3_TCPClientConfig* clientConfig =
					(RTIDNP3_TCPClientConfig*) connConfig->config;

			string serverAddr(clientConfig->serverAddr);
			boost::uint16_t serverPort(clientConfig->serverPort);

			this->stackManager->AddTCPClient(
					connectionName,
					physLayerSettings,
					serverAddr,
					serverPort);

			LOG_BLOCK(apl::LEV_INFO,
					"Created TCP-client port: "
					<< connectionName <<
					" (server: "<< serverAddr << ":" << serverPort << ")");

			break;
		}
		case RTIDNP3_PHYSICAL_CONNECTION_TCP_SERVER: {
			RTIDNP3_TCPServerConfig* serverConfig =
					(RTIDNP3_TCPServerConfig*) connConfig->config;

			string endpointStr(serverConfig->endpointAddr);
			boost::uint16_t port(serverConfig->endpointPort);

			this->stackManager->AddTCPServer(
					connectionName,
					physLayerSettings,
					endpointStr,
					port);

			LOG_BLOCK(apl::LEV_INFO,
					"Created TCP-server port: "
					<< connectionName <<
					" (listening on: "<< endpointStr << ":" << port << ")");

			break;
		}
		default: {

			LOG_BLOCK(apl::LEV_ERROR,
					"Cannot create port of unknown type: "
					<< connConfig->type);

			break;
		}
		}
	}

	LOG_BLOCK(apl::LEV_INFO, "Physical ports created.");

	return true;
}

bool Proxy::DeletePhysicalConnections() {

	RTIDNP3_PhysicalConnectionConfig* connConfig = NULL;
	int i = 0;
	LOG_BLOCK(apl::LEV_INFO, "Deleting physical ports...");

	for (i = 0; i < this->config.physicalConnectionsNum; i++) {
		connConfig = this->config.physicalConnections[i];
		string portName(connConfig->name);
		this->stackManager->RemovePort(portName);

		LOG_BLOCK(apl::LEV_INFO, "Physical port deleted : "<< portName);

	}

	LOG_BLOCK(apl::LEV_INFO, "Physical ports deleted.");

	return true;
}

Connection* Proxy::NewConnection(
		RTIDNP3_ConnectionId_t id,
		RTIDNP3_ConnectionConfig *config) {

	Connection *connection = NULL;
	int added = 0;

	string connectionName;
	ostringstream connectionNameStr;

	connectionNameStr << "Connection-" << id;

	connectionName = connectionNameStr.str();

	apl::Logger* logger = this->mpLogger->GetSubLogger(connectionName,
			this->logLevel);
	logger->SetVarName(connectionName);

	connection = new Connection(connectionName, id, config, this, logger);

	RTIDNP3_ARRAY_ADD_ELEMENT_EXPAND(this->connections, this->connectionsData,
			connection, added);

	if (!added) {
		LOG_BLOCK(apl::LEV_ERROR,
				"Cannot create new connection: " << connectionName);

		delete connection;
		return NULL;
	} else {

		LOG_BLOCK(apl::LEV_INFO, "New connection created: " << connectionName);

		return connection;
	}

}

bool Proxy::DeleteConnection(
		RTIDNP3_ConnectionId_t id) {

	Connection *connection = NULL;
	int connectionIndex = -1;
	int removed = 0;

	RTIDNP3_ARRAY_ITERATE(this->connections, this->connectionsData, {
			connection = (Connection*) _el;
			if (connection->GetId() == id) {
			connectionIndex = _el_i;
			break;
			}
			});

	if (connectionIndex > -1) {
		RTIDNP3_ARRAY_REMOVE_ELEMENT_BY_INDEX(this->connections,
				this->connectionsData,
				connectionIndex, removed);
		if (removed) {
			delete connection;

			LOG_BLOCK(apl::LEV_INFO, "Connection deleted : "<< id);

			return true;
		}
	}

	LOG_BLOCK(apl::LEV_ERROR, "Cannot delete connection : "<< id);

	return false;
}

Connection** Proxy::GetConnections(
		int *size_out) {

	if (size_out != NULL) {
		*size_out = this->connectionsData->currentSize;
	}

	return this->connections;
}

bool Proxy::operator==(
		Proxy& other) {

	return this->GetName() == other.GetName();
}

bool Proxy::operator<(
		Proxy& other) {

	return this->GetName() < other.GetName();
}

}
}
