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

#include "MessageBus.h"

#ifdef __cplusplus
extern "C" {
#endif

void TestMessageBusListener_OnAPDUFn(
		RTIDNP3_MessageBusListener *listener,
		RTIDNP3_APDU *apdu,
		RTIDNP3_MessageBus *bus
		) {
	RTIDNP3_ReturnCode_t retcode = RTIDNP3_RETCODE_ERROR;

	printf(
			"TestMessageBusListener: received APDU of type %d, from %d to %d - Sending it on the BUS\n",
			apdu->info.type, apdu->info.src, apdu->info.dest);

	retcode = RTIDNP3_MessageBus_send(bus, apdu);

	if (retcode != RTIDNP3_RETCODE_OK) {
		printf("TestMessageBusListener: cannot send on the bus!\n");
	}
}

RTIDNP3_FilterResult_t TestMessageBusListener_FilterAPDUFn(
		RTIDNP3_MessageBusListener *listener,
		RTIDNP3_APDU *apdu,
		RTIDNP3_MessageBus *bus
		) {
	return RTIDNP3_FILTER_PASS;
}

int createMasterProxy(
		RTIDNP3_MessageBus *bus) {
	RTIDNP3_ProxyConfig proxyConfig;
	RTIDNP3_PhysicalConnectionConfig *physConnConfig = NULL;
	RTIDNP3_TCPClientConfig tcpClientConfig;
	RTIDNP3_ConnectionConfig connectionConfig;
	RTIDNP3_ProxyId_t proxyId = RTIDNP3_PROXY_ID_NONE;
	RTIDNP3_ConnectionId_t connectionId = RTIDNP3_CONNECTION_ID_NONE;
	RTIDNP3_ReturnCode_t retcode = RTIDNP3_RETCODE_ERROR;

	proxyConfig.isMaster = 1;
	proxyConfig.localAddr = 80;
	proxyConfig.logLevel = RTIDNP3_LOG_LEV_DEBUG;
	proxyConfig.physicalConnectionsNum = 1;
	proxyConfig.physicalConnections =
			(RTIDNP3_PhysicalConnectionConfig**) calloc(
					proxyConfig.physicalConnectionsNum,
					sizeof(RTIDNP3_PhysicalConnectionConfig*));

	/* CREATE A TCP-client physical connection */
	physConnConfig = (RTIDNP3_PhysicalConnectionConfig*) malloc(
			sizeof(RTIDNP3_PhysicalConnectionConfig));
	physConnConfig->name = "TCPClient";
	physConnConfig->logLevel = RTIDNP3_LOG_LEV_DEBUG;
	physConnConfig->physTimeout = 100;
	physConnConfig->type = RTIDNP3_PHYSICAL_CONNECTION_TCP_CLIENT;
	tcpClientConfig.serverAddr = "127.0.0.1";
	tcpClientConfig.serverPort = 4998;
	physConnConfig->config = (void*) &tcpClientConfig;
	proxyConfig.physicalConnections[0] = physConnConfig;

	proxyId = RTIDNP3_MessageBus_createProxy(bus, "MasterProxy",
			&proxyConfig);

	free(proxyConfig.physicalConnections[0]);
	free(proxyConfig.physicalConnections);

	if (proxyId == RTIDNP3_PROXY_ID_NONE) {
		goto error;
	}

	connectionConfig.appTimeout = 1000;
	connectionConfig.linkTimeout = 1000;
	connectionConfig.fragSize = 2048;
	connectionConfig.numRetry = 3;
	connectionConfig.portName = "TCPClient";
	connectionConfig.remoteAddr = 50;
	connectionConfig.useConfirms = 1;

	connectionId = RTIDNP3_MessageBus_createConnection(
			bus, proxyId, &connectionConfig);

	if (connectionId == RTIDNP3_CONNECTION_ID_NONE) {
		goto error;
	}

	return 1;

	error:

	if (proxyId != RTIDNP3_PROXY_ID_NONE) {

		if (connectionId != RTIDNP3_CONNECTION_ID_NONE) {
			RTIDNP3_MessageBus_closeConnection(bus, connectionId);
		}

		RTIDNP3_MessageBus_deleteProxy(bus, proxyId);
	}

	return 0;

}

int createSlaveProxy(
		RTIDNP3_MessageBus *bus) {
	RTIDNP3_ProxyConfig proxyConfig;
	RTIDNP3_PhysicalConnectionConfig *physConnConfig = NULL;
	RTIDNP3_TCPServerConfig tcpServerConfig;
	RTIDNP3_ConnectionConfig connectionConfig;
	RTIDNP3_ProxyId_t proxyId = RTIDNP3_PROXY_ID_NONE;
	RTIDNP3_ConnectionId_t connectionId = RTIDNP3_CONNECTION_ID_NONE;
	RTIDNP3_ReturnCode_t retcode = RTIDNP3_RETCODE_ERROR;

	proxyConfig.isMaster = 0;
	proxyConfig.localAddr = 50;
	proxyConfig.logLevel = RTIDNP3_LOG_LEV_DEBUG;
	proxyConfig.physicalConnectionsNum = 1;
	proxyConfig.physicalConnections =
			(RTIDNP3_PhysicalConnectionConfig**) calloc(
					proxyConfig.physicalConnectionsNum,
					sizeof(RTIDNP3_PhysicalConnectionConfig*));

	/* CREATE A TCP-Server physical connection */
	physConnConfig = (RTIDNP3_PhysicalConnectionConfig*) malloc(
			sizeof(RTIDNP3_PhysicalConnectionConfig));
	physConnConfig->name = "TCPServer";
	physConnConfig->logLevel = RTIDNP3_LOG_LEV_DEBUG;
	physConnConfig->physTimeout = 100;
	physConnConfig->type = RTIDNP3_PHYSICAL_CONNECTION_TCP_SERVER;
	tcpServerConfig.endpointAddr = "127.0.0.1";
	tcpServerConfig.endpointPort = 4999;
	physConnConfig->config = (void*) &tcpServerConfig;
	proxyConfig.physicalConnections[0] = physConnConfig;

	proxyId = RTIDNP3_MessageBus_createProxy(bus, "SlaveProxy",
			&proxyConfig);

	free(proxyConfig.physicalConnections[0]);
	free(proxyConfig.physicalConnections);

	if (proxyId == RTIDNP3_PROXY_ID_NONE) {
		goto error;
	}

	connectionConfig.appTimeout = 1000;
	connectionConfig.linkTimeout = 1000;
	connectionConfig.fragSize = 2048;
	connectionConfig.numRetry = 3;
	connectionConfig.portName = "TCPServer";
	connectionConfig.remoteAddr = 80;
	connectionConfig.useConfirms = 1;

	connectionId = RTIDNP3_MessageBus_createConnection(
			bus, proxyId, &connectionConfig);

	if (connectionId == RTIDNP3_CONNECTION_ID_NONE) {
		goto error;
	}

	return 1;

	error:

	if (proxyId != RTIDNP3_PROXY_ID_NONE) {

		if (connectionId != RTIDNP3_CONNECTION_ID_NONE) {
			RTIDNP3_MessageBus_closeConnection(bus, connectionId);
		}

		RTIDNP3_MessageBus_deleteProxy(bus, proxyId);
	}

	return 0;
}

int main(
		int argc, char **argv) {

	char userInput = 'n';
	RTIDNP3_MessageBus *bus = NULL;
	RTIDNP3_MessageBusListener busListener;
	RTIDNP3_ReturnCode_t retcode = RTIDNP3_RETCODE_ERROR;

	bus = RTIDNP3_MessageBus_new();

	busListener.filterAPDU = TestMessageBusListener_FilterAPDUFn;
	busListener.onAPDU = TestMessageBusListener_OnAPDUFn;
	busListener.listenerData = (void*) bus;

	retcode = RTIDNP3_MessageBus_addMessageBusListener(bus, &busListener, NULL);
	if (retcode != RTIDNP3_RETCODE_OK) {
		goto dispose;
	}

	if (!createSlaveProxy(bus)) {
		goto dispose;
	}

	if (!createMasterProxy(bus)) {
		goto dispose;
	}

	printf("Press enter to exit...");
	scanf("%c", &userInput);

	dispose:

	if (bus != NULL) {

		RTIDNP3_MessageBus_delete(bus);

	}

}

#ifdef __cplusplus
}
#endif
