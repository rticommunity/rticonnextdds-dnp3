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

#ifndef RTIDNP3_MESSAGEBUS_H_
#define RTIDNP3_MESSAGEBUS_H_

#include "Utils.h"
#include "APDU.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void RTIDNP3_MessageBus;

typedef int RTIDNP3_ProxyId_t;
#define RTIDNP3_PROXY_ID_NONE ((RTIDNP3_ProxyId_t)-1)

typedef enum RTIDNP3_ParityType {
	RTIDNP3_SERIAL_PARITY_NONE,
	RTIDNP3_SERIAL_PARITY_EVEN,
	RTIDNP3_SERIAL_PARITY_ODD
} RTIDNP3_ParityType_t;

RTIDNP3_ParityType_t RTIDNP3_ParityType_fromString(
		const char* str);

typedef enum RTIDNP3_FlowType {
	RTIDNP3_SERIAL_FLOW_NONE,
	RTIDNP3_SERIAL_FLOW_HARDWARE,
	RTIDNP3_SERIAL_FLOW_XONXOFF
} RTIDNP3_FlowType_t;

RTIDNP3_FlowType_t RTIDNP3_FlowType_fromString(
		const char* str);

typedef struct RTIDNP3_TCPServerConfig {
	const char* endpointAddr;
	short int endpointPort;
} RTIDNP3_TCPServerConfig;

#define RTIDNP3_TCPServerConfig_INITIALIZER \
		{ NULL, NULL, 0 }

typedef struct RTIDNP3_TCPClientConfig {
	const char* serverAddr;
	short int serverPort;
} RTIDNP3_TCPClientConfig;

#define RTIDNP3_TCPClientConfig_INITIALIZER \
		{ NULL, NULL, 0 }

typedef struct RTIDNP3_SerialConfig {
	const char* device;
	int baud;
	int dataBits;
	int stopBits;
	RTIDNP3_ParityType_t parity;
	RTIDNP3_FlowType_t flowType;
} RTIDNP3_SerialConfig;

#define RTIDNP3_SerialConfig_INITIALIZER \
		{ NULL, NULL, 0, 0, 0, RTIDNP3_SERIAL_PARITY_NONE, RTIDNP3_SERIAL_FLOW_NONE }

typedef enum RTIDNP3_PhysicalConnectionType {
	RTIDNP3_PHYSICAL_CONNECTION_NONE = 0,
	RTIDNP3_PHYSICAL_CONNECTION_SERIAL = 1,
	RTIDNP3_PHYSICAL_CONNECTION_TCP_SERVER = 2,
	RTIDNP3_PHYSICAL_CONNECTION_TCP_CLIENT = 3,
} RTIDNP3_PhysicalConnectionType_t;

typedef struct RTIDNP3_PhysicalConnectionConfig {
	const char *name;
	RTIDNP3_PhysicalConnectionType_t type;
	long long int physTimeout;
	RTIDNP3_LogLevel_t logLevel;
	void *config;
} RTIDNP3_PhysicalConnectionConfig;

RTIDNP3_PhysicalConnectionType_t RTIDNP3_PhysicalConnectionType_fromString(
		const char* str);

#define RTIDNP3_PhysicalConnectionConfig_INITIALIZER \
		{ NULL, RTIDNP3_PHYSICAL_CONNECTION_NONE, 0, RTIDNP3_LOG_LEV_NONE, NULL }

typedef struct RTIDNP3_ProxyConfig {
	short int localAddr;
	unsigned char isMaster;
	RTIDNP3_LogLevel_t logLevel;
	unsigned int physicalConnectionsNum;
	RTIDNP3_PhysicalConnectionConfig** physicalConnections;
} RTIDNP3_ProxyConfig;

#define RTIDNP3_ProxyConfig_INITIALIZER \
		{ 0, 0, RTIDNP3_LOG_LEV_NONE, 0, NULL }

typedef int RTIDNP3_ConnectionId_t;
#define RTIDNP3_CONNECTION_ID_NONE ((RTIDNP3_ConnectionId_t)-1)
typedef struct RTIDNP3_ConnectionConfig {
	const char *portName;
	short int remoteAddr;
	unsigned char useConfirms;
	long long int linkTimeout;
	long long int appTimeout;
	unsigned long int numRetry;
	unsigned long int fragSize;
} RTIDNP3_ConnectionConfig;

#define RTIDNP3_ConnectionConfig_INITIALIZER \
		{ NULL, 0, 0, 0, 0, 0, 0 }

typedef enum RTIDNP3_FilterResult {
	RTIDNP3_FILTER_PASS, RTIDNP3_FILTER_EXCLUDE
} RTIDNP3_FilterResult_t;

typedef struct RTIDNP3_MessageBusListener RTIDNP3_MessageBusListener;

typedef void (*RTIDNP3_MessageBusListener_OnAPDUFn)(
		RTIDNP3_MessageBusListener *listener,
		RTIDNP3_APDU *apdu,
		RTIDNP3_MessageBus *bus
		);

typedef RTIDNP3_FilterResult_t (*RTIDNP3_MessageBusListener_FilterAPDUFn)(
		RTIDNP3_MessageBusListener *listener,
		RTIDNP3_APDU *apdu,
		RTIDNP3_MessageBus *bus
		);

struct RTIDNP3_MessageBusListener {
	RTIDNP3_MessageBusListener_OnAPDUFn onAPDU;
	RTIDNP3_MessageBusListener_FilterAPDUFn filterAPDU;
	void* listenerData;
};

RTIDNP3_MessageBus* RTIDNP3_MessageBus_new();

void RTIDNP3_MessageBus_delete(
		RTIDNP3_MessageBus* bus);

RTIDNP3_ProxyId_t RTIDNP3_MessageBus_createProxy(
		RTIDNP3_MessageBus *bus,
		const char* name,
		RTIDNP3_ProxyConfig *config
		);

RTIDNP3_ReturnCode_t RTIDNP3_MessageBus_deleteProxy(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_ProxyId_t id
		);

RTIDNP3_ProxyId_t RTIDNP3_MessageBus_getProxyId(
		RTIDNP3_MessageBus *bus,
		const char* name
		);

RTIDNP3_ConnectionId_t RTIDNP3_MessageBus_createConnection(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_ProxyId_t proxy,
		RTIDNP3_ConnectionConfig *config
		);

RTIDNP3_ReturnCode_t RTIDNP3_MessageBus_closeConnection(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_ConnectionId_t connection
		);

RTIDNP3_ConnectionConfig* RTIDNP3_MessageBus_getConnectionConfiguration(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_ConnectionId_t connection
		);

RTIDNP3_ConnectionId_t* RTIDNP3_MessageBus_getProxyConnections(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_ProxyId_t proxy,
		int *size_out);

RTIDNP3_ProxyId_t RTIDNP3_MessageBus_getAssociatedProxy(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_ConnectionId_t connection);

RTIDNP3_ConnectionId_t RTIDNP3_MessageBus_getConnectionId(
		RTIDNP3_MessageBus *bus, int src, int dest);

RTIDNP3_ReturnCode_t RTIDNP3_MessageBus_addMessageBusListener(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_MessageBusListener *listener,
		void *listener_data
		);

RTIDNP3_ReturnCode_t RTIDNP3_MessageBus_removeMessageBusListener(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_MessageBusListener *listener
		);

RTIDNP3_ReturnCode_t RTIDNP3_MessageBus_send(
		RTIDNP3_MessageBus *bus,
		RTIDNP3_APDU *apdu
		);

RTIDNP3_ReturnCode_t RTIDNP3_MessageBus_loadConfig(
		RTIDNP3_MessageBus *msgBus, const char *configFile,
		RTIDNP3_ProxyId_t **loadedProxies,
		int *loadedProxiesNum,
		RTIDNP3_ConnectionId_t **loadedConnections,
		int *loadedConnectionsNum);

#ifdef __cplusplus
}
#endif

#endif /* RTIDNP3_MESSAGEBUS_H_ */
