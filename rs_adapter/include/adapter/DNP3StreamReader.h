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

#ifndef DNP3STREAMREADER_H_
#define DNP3STREAMREADER_H_


#ifdef __cplusplus
extern "C" {
#endif


#include <routingservice/routingservice_adapter.h>

#include <dnp3msgbus/Utils.h>
#include <dnp3msgbus/MessageBus.h>

#include "dds_c/SimpleAPDU.h"
#include "APDUConverter.h"

typedef struct DNP3Filter {
	int src;
	int dest;
	com_rti_dnp3_APDUType type;
} DNP3Filter;

typedef struct DNP3Reader {
	struct RTIDNP3_MessageBusListener listener;
	const struct RTI_RoutingServiceStreamReaderListener *streamReaderListener;
	char *filterStr;
	DNP3Filter filter;
	RTIDNP3_APDU **apdus;
	RTIDNP3_ArrayData *apdusData;
	APDUConverter *converter;
	char *name;
} DNP3Reader;

void DNP3Reader_parseFilter(
		DNP3Reader *reader);

void DNP3Reader_onAPDU(
		RTIDNP3_MessageBusListener *listener,
		RTIDNP3_APDU *apdu,
		RTIDNP3_MessageBus *bus
		);

RTIDNP3_FilterResult_t DNP3Reader_filterAPDU(
		RTIDNP3_MessageBusListener *listener,
		RTIDNP3_APDU *apdu,
		RTIDNP3_MessageBus *bus
		);

DNP3Reader* DNP3Reader_new(
		const char *name,
		const char *filter,
		const struct RTI_RoutingServiceStreamReaderListener *listener);

void DNP3Reader_delete(DNP3Reader* reader);

RTIDNP3_FilterResult_t DNP3Reader_filter(
		DNP3Reader *reader, RTIDNP3_APDU *apdu);

void* DNP3Reader_NotificationThread(
		void *arg);

void DNP3StreamReader_read(
		RTI_RoutingServiceStreamReader stream_reader,
		RTI_RoutingServiceSample **sample_list,
		RTI_RoutingServiceSampleInfo **info_list, int *count,
		RTI_RoutingServiceEnvironment *env);

void DNP3StreamReader_returnLoan(
		RTI_RoutingServiceStreamReader stream_reader,
		RTI_RoutingServiceSample *sample_list,
		RTI_RoutingServiceSampleInfo *info_list, int count,
		RTI_RoutingServiceEnvironment *env);

void DNP3StreamReader_update(
		RTI_RoutingServiceAdapterEntity entity,
		const struct RTI_RoutingServiceProperties *properties,
		RTI_RoutingServiceEnvironment *env);


#ifdef __cplusplus
}
#endif

#endif /* DNP3STREAMREADER_H_ */
