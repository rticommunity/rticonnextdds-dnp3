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

#ifndef RTIDNP3_MESSAGEBUSIMPL_H_
#define RTIDNP3_MESSAGEBUSIMPL_H_

#include <utility>

#include <boost/ptr_container/ptr_map.hpp>

#include <APL/Log.h>
#include <APL/Loggable.h>
#include <DNP3/APDUProxy.h>
#include <DNP3/AsyncStackManager.h>
#include <DNP3/APDUListener.h>

#include "Utils.h"
#include "MessageBus.h"

#define RTIDNP3_LOG_LEV_CONVERT_TO_DNP3(in,out) \
	switch (in) { \
	case RTIDNP3_LOG_LEV_EVENT: { \
		out = apl::LEV_EVENT; \
		break; \
	} \
	case RTIDNP3_LOG_LEV_ERROR: { \
		out = apl::LEV_ERROR; \
		break; \
	} \
	case RTIDNP3_LOG_LEV_WARNING: { \
		out = apl::LEV_WARNING; \
		break; \
	} \
	case RTIDNP3_LOG_LEV_INFO: { \
		out = apl::LEV_INFO; \
		break; \
	} \
	case RTIDNP3_LOG_LEV_INTERPRET: { \
		out = apl::LEV_INTERPRET; \
		break; \
	} \
	case RTIDNP3_LOG_LEV_COMM: { \
		out = apl::LEV_COMM; \
		break; \
	} \
	case RTIDNP3_LOG_LEV_DEBUG: { \
		out = apl::LEV_DEBUG; \
		break; \
	} \
	}

#define RTIDNP3_SERIAL_PARITY_CONVERT_TO_DNP3(in,out) \
	switch (in) { \
	case RTIDNP3_SERIAL_PARITY_NONE: { \
		out = apl::PAR_NONE; \
		break; \
	} \
	case RTIDNP3_SERIAL_PARITY_EVEN: { \
		out = apl::PAR_EVEN; \
		break; \
	} \
	case RTIDNP3_SERIAL_PARITY_ODD: { \
		out = apl::PAR_ODD; \
		break; \
	}\
	}

#define RTIDNP3_SERIAL_FLOW_CONVERT_TO_DNP3(in,out) \
	switch (in) { \
	case RTIDNP3_SERIAL_FLOW_NONE: { \
		out = apl::FLOW_NONE; \
		break; \
	} \
	case RTIDNP3_SERIAL_FLOW_HARDWARE: { \
		out = apl::FLOW_HARDWARE; \
		break; \
	} \
	case RTIDNP3_SERIAL_FLOW_XONXOFF: { \
		out = apl::FLOW_XONXOFF; \
		break; \
	}\
	}

/* takes a RTIDNP3_SerialSettings* and
 * converts it to an apl::SerialSettings* */
#define RTIDNP3_SerialConfig_CONVERTO_TO_DNP3(in,out) \
{\
	RTIDNP3_SERIAL_FLOW_CONVERT_TO_DNP3((in)->flowType,(out)->mFlowType);\
	RTIDNP3_SERIAL_PARITY_CONVERT_TO_DNP3((in)->parity,(out)->mParity);\
	(out)->mBaud = (in)->baud; \
	(out)->mDataBits = (in)->dataBits; \
	(out)->mDevice = (in)->device; \
	(out)->mStopBits = (in)->stopBits;\
}

extern "C" {
#define RTIDNP3_EXTCONFIG_PROPS_ENTITY_NAME 			"entity-name"
#define RTIDNP3_EXTCONFIG_PROPS_IS_MASTER 				"is-master"
#define RTIDNP3_EXTCONFIG_PROPS_ADDR 					"address"
#define RTIDNP3_EXTCONFIG_PROPS_LOG_LEVEL 				"log-level"
/*#define RTIDNP3_EXTCONFIG_PROPS_PHYS_PORTS 			"phys-ports"*/

#define RTIDNP3_EXTCONFIG_PROPS_PHYS_NAME 				"name"
#define RTIDNP3_EXTCONFIG_PROPS_PHYS_TIMEOUT 			"timeout"
#define RTIDNP3_EXTCONFIG_PROPS_PHYS_TYPE 				"type"

#define RTIDNP3_EXTCONFIG_PROPS_PHYS_SERIAL_BAUD 		"baud"
#define RTIDNP3_EXTCONFIG_PROPS_PHYS_SERIAL_DATA_BITS 	"data-bits"
#define RTIDNP3_EXTCONFIG_PROPS_PHYS_SERIAL_DEVICE 		"device"
#define RTIDNP3_EXTCONFIG_PROPS_PHYS_SERIAL_FLOW 		"flow-type"
#define RTIDNP3_EXTCONFIG_PROPS_PHYS_SERIAL_PARITY 		"parity"
#define RTIDNP3_EXTCONFIG_PROPS_PHYS_SERIAL_STOP_BITS	"stop-bits"

#define RTIDNP3_EXTCONFIG_PROPS_PHYS_TCP_ADDRESS 		"address"
#define RTIDNP3_EXTCONFIG_PROPS_PHYS_TCP_PORT 			"port"

#define RTIDNP3_EXTCONFIG_PROPS_CONN_TIMEOUT_APP		"timeout-app"
#define RTIDNP3_EXTCONFIG_PROPS_CONN_TIMEOUT_LINK		"timeout-link"
#define RTIDNP3_EXTCONFIG_PROPS_CONN_FRAG_SIZE			"frag-size"
#define RTIDNP3_EXTCONFIG_PROPS_CONN_RETRIES			"retries"
#define RTIDNP3_EXTCONFIG_PROPS_CONN_PHYS_PORT			"phys-port"
#define RTIDNP3_EXTCONFIG_PROPS_CONN_USE_CONFIRMS		"use-confirms"

void RTIDNP3_ExtConfig_disposeConnectionConfig(
		RTIDNP3_ConnectionConfig* connConfig);
int RTIDNP3_ExtConfig_extractConnectionConfig(
		const char *entityName, int connNum,
		RTIDNP3_Map *configMap,
		RTIDNP3_ConnectionConfig* connConfig);
void RTIDNP3_ExtConfig_disposeProxyConfig(
		RTIDNP3_ProxyConfig* proxyConfig);
const char* RTIDNP3_ExtConfig_extractProxyConfig(
		int proxyNum,
		RTIDNP3_Map *configMap,
		RTIDNP3_ProxyConfig* proxyConfigOut);
int RTIDNP3_ExtConfig_extractPhysConnConfig(
		const char *proxyName, int connNum,
		RTIDNP3_Map *configMap,
		RTIDNP3_PhysicalConnectionConfig **physConnConfigOut);
void RTIDNP3_ExtConfig_disposePhysConnConfig(
		RTIDNP3_PhysicalConnectionConfig *physConnConfig);
}

namespace rti {
namespace dnp3 {

class Proxy;

class Connection: protected virtual apl::Loggable {
public:
	Connection(
			string& connName,
			RTIDNP3_ConnectionId_t id,
			RTIDNP3_ConnectionConfig *config,
			Proxy *proxy,
			apl::Logger *logger);
	~Connection();

	string& GetName() {
		return name;
	}

	RTIDNP3_ConnectionId_t GetId() {
		return id;
	}
	RTIDNP3_ConnectionConfig* GetConfig() {
		return &config;
	}
	Proxy* GetProxy() {
		return proxy;
	}
	apl::dnp::APDUProxy* GetAPDUProxy() {
		return apduProxy;
	}

	apl::dnp::APDU* GetAPDU() {
		return apdu;
	}

	bool operator==(
			const Connection&);
	bool operator<(
			const Connection&);

private:
	string name;
	RTIDNP3_ConnectionId_t id;
	RTIDNP3_ConnectionConfig config;
	Proxy *proxy;
	apl::dnp::APDUProxy *apduProxy;
	string apduProxyName;

	apl::dnp::APDU *apdu;

	apl::FilterLevel logLevel;

	bool CreateAPDUProxy();
	bool DeleteAPDUProxy();
};

class Proxy: protected virtual apl::Loggable {
public:
	Proxy(string& name, RTIDNP3_ProxyId_t id, RTIDNP3_ProxyConfig *config,
			apl::Logger *logger);
	~Proxy();

	string& GetName() {
		return name;
	}
	RTIDNP3_ProxyId_t GetId() {
		return id;
	}
	RTIDNP3_ProxyConfig* GetConfig() {
		return &config;
	}
	apl::dnp::AsyncStackManager* GetStackManager() {
		return stackManager;
	}

	Connection* NewConnection(
			RTIDNP3_ConnectionId_t id,
			RTIDNP3_ConnectionConfig *config);
	bool DeleteConnection(
			RTIDNP3_ConnectionId_t id);
	Connection** GetConnections(
			int *size_out);

	bool operator==(
			Proxy&);
	bool operator<(
			Proxy&);

private:
	string name;
	RTIDNP3_ProxyId_t id;
	RTIDNP3_ProxyConfig config;
	apl::dnp::AsyncStackManager *stackManager;
	Connection **connections;
	RTIDNP3_ArrayData *connectionsData;

	apl::FilterLevel logLevel;

	bool CreateStackManager();
	bool DeleteStackManager();
	bool CreatePhysicalConnections();
	bool DeletePhysicalConnections();
};

class MessageBus: public apl::dnp::APDUListener {
public:
	MessageBus();
	~MessageBus();

	RTIDNP3_ProxyId_t NewProxy(
			string& name,
			RTIDNP3_ProxyConfig *config);
	bool DeleteProxy(
			RTIDNP3_ProxyId_t id);
	Proxy** GetProxies(
			int *size_out);

	RTIDNP3_ConnectionId_t NewConnection(
			RTIDNP3_ProxyId_t proxy,
			RTIDNP3_ConnectionConfig *config);
	bool DeleteConnection(
			RTIDNP3_ConnectionId_t id);
	Connection** GetConnections(
			int *size_out);

	bool AddMessageBusListener(
			RTIDNP3_MessageBusListener *listener);
	bool RemoveMessageBusListener(
			RTIDNP3_MessageBusListener *listener);

	RTIDNP3_ReturnCode_t Send(
			RTIDNP3_APDU *apdu);

	std::string& getName();

	void newRequestAPDU(
			apl::dnp::APDUProxy* proxy, apl::dnp::APDU& apdu);

	void newResponseAPDU(
			apl::dnp::APDUProxy* proxy, apl::dnp::APDU& apdu);

	void newUnsolResponseAPDU(
			apl::dnp::APDUProxy* proxy, apl::dnp::APDU& apdu);

private:

	string* listenerName;

	Proxy **proxies;
	RTIDNP3_ArrayData *proxiesData;

	Connection **connections;
	RTIDNP3_ArrayData *connectionsData;

	RTIDNP3_MessageBusListener **listeners;
	RTIDNP3_ArrayData *listenersData;

	apl::Logger *mpLogger;
	apl::EventLog log;
	apl::FilterLevel logLevel;

	Connection* GetConnection(
			int src, int dest);

};

}
}

#endif /* RTIDNP3_MESSAGEBUSIMPL_H_ */
