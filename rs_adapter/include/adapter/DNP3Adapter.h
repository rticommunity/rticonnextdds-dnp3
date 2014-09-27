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

#ifndef DNP3ADAPTER_H_
#define DNP3ADAPTER_H_


#ifdef __cplusplus
extern "C" {
#endif

#include <routingservice/routingservice_adapter.h>

struct RTI_RoutingServiceAdapterPlugin * DNP3Adapter_create(
		const struct RTI_RoutingServiceProperties * properties,
		RTI_RoutingServiceEnvironment * env);

void DNP3Adapter_delete(
		struct RTI_RoutingServiceAdapterPlugin *plugin,
		RTI_RoutingServiceEnvironment *env);

RTI_RoutingServiceConnection DNP3Adapter_createConnection(
		struct RTI_RoutingServiceAdapterPlugin *plugin,
		const char *routing_service_name,
		const char *routing_service_group_name,
		const struct RTI_RoutingServiceStreamReaderListener *input_stream_discovery_listener,
		const struct RTI_RoutingServiceStreamReaderListener *output_stream_discovery_listener,
		const struct RTI_RoutingServiceTypeInfo **registeredTypes,
		int registeredTypeCount,
		const struct RTI_RoutingServiceProperties *properties,
		RTI_RoutingServiceEnvironment *env);

void DNP3Adapter_deleteConnection(
		struct RTI_RoutingServiceAdapterPlugin *plugin,
		RTI_RoutingServiceConnection connection,
		RTI_RoutingServiceEnvironment *env);


#ifdef __cplusplus
}
#endif

#endif /* DNP3ADAPTER_H_ */
