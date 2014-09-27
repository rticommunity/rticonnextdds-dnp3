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

#ifndef DNP3STREAMWRITER_H_
#define DNP3STREAMWRITER_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <routingservice/routingservice_adapter.h>
#include <dnp3msgbus/MessageBus.h>

#include "APDUConverter.h"

typedef struct DNP3Writer {
	char *name;
	APDUConverter *converter;
	RTIDNP3_MessageBus *msgBus;
} DNP3Writer;

DNP3Writer* DNP3Writer_new(char *name, RTIDNP3_MessageBus *msgBus);

void DNP3Writer_delete(DNP3Writer *writer);

int DNP3StreamWriter_write(
		RTI_RoutingServiceStreamWriter stream_writer,
		const RTI_RoutingServiceSample *sample_list,
		const RTI_RoutingServiceSampleInfo *info_list, int count,
		RTI_RoutingServiceEnvironment *env);

void DNP3StreamWriter_update(
		RTI_RoutingServiceAdapterEntity entity,
		const struct RTI_RoutingServiceProperties *properties,
		RTI_RoutingServiceEnvironment *env);


#ifdef __cplusplus
}
#endif

#endif /* DNP3STREAMWRITER_H_ */
