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
#include <string.h>
#include <dnp3msgbus/MessageBus.h>
#include <dnp3msgbus/Utils.h>

#include "DNP3Adapter.h"
#include "DNP3Connection.h"
#include "DNP3StreamReader.h"
#include "DNP3StreamWriter.h"

struct RTI_RoutingServiceAdapterPlugin * DNP3Adapter_create(
		const struct RTI_RoutingServiceProperties * properties,
		RTI_RoutingServiceEnvironment * env) {

	struct RTI_RoutingServiceAdapterPlugin* adapter = NULL;
	struct RTI_RoutingServiceVersion version = { 1, 0, 0, 0 };
	RTIDNP3_MessageBus * msgBus = NULL;

	adapter = (struct RTI_RoutingServiceAdapterPlugin*) calloc(1,
			sizeof(struct RTI_RoutingServiceAdapterPlugin));
	if (adapter == NULL) {
		RTI_RoutingServiceEnvironment_set_error(env,
				"Memory allocation error");
		return NULL;
	}

	RTI_RoutingServiceAdapterPlugin_initialize(adapter);
	adapter->plugin_version = version;

	adapter->adapter_plugin_delete = DNP3Adapter_delete;
	adapter->adapter_plugin_create_connection = DNP3Adapter_createConnection;
	adapter->adapter_plugin_delete_connection = DNP3Adapter_deleteConnection;

	adapter->connection_create_session = DNP3Connection_createSession;
	adapter->connection_delete_session = DNP3Connection_deleteSession;
	adapter->connection_create_stream_reader =
			DNP3Connection_createStreamReader;
	adapter->connection_delete_stream_reader =
			DNP3Connection_deleteStreamReader;
	adapter->connection_create_stream_writer =
			DNP3Connection_createStreamWriter;
	adapter->connection_delete_stream_writer =
			DNP3Connection_deleteStreamWriter;
	adapter->connection_copy_type_representation =
			DNP3Connection_copyTypeRepresentation;
	adapter->connection_delete_type_representation =
			DNP3Connection_deleteTypeRepresentation;
	adapter->connection_get_attributes = DNP3Connection_getAttributes;
	adapter->connection_get_input_stream_discovery_reader =
			DNP3Connection_getInputDiscoveryReader;
	adapter->connection_get_output_stream_discovery_reader =
			DNP3Connection_getOutputDiscoveryReader;
	adapter->connection_to_string = DNP3Connection_toString;
	adapter->connection_update = DNP3Connection_update;

	adapter->session_update = DNP3Session_update;

	adapter->stream_reader_read = DNP3StreamReader_read;
	adapter->stream_reader_return_loan = DNP3StreamReader_returnLoan;
	adapter->stream_reader_update = DNP3StreamReader_update;

	adapter->stream_writer_write = DNP3StreamWriter_write;
	adapter->stream_writer_update = DNP3StreamWriter_update;

	adapter->user_object = NULL;
	msgBus = RTIDNP3_MessageBus_new();
	adapter->user_object = (void*) msgBus;

	return adapter;
}

void DNP3Adapter_delete(
		struct RTI_RoutingServiceAdapterPlugin *plugin,
		RTI_RoutingServiceEnvironment *env) {

	RTIDNP3_MessageBus* msgBus = NULL;



	msgBus = (RTIDNP3_MessageBus*) plugin->user_object;
	RTIDNP3_MessageBus_delete(msgBus);
	plugin->user_object = NULL;

}

RTI_RoutingServiceConnection DNP3Adapter_createConnection(
		struct RTI_RoutingServiceAdapterPlugin *plugin,
		const char *routing_service_name,
		const char *routing_service_group_name,
		const struct RTI_RoutingServiceStreamReaderListener *input_stream_discovery_listener,
		const struct RTI_RoutingServiceStreamReaderListener *output_stream_discovery_listener,
		const struct RTI_RoutingServiceTypeInfo **registeredTypes,
		int registeredTypeCount,
		const struct RTI_RoutingServiceProperties *properties,
		RTI_RoutingServiceEnvironment *env) {

	RTIDNP3_ProxyConfig proxyConfig = RTIDNP3_ProxyConfig_INITIALIZER;
	RTIDNP3_ConnectionConfig connectionConfig =
			RTIDNP3_ConnectionConfig_INITIALIZER;
	RTIDNP3_MessageBus* msgBus = NULL;
	DNP3Connection *dnp3Connection = NULL;
	const char* entityName = NULL;
	int i = 0;
	unsigned char added = 0;
	const char *configFile = NULL;
	char keyBuff[1024];
	RTIDNP3_ReturnCode_t retcode = RTIDNP3_RETCODE_ERROR;

	bzero(keyBuff, 1024);

	msgBus = (RTIDNP3_MessageBus*) plugin->user_object;

	dnp3Connection = (DNP3Connection*) malloc(sizeof(DNP3Connection));

	dnp3Connection->msgBus = msgBus;
	dnp3Connection->name = strdup(routing_service_name);

	configFile = RTI_RoutingServiceProperties_lookup_property(
			properties,
			"dnp3config");

	retcode = RTIDNP3_MessageBus_loadConfig(msgBus, configFile,
			&dnp3Connection->proxyIds, &dnp3Connection->proxiesSize,
			&dnp3Connection->connectionIds,
			&dnp3Connection->connectionsSize);

	if (retcode != RTIDNP3_RETCODE_OK) {
		RTI_RoutingServiceEnvironment_set_error(env,
				"Cannot load DNP3 configuration file %s\n", configFile);
		free(dnp3Connection);
		return NULL;
	}

	return (RTI_RoutingServiceConnection) dnp3Connection;

}

void DNP3Adapter_deleteConnection(
		struct RTI_RoutingServiceAdapterPlugin *plugin,
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceEnvironment *env) {

	DNP3Connection* dnp3Connection = NULL;
	RTIDNP3_MessageBus* msgBus = NULL;

	dnp3Connection = (DNP3Connection*) connection;
	msgBus = (RTIDNP3_MessageBus*) plugin->user_object;

	free(dnp3Connection->proxyIds);
	free(dnp3Connection->connectionIds);
	free(dnp3Connection);

}

