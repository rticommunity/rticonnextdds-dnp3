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

#include "DNP3Connection.h"
#include "DNP3StreamReader.h"
#include "DNP3StreamWriter.h"

RTI_RoutingServiceSession DNP3Connection_createSession(
		RTI_RoutingServiceConnection connection,
		const struct RTI_RoutingServiceProperties *properties,
		RTI_RoutingServiceEnvironment *env) {
	RTI_RoutingServiceSession session = NULL;

	return session;
}

void DNP3Connection_deleteSession(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceSession session,
		RTI_RoutingServiceEnvironment *env) {

}

const char* DNP3Connection_toString(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceEnvironment *env) {

	return NULL;
}

RTI_RoutingServiceStreamReader DNP3Connection_createStreamReader(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceSession session,
		const struct RTI_RoutingServiceStreamInfo *stream_info,
		const struct RTI_RoutingServiceProperties *properties,
		const struct RTI_RoutingServiceStreamReaderListener *listener,
		RTI_RoutingServiceEnvironment *env) {
	RTI_RoutingServiceStreamReader sreader = NULL;
	DNP3Connection *conn = (DNP3Connection*) connection;
	const char *dnp3Filter = NULL;
	DNP3Reader *reader = NULL;

	printf("[DNP3Connection][%s]--[creating stream reader %s]\n",
				conn->name,
				stream_info->stream_name);

	dnp3Filter = RTI_RoutingServiceProperties_lookup_property(properties,
			"dnp3filter");

	if (dnp3Filter == NULL) {
		RTI_RoutingServiceEnvironment_set_error(env,
				"No dnp3 filter specified, cannot create stream reader");
		return NULL;
	}

	reader = DNP3Reader_new(stream_info->stream_name, dnp3Filter, listener);

	RTIDNP3_MessageBus_addMessageBusListener(conn->msgBus,
			(RTIDNP3_MessageBusListener*) reader, reader);

	return reader;
}

void DNP3Connection_deleteStreamReader(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceStreamReader stream_reader,
		RTI_RoutingServiceEnvironment *env) {

	DNP3Connection *conn = (DNP3Connection*) connection;
	DNP3Reader *reader = (DNP3Reader *) stream_reader;

	RTIDNP3_MessageBus_removeMessageBusListener(conn->msgBus,
			(RTIDNP3_MessageBusListener*) reader);

	DNP3Reader_delete(reader);

}

RTI_RoutingServiceStreamWriter DNP3Connection_createStreamWriter(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceSession session,
		const struct RTI_RoutingServiceStreamInfo *stream_info,
		const struct RTI_RoutingServiceProperties *properties,
		RTI_RoutingServiceEnvironment *env) {

	DNP3Connection *dnp3Conn = (DNP3Connection*) connection;
	DNP3Writer *writer = NULL;

	printf("[DNP3Connection][%s]--[creating stream writer %s]\n",
			dnp3Conn->name,
			stream_info->stream_name);

	writer = DNP3Writer_new(stream_info->stream_name, dnp3Conn->msgBus);

	return (RTI_RoutingServiceStreamWriter) writer;
}

void DNP3Connection_deleteStreamWriter(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceStreamWriter stream_writer,
		RTI_RoutingServiceEnvironment *env) {

	DNP3Writer *writer = (DNP3Writer*) stream_writer;

	DNP3Writer_delete(writer);

}

RTI_RoutingServiceStreamReader DNP3Connection_getInputDiscoveryReader(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceEnvironment *env) {

	return NULL;

}

RTI_RoutingServiceStreamReader DNP3Connection_getOutputDiscoveryReader(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceEnvironment *env) {

	return NULL;

}

RTI_RoutingServiceTypeRepresentation DNP3Connection_copyTypeRepresentation(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceTypeRepresentationKind type_representation_kind,
		RTI_RoutingServiceTypeRepresentation type_representation,
		RTI_RoutingServiceEnvironment *env) {

	return NULL;

}

void DNP3Connection_deleteTypeRepresentation(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceTypeRepresentationKind type_representation_kind,
		RTI_RoutingServiceTypeRepresentation type_representation,
		RTI_RoutingServiceEnvironment *env) {

}

const struct RTI_RoutingServiceProperties * DNP3Connection_getAttributes(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceEnvironment * env) {

	const struct RTI_RoutingServiceProperties * props = NULL;

	return NULL;
}

void DNP3Connection_update(
		RTI_RoutingServiceAdapterEntity entity,
		const struct RTI_RoutingServiceProperties *properties,
		RTI_RoutingServiceEnvironment *env) {

}

void DNP3Session_update(
		RTI_RoutingServiceAdapterEntity entity,
		const struct RTI_RoutingServiceProperties *properties,
		RTI_RoutingServiceEnvironment *env) {

}
