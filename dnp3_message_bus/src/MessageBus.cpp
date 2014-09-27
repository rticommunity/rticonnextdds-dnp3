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

#include <APL/Log.h>
#include <APL/LogToStdio.h>

#include "MessageBus.h"
#include "MessageBusImpl.h"

namespace rti {
namespace dnp3 {

MessageBus::MessageBus() :
		listenerName(NULL), connections(NULL), connectionsData(NULL), listeners(
				NULL), listenersData(NULL), proxies(NULL), proxiesData(NULL), log(), logLevel(
				apl::LEV_DEBUG) {

	RTIDNP3_ARRAY_ALLOCATE( this->proxies, this->proxiesData, Proxy*,
			RTIDNP3_ARRAY_DEFAULT_SIZE, RTIDNP3_ARRAY_DEFAULT_MAX,
			RTIDNP3_ARRAY_DEFAULT_INCR);

	RTIDNP3_ARRAY_ALLOCATE( this->connections, this->connectionsData,
			Connection*, RTIDNP3_ARRAY_DEFAULT_SIZE, RTIDNP3_ARRAY_DEFAULT_MAX,
			RTIDNP3_ARRAY_DEFAULT_INCR);

	RTIDNP3_ARRAY_ALLOCATE( this->listeners, this->listenersData,
			RTIDNP3_MessageBusListener*, RTIDNP3_ARRAY_DEFAULT_SIZE,
			RTIDNP3_ARRAY_DEFAULT_MAX, RTIDNP3_ARRAY_DEFAULT_INCR);

	this->log.AddLogSubscriber(apl::LogToStdio::Inst());
	this->mpLogger = log.GetLogger(this->logLevel, "MsgBus");

	this->listenerName = new string("MessageBus");

}

MessageBus::~MessageBus() {

	RTIDNP3_ARRAY_FREE_DEEP(this->proxies, this->proxiesData,
			{ this->DeleteProxy((RTIDNP3_ProxyId_t)_el_i); });

	RTIDNP3_ARRAY_FREE(this->connections, this->connectionsData);

	RTIDNP3_ARRAY_FREE_DEEP(this->listeners, this->listenersData,
			{ this->RemoveMessageBusListener((RTIDNP3_MessageBusListener*)_el); });

	free(this->listenerName);

}

RTIDNP3_ProxyId_t MessageBus::NewProxy(string& name,
		RTIDNP3_ProxyConfig *config) {

	int added = 0;
	RTIDNP3_ProxyId_t id = this->proxiesData->currentSize;
	apl::FilterLevel loggerLevel;
	apl::Logger* logger;

	RTIDNP3_LOG_LEV_CONVERT_TO_DNP3(config->logLevel, loggerLevel);
	logger = this->mpLogger->GetSubLogger(name, loggerLevel);

	Proxy* proxy = new Proxy(name, id, config, logger);

	RTIDNP3_ARRAY_ADD_ELEMENT_EXPAND(this->proxies, this->proxiesData, proxy,
			added);
	if (!added) {
		LOG_BLOCK(apl::LEV_ERROR, "Failed to create DNP3 proxy : " << name);
		delete proxy;
		return RTIDNP3_PROXY_ID_NONE;
	}

	LOG_BLOCK(apl::LEV_INFO,
			"New DNP3 proxy created : " << name << " (id=" << id << ")");

	return id;
}

bool MessageBus::DeleteProxy(RTIDNP3_ProxyId_t id) {

	if (id < this->proxiesData->currentSize) {
		int removed = 0;
		Proxy* p = RTIDNP3_ARRAY_GET(this->proxies,id);
		RTIDNP3_ARRAY_REMOVE_ELEMENT_BY_INDEX(this->proxies, this->proxiesData,
				id, removed);
		if (removed) {
			delete p;

			LOG_BLOCK(apl::LEV_INFO, "Deleted DNP3 proxy with id=" << id);

			return true;
		}
	}

	LOG_BLOCK(apl::LEV_ERROR, "Failed to delete DNP3 proxy with id=" << id);

	return false;
}

Proxy** MessageBus::GetProxies(int *size_out) {

	if (size_out) {
		*size_out = this->proxiesData->currentSize;
	}

	return this->proxies;
}

RTIDNP3_ConnectionId_t MessageBus::NewConnection(RTIDNP3_ProxyId_t proxyId,
		RTIDNP3_ConnectionConfig *config) {

	Proxy * proxy;
	RTIDNP3_ConnectionId_t connId = 0;
	int added;

	if (proxyId == RTIDNP3_PROXY_ID_NONE) {
		return RTIDNP3_CONNECTION_ID_NONE;
	}

	proxy = RTIDNP3_ARRAY_GET(this->proxies,proxyId);
	connId = this->connectionsData->currentSize;

	Connection *conn = proxy->NewConnection(connId, config);

	RTIDNP3_ARRAY_ADD_ELEMENT_EXPAND(this->connections, this->connectionsData,
			conn, added);

	if (!added) {
		proxy->DeleteConnection(connId);

		LOG_BLOCK(apl::LEV_ERROR,
				"Failed to create a new connection on proxy (id=" << proxyId
						<< ")");

		return RTIDNP3_CONNECTION_ID_NONE;
	}

	apl::dnp::APDUProxy* apduProxy = conn->GetAPDUProxy();
	apduProxy->AddAPDUListener(this);

	LOG_BLOCK(apl::LEV_INFO,
			"New connection created on proxy (id=" << proxyId << ")");

	return connId;
}

bool MessageBus::DeleteConnection(RTIDNP3_ConnectionId_t id) {

	Proxy *proxy = NULL;
	Connection *conn = NULL;
	int removed;

	conn = this->connections[id];

	RTIDNP3_ARRAY_REMOVE_ELEMENT_BY_INDEX(this->connections,
			this->connectionsData, id, removed);

	if (!removed) {

		LOG_BLOCK(apl::LEV_ERROR,
				"Failed to delete connection (id=" << id << ")");

		return false;
	}

	proxy = conn->GetProxy();

	apl::dnp::APDUProxy* apduProxy = conn->GetAPDUProxy();
	apduProxy->RemoveAPDUListener(this);

	bool result = proxy->DeleteConnection(id);

	if (!result) {
		LOG_BLOCK(apl::LEV_ERROR,
				"Failed to delete connection (id=" << id << ") on proxy "
						<< proxy->GetName());
	} else {
		LOG_BLOCK(apl::LEV_INFO,
				"Deleted connection (id=" << id << ") on proxy "
						<< proxy->GetName());
	}

	return result;
}

Connection** MessageBus::GetConnections(int *size_out) {

	if (size_out != NULL) {
		*size_out = this->connectionsData->currentSize;
	}

	return this->connections;
}

bool MessageBus::AddMessageBusListener(RTIDNP3_MessageBusListener *listener) {

	int added = 0;
	RTIDNP3_ARRAY_ADD_ELEMENT_EXPAND(this->listeners, this->listenersData,
			listener, added);

	if (added) {
		LOG_BLOCK(apl::LEV_INFO, "New listener registered: " << listener);
	} else {
		LOG_BLOCK(apl::LEV_ERROR, "Listener NOT registered: " << listener);
	}

	return added;
}

bool MessageBus::RemoveMessageBusListener(
		RTIDNP3_MessageBusListener *listener) {
	int removed = 0;
	RTIDNP3_ARRAY_REMOVE_ELEMENT(this->listeners, this->listenersData, listener,
			removed);

	if (removed) {
		LOG_BLOCK(apl::LEV_INFO, "Listener removed: " << listener);
	} else {
		LOG_BLOCK(apl::LEV_ERROR, "Listener NOT removed: " << listener);
	}

	return removed;
}

RTIDNP3_ReturnCode_t MessageBus::Send(RTIDNP3_APDU *apdu) {

	int src = apdu->info.src;
	int dest = apdu->info.dest;

	LOG_BLOCK(apl::LEV_INFO,
			"Sending new APDU of type " << apdu->info.type << " from " << src
					<< " to " << dest);

	Connection * conn = this->GetConnection(src, dest);

	if (conn == NULL) {
		LOG_BLOCK(apl::LEV_ERROR,
				"Cannot find a connection for (src=" << src << ", dest=" << dest
						<< ")");
		return RTIDNP3_RETCODE_ERROR;
	} else {
		string& connName = conn->GetName();
		string& proxyName = conn->GetProxy()->GetName();
		LOG_BLOCK(apl::LEV_INFO,
				"Using connection " << connName << " on proxy " << proxyName
						<< " to send APDU");

	}

	apl::dnp::APDUProxy* apduProxy = conn->GetAPDUProxy();

	RTIDNP3_ReturnCode_t retcode = RTIDNP3_RETCODE_ERROR;

	apl::dnp::APDU *apduToSend = conn->GetAPDU();
	apduToSend->Write((uint8_t*) apdu->buffer, apdu->buffer_size);
	apduToSend->Interpret();
	enum apl::CommandStatus status;

	switch (apdu->info.type) {
	case RTIDNP3_APDU_REQUEST: {
		status = apduProxy->SendRequestAPDU(*apduToSend);
		break;
	}
	case RTIDNP3_APDU_RESPONSE: {
		status = apduProxy->SendResponseAPDU(*apduToSend);
		break;
	}
	case RTIDNP3_APDU_UNSOL_RESPONSE: {
		status = apduProxy->SendUnsolResponseAPDU(*apduToSend);
		break;
	}
	}

	switch (status) {
	case apl::CS_SUCCESS: {
		retcode = RTIDNP3_RETCODE_OK;
		break;
	}
	default: {
		retcode = RTIDNP3_RETCODE_ERROR;
		break;
	}
	}

	return retcode;
}

Connection* MessageBus::GetConnection(int src, int dest) {
	for (int i = 0; i < this->proxiesData->currentSize; i++) {
		Proxy* proxy = this->proxies[i];

		if (proxy->GetConfig()->localAddr != src) {
			continue;
		} else {
			int j = 0;
			Connection** conns = proxy->GetConnections(&j);
			for (; j > 0; j--) {
				Connection *conn = conns[j - 1];
				if (conn->GetConfig()->remoteAddr == dest) {
					return conn;
				}
			}
		}
	}

	return NULL;
}

std::string& MessageBus::getName() {
	return *(this->listenerName);
}

void MessageBus::newRequestAPDU(apl::dnp::APDUProxy* proxy,
		apl::dnp::APDU& apdu) {

	LOG_BLOCK(apl::LEV_INFO,
			"New Request APDU received from APDU proxy : " << apdu.ToString());

	RTIDNP3_APDU cAPDU;
	cAPDU.buffer_size = apdu.Size();
	cAPDU.buffer = calloc(cAPDU.buffer_size, sizeof(unsigned char));
	memcpy(cAPDU.buffer, apdu.GetBuffer(), cAPDU.buffer_size);
	cAPDU.info.dest = apdu.GetInfo()->dest;
	cAPDU.info.src = apdu.GetInfo()->src;
	cAPDU.info.master = apdu.GetInfo()->master;
	cAPDU.info.type = RTIDNP3_APDU_REQUEST;

	LOG_BLOCK(apl::LEV_INFO,
			"Notifying " << this->listenersData->currentSize << " listeners");

	for (int i = 0; i < this->listenersData->currentSize; ++i) {
		RTIDNP3_MessageBusListener *listener = this->listeners[i];
		RTIDNP3_FilterResult_t interested = listener->filterAPDU(listener,
				&cAPDU, (RTIDNP3_MessageBus*) this);

		if (interested == RTIDNP3_FILTER_PASS) {
			listener->onAPDU(listener, &cAPDU, (RTIDNP3_MessageBus*) this);
		}
	}

}

void MessageBus::newResponseAPDU(apl::dnp::APDUProxy* proxy,
		apl::dnp::APDU& apdu) {

	LOG_BLOCK(apl::LEV_INFO,
			"New Response APDU received from APDU proxy : " << apdu.ToString());

	RTIDNP3_APDU cAPDU;
	cAPDU.buffer_size = apdu.Size();
	cAPDU.buffer = calloc(cAPDU.buffer_size, sizeof(unsigned char));
	memcpy(cAPDU.buffer, apdu.GetBuffer(), cAPDU.buffer_size);
	cAPDU.info.dest = apdu.GetInfo()->dest;
	cAPDU.info.src = apdu.GetInfo()->src;
	cAPDU.info.master = apdu.GetInfo()->master;
	cAPDU.info.type = RTIDNP3_APDU_RESPONSE;

	LOG_BLOCK(apl::LEV_INFO,
			"Notifying " << this->listenersData->currentSize << " listeners");

	for (int i = 0; i < this->listenersData->currentSize; ++i) {
		RTIDNP3_MessageBusListener *listener = this->listeners[i];
		RTIDNP3_FilterResult_t interested = listener->filterAPDU(listener,
				&cAPDU, (RTIDNP3_MessageBus*) this);

		if (interested == RTIDNP3_FILTER_PASS) {
			listener->onAPDU(listener, &cAPDU, (RTIDNP3_MessageBus*) this);
		}
	}

}

void MessageBus::newUnsolResponseAPDU(apl::dnp::APDUProxy* proxy,
		apl::dnp::APDU& apdu) {

	LOG_BLOCK(apl::LEV_INFO,
			"New Unsolicited Response APDU received from APDU proxy : "
					<< apdu.ToString());

	RTIDNP3_APDU cAPDU;
	cAPDU.buffer_size = apdu.Size();
	cAPDU.buffer = calloc(cAPDU.buffer_size, sizeof(unsigned char));
	memcpy(cAPDU.buffer, apdu.GetBuffer(), cAPDU.buffer_size);
	cAPDU.info.dest = apdu.GetInfo()->dest;
	cAPDU.info.src = apdu.GetInfo()->src;
	cAPDU.info.master = apdu.GetInfo()->master;
	cAPDU.info.type = RTIDNP3_APDU_UNSOL_RESPONSE;

	LOG_BLOCK(apl::LEV_INFO,
			"Notifying " << this->listenersData->currentSize << " listeners");

	for (int i = 0; i < this->listenersData->currentSize; ++i) {
		RTIDNP3_MessageBusListener *listener = this->listeners[i];
		RTIDNP3_FilterResult_t interested = listener->filterAPDU(listener,
				&cAPDU, (RTIDNP3_MessageBus*) this);

		if (interested == RTIDNP3_FILTER_PASS) {
			listener->onAPDU(listener, &cAPDU, (RTIDNP3_MessageBus*) this);
		}
	}

}

}
}
