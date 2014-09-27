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

#include <stdio.h>
#include <stdlib.h>
#include "lcfg_static.h"
#include "MessageBus.h"
#include "MessageBusImpl.h"

extern "C" {
using namespace rti::dnp3;

RTIDNP3_MessageBus* RTIDNP3_MessageBus_new() {

	MessageBus *bus = new MessageBus();
	return (RTIDNP3_MessageBus*) bus;

}

void RTIDNP3_MessageBus_delete(
		RTIDNP3_MessageBus* bus) {

	MessageBus *cppBus = (MessageBus*) bus;
	delete cppBus;
}

RTIDNP3_ProxyId_t RTIDNP3_MessageBus_createProxy(
		RTIDNP3_MessageBus *bus,
		const char* name,
		RTIDNP3_ProxyConfig *config
		) {

	MessageBus *cppBus = (MessageBus*) bus;
	string proxyName(name);
	return cppBus->NewProxy(proxyName, config);

}

RTIDNP3_ReturnCode_t RTIDNP3_MessageBus_deleteProxy(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_ProxyId_t id
		) {

	MessageBus *cppBus = (MessageBus*) bus;
	bool deleted = cppBus->DeleteProxy(id);
	if (deleted) {
		return RTIDNP3_RETCODE_OK;
	} else {
		return RTIDNP3_RETCODE_ERROR;
	}
}

RTIDNP3_ProxyId_t RTIDNP3_MessageBus_getProxyId(
		RTIDNP3_MessageBus *bus,
		const char* name
		) {

	MessageBus *cppBus = (MessageBus*) bus;
	string proxyName(name);

	int size = 0;
	Proxy** proxies = cppBus->GetProxies(&size);
	for (int i = 0; i < size; i++) {
		Proxy *p = proxies[i];
		if (proxyName == p->GetName()) {
			return p->GetId();
		}
	}

	return RTIDNP3_PROXY_ID_NONE;
}

RTIDNP3_ConnectionId_t RTIDNP3_MessageBus_createConnection(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_ProxyId_t proxy,
		RTIDNP3_ConnectionConfig *config
		) {

	MessageBus *cppBus = (MessageBus*) bus;
	return cppBus->NewConnection(proxy, config);
}

RTIDNP3_ReturnCode_t RTIDNP3_MessageBus_closeConnection(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_ConnectionId_t connection
		) {

	MessageBus *cppBus = (MessageBus*) bus;
	bool deleted = cppBus->DeleteConnection(connection);
	if (deleted) {
		return RTIDNP3_RETCODE_OK;
	} else {
		return RTIDNP3_RETCODE_ERROR;
	}
}

RTIDNP3_ConnectionConfig* RTIDNP3_MessageBus_getConnectionConfiguration(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_ConnectionId_t connection
		) {

	MessageBus *cppBus = (MessageBus*) bus;
	int size = 0;
	Connection** conns = cppBus->GetConnections(&size);
	for (int i = 0; i < size; i++) {
		Connection* conn = conns[i];
		if (conn->GetId() == connection) {
			return conn->GetConfig();
		}
	}

	return NULL;
}

RTIDNP3_ConnectionId_t* RTIDNP3_MessageBus_getProxyConnections(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_ProxyId_t proxy,
		int *size_out) {

	printf(
			"Function RTIDNP3_MessageBus_getProxyConnections() NOT IMPLEMENTED YET\n");

	return NULL;

}

RTIDNP3_ProxyId_t RTIDNP3_MessageBus_getAssociatedProxy(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_ConnectionId_t connection) {

	MessageBus *cppBus = (MessageBus*) bus;
	int size = 0;
	Connection** conns = cppBus->GetConnections(&size);
	for (int i = 0; i < size; i++) {
		Connection* conn = conns[i];
		if (conn->GetId() == connection) {
			return conn->GetProxy()->GetId();
		}
	}

	return RTIDNP3_PROXY_ID_NONE;

}

RTIDNP3_ReturnCode_t RTIDNP3_MessageBus_addMessageBusListener(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_MessageBusListener *listener,
		void *listener_data
		) {

	MessageBus *cppBus = (MessageBus*) bus;
	bool added = cppBus->AddMessageBusListener(listener);
	if (added) {
		return RTIDNP3_RETCODE_OK;
	} else {
		return RTIDNP3_RETCODE_ERROR;
	}
}

RTIDNP3_ReturnCode_t RTIDNP3_MessageBus_removeMessageBusListener(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_MessageBusListener *listener
		) {

	MessageBus *cppBus = (MessageBus*) bus;
	bool removed = cppBus->RemoveMessageBusListener(listener);
	if (removed) {
		return RTIDNP3_RETCODE_OK;
	} else {
		return RTIDNP3_RETCODE_ERROR;
	}
}

RTIDNP3_ReturnCode_t RTIDNP3_MessageBus_send(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_APDU *apdu
		) {

	MessageBus *cppBus = (MessageBus*) bus;
	return cppBus->Send(apdu);
}
#ifndef MIN
#define MIN(a,b) ((a) < (b) ? a : b)
#endif

RTIDNP3_PhysicalConnectionType_t RTIDNP3_PhysicalConnectionType_fromString(
		const char* str) {

	size_t len = 0;

	if (str == NULL) {
		return RTIDNP3_PHYSICAL_CONNECTION_NONE;
	}

	len = strlen(str);

	if (strncasecmp(str, "serial", MIN(len,strlen("serial"))) == 0) {
		return RTIDNP3_PHYSICAL_CONNECTION_SERIAL;
	}

	if (strncasecmp(str, "server", MIN(len,strlen("server"))) == 0) {
		return RTIDNP3_PHYSICAL_CONNECTION_TCP_SERVER;
	}

	if (strncasecmp(str, "client", MIN(len,strlen("client"))) == 0) {
		return RTIDNP3_PHYSICAL_CONNECTION_TCP_CLIENT;
	}

	return RTIDNP3_PHYSICAL_CONNECTION_NONE;

}

RTIDNP3_FlowType_t RTIDNP3_FlowType_fromString(
		const char* str) {
	size_t len = 0;

	if (str == NULL) {
		return RTIDNP3_SERIAL_FLOW_NONE;
	}

	len = strlen(str);

	if (strncasecmp(str, "none", MIN(len,strlen("none"))) == 0) {
		return RTIDNP3_SERIAL_FLOW_NONE;
	}

	if (strncasecmp(str, "hardware", MIN(len,strlen("hardware"))) == 0) {
		return RTIDNP3_SERIAL_FLOW_HARDWARE;
	}

	if (strncasecmp(str, "xonxoff", MIN(len,strlen("xonxoff"))) == 0) {
		return RTIDNP3_SERIAL_FLOW_XONXOFF;
	}

	return RTIDNP3_SERIAL_FLOW_NONE;
}

RTIDNP3_ParityType_t RTIDNP3_ParityType_fromString(
		const char* str) {
	size_t len = 0;

	if (str == NULL) {
		return RTIDNP3_SERIAL_PARITY_NONE;
	}

	len = strlen(str);

	if (strncasecmp(str, "none", MIN(len,strlen("none"))) == 0) {
		return RTIDNP3_SERIAL_PARITY_NONE;
	}

	if (strncasecmp(str, "even", MIN(len,strlen("even"))) == 0) {
		return RTIDNP3_SERIAL_PARITY_EVEN;
	}

	if (strncasecmp(str, "odd", MIN(len,strlen("odd"))) == 0) {
		return RTIDNP3_SERIAL_PARITY_ODD;
	}

	return RTIDNP3_SERIAL_PARITY_NONE;
}

int RTIDNP3_ExtConfig_extractPhysConnConfig(
		const char *proxyName, int connNum,
		RTIDNP3_Map *configMap,
		RTIDNP3_PhysicalConnectionConfig **physConnConfigOut) {
	char keyBuff[1024];
	const char* propsPhysName = NULL;
	const char* propsPhysTimeout = NULL;
	const char* propsPhysType = NULL;
	RTIDNP3_PhysicalConnectionConfig* physConfig = NULL;

	bzero(keyBuff, 1024);

	sprintf(keyBuff, "%s-phys-port-%d", proxyName, connNum);
	propsPhysName = RTIDNP3_Map_get(configMap, keyBuff);

	if (propsPhysName == NULL) {
		return 0;
	}

	physConfig = (RTIDNP3_PhysicalConnectionConfig*) malloc(
			sizeof(RTIDNP3_PhysicalConnectionConfig));

	sprintf(keyBuff, "%s-%s", propsPhysName,
			RTIDNP3_EXTCONFIG_PROPS_PHYS_TIMEOUT);
	propsPhysTimeout = RTIDNP3_Map_get(configMap, keyBuff);

	sprintf(keyBuff, "%s-%s", propsPhysName,
			RTIDNP3_EXTCONFIG_PROPS_PHYS_TYPE);
	propsPhysType = RTIDNP3_Map_get(configMap, keyBuff);

	physConfig->name = propsPhysName;
	physConfig->physTimeout = strtol(propsPhysTimeout, NULL, 0);
	physConfig->type = RTIDNP3_PhysicalConnectionType_fromString(
			propsPhysType);

	switch (physConfig->type) {
	case RTIDNP3_PHYSICAL_CONNECTION_SERIAL: {
		const char* propsBaud = NULL;
		const char* propsDataBits = NULL;
		const char* propsDevice = NULL;
		const char* propsFlow = NULL;
		const char* propsParity = NULL;
		const char* propsStopBits = NULL;
		RTIDNP3_SerialConfig* serialConfig = (RTIDNP3_SerialConfig*) malloc(
				sizeof(RTIDNP3_SerialConfig));

		sprintf(keyBuff, "%s-%s", propsPhysName,
				RTIDNP3_EXTCONFIG_PROPS_PHYS_SERIAL_BAUD);
		propsBaud = RTIDNP3_Map_get(configMap, keyBuff);
		sprintf(keyBuff, "%s-%s", propsPhysName,
				RTIDNP3_EXTCONFIG_PROPS_PHYS_SERIAL_DATA_BITS);
		propsDataBits = RTIDNP3_Map_get(configMap, keyBuff);
		sprintf(keyBuff, "%s-%s", propsPhysName,
				RTIDNP3_EXTCONFIG_PROPS_PHYS_SERIAL_DEVICE);
		propsDevice = RTIDNP3_Map_get(configMap, keyBuff);
		sprintf(keyBuff, "%s-%s", propsPhysName,
				RTIDNP3_EXTCONFIG_PROPS_PHYS_SERIAL_FLOW);
		propsFlow = RTIDNP3_Map_get(configMap, keyBuff);
		sprintf(keyBuff, "%s-%s", propsPhysName,
				RTIDNP3_EXTCONFIG_PROPS_PHYS_SERIAL_PARITY);
		propsParity = RTIDNP3_Map_get(configMap, keyBuff);
		sprintf(keyBuff, "%s-%s", propsPhysName,
				RTIDNP3_EXTCONFIG_PROPS_PHYS_SERIAL_STOP_BITS);
		propsStopBits = RTIDNP3_Map_get(configMap, keyBuff);

		serialConfig->baud = strtol(propsBaud, NULL, 0);
		serialConfig->dataBits = strtol(propsDataBits, NULL, 0);
		serialConfig->device = propsDevice;
		serialConfig->flowType = RTIDNP3_FlowType_fromString(propsFlow);
		serialConfig->parity = RTIDNP3_ParityType_fromString(propsParity);
		serialConfig->stopBits = strtol(propsStopBits, NULL, 0);

		physConfig->config = (void*) serialConfig;

		break;
	}
	case RTIDNP3_PHYSICAL_CONNECTION_TCP_CLIENT: {
		const char* propsServerAddr = NULL;
		const char* propsServerPort = NULL;
		RTIDNP3_TCPClientConfig* clientConfig =
				(RTIDNP3_TCPClientConfig*) malloc(
						sizeof(RTIDNP3_TCPClientConfig));

		sprintf(keyBuff, "%s-%s", propsPhysName,
				RTIDNP3_EXTCONFIG_PROPS_PHYS_TCP_ADDRESS);
		propsServerAddr = RTIDNP3_Map_get(configMap, keyBuff);
		sprintf(keyBuff, "%s-%s", propsPhysName,
				RTIDNP3_EXTCONFIG_PROPS_PHYS_TCP_PORT);
		propsServerPort = RTIDNP3_Map_get(configMap, keyBuff);

		clientConfig->serverAddr = propsServerAddr;
		clientConfig->serverPort = strtol(propsServerPort, NULL, 0);

		physConfig->config = (void*) clientConfig;

		break;
	}
	case RTIDNP3_PHYSICAL_CONNECTION_TCP_SERVER: {
		const char* propsAddr = NULL;
		const char* propsPort = NULL;
		RTIDNP3_TCPServerConfig* serverConfig =
				(RTIDNP3_TCPServerConfig*) malloc(
						sizeof(RTIDNP3_TCPServerConfig));

		sprintf(keyBuff, "%s-%s", propsPhysName,
				RTIDNP3_EXTCONFIG_PROPS_PHYS_TCP_ADDRESS);
		propsAddr = RTIDNP3_Map_get(configMap, keyBuff);
		sprintf(keyBuff, "%s-%s", propsPhysName,
				RTIDNP3_EXTCONFIG_PROPS_PHYS_TCP_PORT);
		propsPort = RTIDNP3_Map_get(configMap, keyBuff);

		serverConfig->endpointAddr = propsAddr;
		serverConfig->endpointPort = strtol(propsPort, NULL, 0);

		physConfig->config = (void*) serverConfig;

		break;
	}
	default: {
		break;
	}
	}

	*physConnConfigOut = physConfig;

	return 1;
}

void RTIDNP3_ExtConfig_disposePhysConnConfig(
		RTIDNP3_PhysicalConnectionConfig *physConnConfig) {

	free(physConnConfig->config);
	free(physConnConfig);

}

const char* RTIDNP3_ExtConfig_extractProxyConfig(
		int proxyNum,
		RTIDNP3_Map *configMap,
		RTIDNP3_ProxyConfig* proxyConfigOut) {
	const char *propsEntityName = NULL;
	const char* propsIsMaster = NULL;
	const char* propsLocalAddr = NULL;
	const char* propsLogLevel = NULL;
	int physConnNum = 0;
	RTIDNP3_ArrayData *physConnsData = NULL;
	RTIDNP3_PhysicalConnectionConfig **physConns = NULL;
	int i = 0;
	char keyBuff[1024];
	bzero(keyBuff, 1024);

	RTIDNP3_ARRAY_ALLOCATE(physConns, physConnsData,
			RTIDNP3_PhysicalConnectionConfig*, 10, 0, 10);

	sprintf(keyBuff, "proxy-%d", proxyNum);
	propsEntityName = RTIDNP3_Map_get(configMap, keyBuff);

	if (propsEntityName == NULL) {
		/* NO MORE PROXY CONFIGURATIONS */
		return NULL;
	}

	sprintf(keyBuff, "%s-%s", propsEntityName,
			RTIDNP3_EXTCONFIG_PROPS_IS_MASTER);
	propsIsMaster = RTIDNP3_Map_get(configMap, keyBuff);
	sprintf(keyBuff, "%s-%s", propsEntityName,
			RTIDNP3_EXTCONFIG_PROPS_ADDR);
	propsLocalAddr = RTIDNP3_Map_get(configMap, keyBuff);
	sprintf(keyBuff, "%s-%s", propsEntityName,
			RTIDNP3_EXTCONFIG_PROPS_LOG_LEVEL);
	propsLogLevel = RTIDNP3_Map_get(configMap, keyBuff);

	proxyConfigOut->isMaster = strtol(propsIsMaster, NULL, 0);
	proxyConfigOut->localAddr = strtol(propsLocalAddr, NULL, 0);
	proxyConfigOut->logLevel = RTIDNP3_LogLevel_fromString(propsLogLevel);

	RTIDNP3_PhysicalConnectionConfig* physConfig = NULL;

	i = 0;

	while (RTIDNP3_ExtConfig_extractPhysConnConfig(propsEntityName, i++,
			configMap, &physConfig)) {
		char added = 0;

		physConfig->logLevel = proxyConfigOut->logLevel;
		RTIDNP3_ARRAY_ADD_ELEMENT_EXPAND(physConns, physConnsData, physConfig,
				added);

		if (!added) {
			/* DO SOMETHING IF THIS HAPPENS... */
		}
	}

	proxyConfigOut->physicalConnectionsNum = physConnsData->currentSize;
	proxyConfigOut->physicalConnections = physConns;

	free(physConnsData);

	return propsEntityName;

}

void RTIDNP3_ExtConfig_disposeProxyConfig(
		RTIDNP3_ProxyConfig* proxyConfig) {
	int i = 0;

	for (i = 0; i < proxyConfig->physicalConnectionsNum; ++i) {
		RTIDNP3_ExtConfig_disposePhysConnConfig(
				proxyConfig->physicalConnections[i]);
		proxyConfig->physicalConnections[i] = NULL;
	}

	free(proxyConfig->physicalConnections);
}

int RTIDNP3_ExtConfig_extractConnectionConfig(
		const char *entityName, int connNum,
		RTIDNP3_Map *configMap,
		RTIDNP3_ConnectionConfig* connConfig) {

	char keyBuff[1024];
	const char* propsRemoteAddr = NULL;
	const char* propsAppTimeout = NULL;
	const char* propsLinkTimeout = NULL;
	const char* propsNumRetry = NULL;
	const char* propsPortName = NULL;
	const char* propsUseConfirms = NULL;
	const char* propsFragSize = NULL;
	int remoteAddr = 0;

	sprintf(keyBuff, "%s-connection-%d", entityName, connNum);
	propsRemoteAddr = RTIDNP3_Map_get(configMap, keyBuff);

	if (propsRemoteAddr != NULL) {

		remoteAddr = strtol(propsRemoteAddr, NULL, 0);

		sprintf(keyBuff, "%s-%d-%s", entityName, remoteAddr,
				RTIDNP3_EXTCONFIG_PROPS_CONN_TIMEOUT_APP);
		propsAppTimeout = RTIDNP3_Map_get(configMap, keyBuff);

		sprintf(keyBuff, "%s-%d-%s", entityName, remoteAddr,
				RTIDNP3_EXTCONFIG_PROPS_CONN_TIMEOUT_LINK);
		propsLinkTimeout = RTIDNP3_Map_get(configMap, keyBuff);

		sprintf(keyBuff, "%s-%d-%s", entityName, remoteAddr,
				RTIDNP3_EXTCONFIG_PROPS_CONN_FRAG_SIZE);
		propsFragSize = RTIDNP3_Map_get(configMap, keyBuff);

		sprintf(keyBuff, "%s-%d-%s", entityName, remoteAddr,
				RTIDNP3_EXTCONFIG_PROPS_CONN_RETRIES);
		propsNumRetry = RTIDNP3_Map_get(configMap, keyBuff);

		sprintf(keyBuff, "%s-%d-%s", entityName, remoteAddr,
				RTIDNP3_EXTCONFIG_PROPS_CONN_PHYS_PORT);
		propsPortName = RTIDNP3_Map_get(configMap, keyBuff);

		sprintf(keyBuff, "%s-%d-%s", entityName, remoteAddr,
				RTIDNP3_EXTCONFIG_PROPS_CONN_USE_CONFIRMS);
		propsUseConfirms = RTIDNP3_Map_get(configMap, keyBuff);

		connConfig->remoteAddr = remoteAddr;
		connConfig->appTimeout = strtol(propsAppTimeout, NULL, 0);
		connConfig->fragSize = strtol(propsFragSize, NULL, 0);
		connConfig->linkTimeout = strtol(propsLinkTimeout, NULL, 0);
		connConfig->numRetry = strtol(propsNumRetry, NULL, 0);
		connConfig->portName = propsPortName;
		connConfig->useConfirms = strtol(propsUseConfirms, NULL, 0);

		return 1;
	}

	return 0;
}

void RTIDNP3_ExtConfig_disposeConnectionConfig(
		RTIDNP3_ConnectionConfig* connConfig) {
	/* nothing to do for now, possibly we'll have to free strings */
}

enum lcfg_status RTIDNP3_ExtConfig_config_visitor(
		const char *key, void *data, size_t len, void *user_data) {

	RTIDNP3_Map *configMap = (RTIDNP3_Map*) user_data;

	RTIDNP3_Map_add(configMap, key, (char*) data);

	return lcfg_status_ok;
}

RTIDNP3_ReturnCode_t RTIDNP3_MessageBus_loadConfig(
		RTIDNP3_MessageBus *msgBus, const char *configFile,
		RTIDNP3_ProxyId_t **loadedProxies,
		int *loadedProxiesNum,
		RTIDNP3_ConnectionId_t **loadedConnections,
		int *loadedConnectionsNum) {

	RTIDNP3_Map *configMap = NULL;
	RTIDNP3_ProxyConfig proxyConfig;
	RTIDNP3_ConnectionConfig connectionConfig;
	int proxyNum = 0;
	int connNum = 0;
	RTIDNP3_ProxyId_t proxyId = RTIDNP3_PROXY_ID_NONE;
	RTIDNP3_ConnectionId_t connectionId = RTIDNP3_CONNECTION_ID_NONE;
	const char *proxyName = NULL;
	struct lcfg *c = NULL;
	RTIDNP3_ProxyId_t *lProxies = NULL;
	RTIDNP3_ConnectionId_t *lConnections = NULL;
	RTIDNP3_ArrayData *lProxiesData = NULL;
	RTIDNP3_ArrayData *lConnectionsData = NULL;
	int added = 0;

	printf(
			"[DNP3MessageBus]--[Loading proxies and connections from: %s]\n",
			configFile);

	c = lcfg_new(configFile);
	configMap = RTIDNP3_Map_new();

	RTIDNP3_ARRAY_ALLOCATE(lProxies, lProxiesData, RTIDNP3_ProxyId_t, 10, 0,
			10);
	RTIDNP3_ARRAY_ALLOCATE(lConnections, lConnectionsData,
			RTIDNP3_ConnectionId_t, 10, 0, 10);

	if (lcfg_parse(c) != lcfg_status_ok) {
		printf("[DNP3MessageBus]--[configuration parser error]-[%s]\n",
				lcfg_error_get(c));
		goto end;
	} else {
		lcfg_accept(c, RTIDNP3_ExtConfig_config_visitor, configMap);
	}

	lcfg_delete(c);

	while ((proxyName = RTIDNP3_ExtConfig_extractProxyConfig(proxyNum++,
			configMap,
			&proxyConfig)) != NULL) {

		if (RTIDNP3_MessageBus_getProxyId(msgBus,
				proxyName) == RTIDNP3_PROXY_ID_NONE) {

			proxyId = RTIDNP3_MessageBus_createProxy(msgBus, proxyName,
					&proxyConfig);

			if (proxyId == RTIDNP3_PROXY_ID_NONE) {
				printf("[DNP3MessageBus]--[Cannot create proxy]-[%s]\n", proxyName);
				continue;
			}

			RTIDNP3_ARRAY_ADD_ELEMENT_EXPAND(lProxies, lProxiesData, proxyId,
					added);

		} else {
			printf("[DNP3MessageBus]-[skipping creation of existing proxy]-[%s]\n",
					proxyName);
		}

		RTIDNP3_ExtConfig_disposeProxyConfig(&proxyConfig);

		connNum = 0;

		while (RTIDNP3_ExtConfig_extractConnectionConfig(proxyName, connNum++,
				configMap,
				&connectionConfig)) {
			int src = proxyConfig.localAddr;
			int dest = connectionConfig.remoteAddr;

			if (RTIDNP3_MessageBus_getConnectionId(msgBus,
					src, dest) != RTIDNP3_CONNECTION_ID_NONE) {

				printf(
						"[DNP3MessageBus]--[skipping creation of existing connection]--[%d,%d]\n",
						src, dest);

				RTIDNP3_ExtConfig_disposeConnectionConfig(&connectionConfig);
				continue;

			}

			connectionId = RTIDNP3_MessageBus_createConnection(msgBus, proxyId,
					&connectionConfig);

			RTIDNP3_ExtConfig_disposeConnectionConfig(&connectionConfig);

			if (connectionId == RTIDNP3_CONNECTION_ID_NONE) {
				printf(
						"[DNP3MessageBus]--[cannot create connection]--[%d,%d]\n",
						src, dest);
				continue;
			}

			RTIDNP3_ARRAY_ADD_ELEMENT_EXPAND(lConnections, lConnectionsData,
					connectionId,
					added);

		}

	}

	end:

	*loadedProxies = lProxies;
	*loadedProxiesNum = lProxiesData->currentSize;
	*loadedConnections = lConnections;
	*loadedConnectionsNum = lConnectionsData->currentSize;

	RTIDNP3_Map_delete(configMap);

	free(lProxiesData);
	free(lConnectionsData);

	return RTIDNP3_RETCODE_OK;
}

RTIDNP3_ConnectionId_t RTIDNP3_MessageBus_getConnectionId(
		RTIDNP3_MessageBus *bus, int src, int dest) {

	int i = 0;
	rti::dnp3::MessageBus *mb = (rti::dnp3::MessageBus *) bus;
	int j = 0;

	rti::dnp3::Connection** connections = mb->GetConnections(&i);

	for (i--; i >= 0; i--) {
		rti::dnp3::Connection *connection = connections[i];
		rti::dnp3::Proxy *proxy = connection->GetProxy();

		if (proxy->GetConfig()->localAddr == src
				&& connection->GetConfig()->remoteAddr == dest) {
			return connection->GetId();
		}
	}

	return RTIDNP3_CONNECTION_ID_NONE;
}

}
