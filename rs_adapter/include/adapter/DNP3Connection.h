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

#ifndef DNP3CONNECTION_H_
#define DNP3CONNECTION_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <routingservice/routingservice_adapter.h>

#include <dnp3msgbus/MessageBus.h>

typedef struct DNP3Connection {
	char *name;
	RTIDNP3_MessageBus *msgBus;
	RTIDNP3_ConnectionId_t* proxyIds;
	int proxiesSize;
	RTIDNP3_ConnectionId_t* connectionIds;
	int connectionsSize;
} DNP3Connection;

RTI_RoutingServiceSession DNP3Connection_createSession(
		RTI_RoutingServiceConnection connection,
		const struct RTI_RoutingServiceProperties *properties,
		RTI_RoutingServiceEnvironment *env);

void DNP3Connection_deleteSession(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceSession session,
		RTI_RoutingServiceEnvironment *env);

const char* DNP3Connection_toString(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceEnvironment *env);

RTI_RoutingServiceStreamReader DNP3Connection_createStreamReader(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceSession session,
		const struct RTI_RoutingServiceStreamInfo *stream_info,
		const struct RTI_RoutingServiceProperties *properties,
		const struct RTI_RoutingServiceStreamReaderListener *listener,
		RTI_RoutingServiceEnvironment *env);

void DNP3Connection_deleteStreamReader(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceStreamReader stream_reader,
		RTI_RoutingServiceEnvironment *env);

RTI_RoutingServiceStreamWriter DNP3Connection_createStreamWriter(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceSession session,
		const struct RTI_RoutingServiceStreamInfo *stream_info,
		const struct RTI_RoutingServiceProperties *properties,
		RTI_RoutingServiceEnvironment *env);

void DNP3Connection_deleteStreamWriter(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceStreamWriter stream_writer,
		RTI_RoutingServiceEnvironment *env);

RTI_RoutingServiceStreamReader DNP3Connection_getInputDiscoveryReader(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceEnvironment *env);

RTI_RoutingServiceStreamReader DNP3Connection_getOutputDiscoveryReader(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceEnvironment *env);

RTI_RoutingServiceTypeRepresentation DNP3Connection_copyTypeRepresentation(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceTypeRepresentationKind type_representation_kind,
		RTI_RoutingServiceTypeRepresentation type_representation,
		RTI_RoutingServiceEnvironment *env);

void DNP3Connection_deleteTypeRepresentation(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceTypeRepresentationKind type_representation_kind,
		RTI_RoutingServiceTypeRepresentation type_representation,
		RTI_RoutingServiceEnvironment *env);

const struct RTI_RoutingServiceProperties * DNP3Connection_getAttributes(
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceEnvironment * env);

void DNP3Connection_update(
		RTI_RoutingServiceAdapterEntity entity,
		const struct RTI_RoutingServiceProperties *properties,
		RTI_RoutingServiceEnvironment *env);

void DNP3Session_update(
		RTI_RoutingServiceAdapterEntity entity,
		const struct RTI_RoutingServiceProperties *properties,
		RTI_RoutingServiceEnvironment *env);



#ifdef __cplusplus
}
#endif

#endif /* DNP3CONNECTION_H_ */
