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

#include "DNP3StreamReader.h"

#include <string.h>

void DNP3StreamReader_read(
		RTI_RoutingServiceStreamReader stream_reader,
		RTI_RoutingServiceSample **sample_list,
		RTI_RoutingServiceSampleInfo **info_list, int *count,
		RTI_RoutingServiceEnvironment *env) {
	int i = 0;
	char removed = 0;
	int removedAPDUs = 0;
	int samplesSize = 0;
	int convertedSamples = 0;
	DNP3Reader *reader = (DNP3Reader*) stream_reader;
	struct DDS_DynamicData* sample = NULL;
	RTIDNP3_ReturnCode_t retcode = RTIDNP3_RETCODE_ERROR;
	struct DDS_DynamicData **output = NULL;

	samplesSize = reader->apdusData->currentSize;

	output = (struct DDS_DynamicData**) calloc(samplesSize,
			sizeof(struct DDS_DynamicData*));

	printf("[DNP3StreamReader][%s]--[reading %d APDU]\n", reader->name,
			samplesSize);

	for (i = 0; i < samplesSize; ++i) {
		RTIDNP3_APDU *apdu = reader->apdus[i];

		sample = DDS_DynamicDataTypeSupport_create_data(
				reader->converter->simpleAPDUSupport);

		retcode = APDUConverter_convertToDDS_simple(reader->converter, apdu,
				sample);

		if (retcode != RTIDNP3_RETCODE_OK) {

			/* do something in case of error... */
			printf("[DNP3StreamReader][%s]--[cannot convert APDU %d]\n",
					reader->name, i);

		} else {
			output[convertedSamples] = sample;
			convertedSamples++;
		}

	}

	while (--i >= 0) {
		RTIDNP3_APDU *apdu = reader->apdus[i];
		RTIDNP3_ARRAY_REMOVE_ELEMENT_BY_INDEX(reader->apdus, reader->apdusData,
				i, removed);
		RTIDNP3_APDU_finalize(apdu);
		free(apdu);

		removedAPDUs++;
	}

	*sample_list = (RTI_RoutingServiceSample*) output;
	*info_list = NULL;
	*count = convertedSamples;

	printf(
			"[DNP3StreamReader][%s]--[read %d samples]-[removed: %d]-[total APDUs: %lu]\n",
			reader->name,
			convertedSamples, removedAPDUs, reader->apdusData->currentSize);
}

void DNP3StreamReader_returnLoan(
		RTI_RoutingServiceStreamReader stream_reader,
		RTI_RoutingServiceSample *sample_list,
		RTI_RoutingServiceSampleInfo *info_list, int count,
		RTI_RoutingServiceEnvironment *env) {
	DNP3Reader *reader = (DNP3Reader*) stream_reader;
	struct DDS_DynamicData **output = (struct DDS_DynamicData**) sample_list;
	int i = 0;

	for (i = 0; i < count; ++i) {
		struct DDS_DynamicData *sample = output[i];

		DDS_DynamicDataTypeSupport_delete_data(
				reader->converter->simpleAPDUSupport, sample);
	}

	free(output);

}

void DNP3StreamReader_update(
		RTI_RoutingServiceAdapterEntity entity,
		const struct RTI_RoutingServiceProperties *properties,
		RTI_RoutingServiceEnvironment *env) {

}

int DNP3Reader_parseValue(
		const char *filter, int *i, char *valueOut) {
	int size = 0;
	int max = strlen(filter);

	if (*i >= max)
		return 0;

	valueOut[0] = '\0';
	while (filter[*i] != ';' && filter[*i] != '\0') {
		if (filter[*i] == ' ') {
			*i = (*i) + 1;
			continue;
		}
		valueOut[size++] = filter[*i];
		*i = (*i) + 1;
	}
	valueOut[size] = '\0';

	//printf("PARSED value \"%s\"\n", valueOut);
	return size;
}

int DNP3Reader_parseKey(
		const char *filter, int *i, char *keyOut) {
	int size = 0;
	int max = strlen(filter);

	if (*i >= max)
		return 0;

	keyOut[0] = '\0';
	while (filter[*i] != '=' && filter[*i] != '\0') {
		if (filter[*i] == ' ') {
			/*printf("%d blank\n", *i);*/
			*i = (*i) + 1;
			continue;
		}
		keyOut[size++] = filter[*i];
		*i = (*i) + 1;
		/*printf("%d %c\n", *i, keyOut[size -1]);*/
	}
	keyOut[size] = '\0';

	/*printf("PARSED key \"%s\"\n", keyOut);*/

	return size;
}

void DNP3Reader_parseFilter(
		DNP3Reader *reader) {

	int cursor = 0;
	char keyBuff[1024];
	char valueBuff[1024];

	reader->filter.src = -1;
	reader->filter.dest = -1;
	reader->filter.type = (com_rti_dnp3_APDUType) -1;

	while (DNP3Reader_parseKey(reader->filterStr, &cursor, keyBuff) > 0) {

		cursor++;

		if (DNP3Reader_parseValue(reader->filterStr, &cursor, valueBuff) > 0) {

			cursor++;

			if (strcmp(keyBuff, "src") == 0) {
				reader->filter.src = strtol(valueBuff, NULL, 0);
			}
			if (strcmp(keyBuff, "dest") == 0) {
				reader->filter.dest = strtol(valueBuff, NULL, 0);
			}
			if (strcmp(keyBuff, "type") == 0) {
				if (strcmp(valueBuff, "request") == 0) {
					reader->filter.type = APDU_TYPE_REQUEST;
				} else if (strcmp(valueBuff, "response") == 0) {
					reader->filter.type = APDU_TYPE_RESPONSE;
				}
				/*else if (strcmp(valueBuff, "unsol-response") == 0) {
					reader->filter.type = APDU_TYPE_UNSOL_RESPONSE;
				}*/
			}

		}

	}

}

DNP3Reader* DNP3Reader_new(
		const char *name,
		const char *filter,
		const struct RTI_RoutingServiceStreamReaderListener *listener) {
	DNP3Reader *reader = (DNP3Reader*) malloc(sizeof(DNP3Reader));
	reader->filterStr = strdup(filter);
	reader->streamReaderListener = listener;
	reader->listener.filterAPDU = DNP3Reader_filterAPDU;
	reader->listener.onAPDU = DNP3Reader_onAPDU;
	reader->listener.listenerData = reader;
	reader->name = strdup(name);

	RTIDNP3_ARRAY_ALLOCATE(reader->apdus, reader->apdusData, RTIDNP3_APDU*, 10,
			0, 10);

	reader->converter = APDUConverter_new();

	DNP3Reader_parseFilter(reader);

	return reader;
}

void DNP3Reader_delete(
		DNP3Reader* reader) {
	free(reader->filterStr);
	RTIDNP3_ARRAY_FREE_DEEP(reader->apdus, reader->apdusData, {
			RTIDNP3_APDU* apdu = (RTIDNP3_APDU*) _el;
			RTIDNP3_APDU_finalize(apdu);
			free(apdu);
			});

	APDUConverter_delete(reader->converter);
}

void DNP3Reader_onAPDU(
		RTIDNP3_MessageBusListener *listener,
		RTIDNP3_APDU *apdu,
		RTIDNP3_MessageBus *bus) {
	int added = 0;
	DNP3Reader *reader = (DNP3Reader*) listener;
	RTIDNP3_APDU *apduCopy = NULL;

	apduCopy = (RTIDNP3_APDU*) malloc(sizeof(RTIDNP3_APDU));

	RTIDNP3_APDU_init(apduCopy, apdu->info.src, apdu->info.master,
			apdu->info.dest, apdu->info.type, apdu->buffer_size);
	memcpy(apduCopy->buffer, apdu->buffer, apdu->buffer_size);

	RTIDNP3_ARRAY_ADD_ELEMENT_EXPAND(reader->apdus, reader->apdusData, apduCopy,
			added);

	printf(
			"[DNP3StreamReader][%s]--[APDU received: src=%d; dest=%d; type=%d][total APDUs: %lu]\n",
			reader->name, apdu->info.src, apdu->info.dest, apdu->info.type,
			reader->apdusData->currentSize);

	reader->streamReaderListener->on_data_available(reader,
			reader->streamReaderListener->listener_data);
}

RTIDNP3_FilterResult_t DNP3Reader_filterAPDU(
		RTIDNP3_MessageBusListener *listener,
		RTIDNP3_APDU *apdu,
		RTIDNP3_MessageBus *bus) {

	DNP3Reader *reader = (DNP3Reader*) listener;

	return DNP3Reader_filter(reader, apdu);
}

RTIDNP3_FilterResult_t DNP3Reader_filter(
		DNP3Reader *reader, RTIDNP3_APDU *apdu) {
	RTIDNP3_FilterResult_t result = RTIDNP3_FILTER_EXCLUDE;
	int filtered = 1;

	printf(
			"[DNP3StreamReader][%s]--[filtering]--[filter: src=%d; dest=%d; type=%d]-[APDU: src=%d; dest=%d; type=%d]\n",
			reader->name, reader->filter.src, reader->filter.dest,
			reader->filter.type,
			apdu->info.src, apdu->info.dest, apdu->info.type);

	if (reader->filter.src != -1) {
		filtered = filtered && (apdu->info.src == reader->filter.src);
	}

	if (!filtered) {
		result = RTIDNP3_FILTER_EXCLUDE;
		goto done;
	}

	if (reader->filter.dest != -1) {
		filtered = filtered && (apdu->info.dest == reader->filter.dest);
	}

	if (!filtered) {
		result = RTIDNP3_FILTER_EXCLUDE;
		goto done;
	}

	if (reader->filter.type != -1) {
		if (reader->filter.type == APDU_TYPE_REQUEST) {
			filtered = filtered && (apdu->info.type == RTIDNP3_APDU_REQUEST);
		} else if (reader->filter.type == APDU_TYPE_RESPONSE) {
			filtered = filtered && (apdu->info.type == RTIDNP3_APDU_RESPONSE);
		}
	}

	if (!filtered) {
		result = RTIDNP3_FILTER_EXCLUDE;
		goto done;
	}

	result = RTIDNP3_FILTER_PASS;

	done:

	printf("[DNP3StreamReader][%s]--[filter result]-[%s]\n",
			reader->name,
			(result == RTIDNP3_FILTER_PASS) ? "OK" :
			(result == RTIDNP3_FILTER_EXCLUDE) ? "EXCLUDE" : "BAD RESULT");

	return result;

}

void* DNP3Reader_NotificationThread(
		void *arg) {
	int i = 0;
	DNP3Reader *reader = (DNP3Reader*) arg;

	while (1) {

		if (reader->apdusData->currentSize > 0) {
			RTIDNP3_APDU *apdu = reader->apdus[i];
			reader->streamReaderListener->on_data_available(reader, NULL);
		}

		usleep(300);

	}

	return NULL;
}
