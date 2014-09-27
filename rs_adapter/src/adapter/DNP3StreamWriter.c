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

#include <dnp3msgbus/APDU.h>
#include <string.h>

#include "DNP3StreamWriter.h"

int DNP3StreamWriter_write(
		RTI_RoutingServiceStreamWriter stream_writer,
		const RTI_RoutingServiceSample *sample_list,
		const RTI_RoutingServiceSampleInfo *info_list, int count,
		RTI_RoutingServiceEnvironment *env) {
	DNP3Writer *writer = (DNP3Writer *) stream_writer;
	RTIDNP3_APDU apdu;
	RTIDNP3_ReturnCode_t retcode = RTIDNP3_RETCODE_ERROR;
	struct DDS_DynamicData **output = (struct DDS_DynamicData**) sample_list;
	int i = 0;


	printf("[DNP3StreamWriter][%s]--[writing %d samples]\n",writer->name, count);


	for (i = 0; i < count; ++i) {
		struct DDS_DynamicData *sample = output[i];

		retcode = APDUConverter_convertToDNP3_simple(writer->converter, sample,
				&apdu);

		if (retcode == RTIDNP3_RETCODE_OK) {

			RTIDNP3_MessageBus_send(writer->msgBus, &apdu);

		}

		RTIDNP3_APDU_finalize(&apdu);

		if (retcode != RTIDNP3_RETCODE_OK) {
			RTI_RoutingServiceEnvironment_set_error(env,
					"Cannot convert APDU from DDS to DNP3");

			i--;

			break;
		}

	}

	return i;
}

void DNP3StreamWriter_update(
		RTI_RoutingServiceAdapterEntity entity,
		const struct RTI_RoutingServiceProperties *properties,
		RTI_RoutingServiceEnvironment *env) {

}

DNP3Writer* DNP3Writer_new(
		char *name,
		RTIDNP3_MessageBus *msgBus) {
	DNP3Writer* writer = NULL;

	writer = (DNP3Writer*) malloc(sizeof(DNP3Writer));

	writer->name = strdup(name);
	writer->msgBus = msgBus;
	writer->converter = APDUConverter_new();

	return writer;
}

void DNP3Writer_delete(
		DNP3Writer *writer) {

	APDUConverter_delete(writer->converter);
	free(writer);

}
