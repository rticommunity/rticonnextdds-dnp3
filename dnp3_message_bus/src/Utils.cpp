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
#include "Utils.h"

extern "C" {

RTIDNP3_Map* RTIDNP3_Map_new() {
	RTIDNP3_Map *map = NULL;

	map = (RTIDNP3_Map*) malloc(sizeof(RTIDNP3_Map));

	RTIDNP3_ARRAY_ALLOCATE(map->keys, map->keysData, const char*, 10, 0, 10);
	RTIDNP3_ARRAY_ALLOCATE(map->values, map->valuesData, const char*, 10, 0,
			10);

	map->size = 0;

	return map;
}

void RTIDNP3_Map_delete(
		RTIDNP3_Map *map) {

	RTIDNP3_ARRAY_FREE_DEEP(map->keys, map->keysData, {
	 free(_el);
	 });

	RTIDNP3_ARRAY_FREE_DEEP(map->values, map->valuesData, {
			free(_el);
			});

	free(map);

}

RTIDNP3_Map* RTIDNP3_Map_init(
		const char **keys, const char **values, int size) {

	RTIDNP3_Map *map = NULL;

	map = (RTIDNP3_Map*) malloc(sizeof(RTIDNP3_Map));

	map->keys = keys;
	map->values = values;
	map->size = size;

	return map;
}

const char* RTIDNP3_Map_get(
		RTIDNP3_Map *map, const char *key) {
	int i = 0;
	for (i = map->size - 1; i >= 0; i--) {
		const char *k = map->keys[i];
		if (strcmp(key, k) == 0) {
			return map->values[i];
		}
	}

	return NULL;
}

int RTIDNP3_Map_add(
		RTIDNP3_Map *map,
		const char *key,
		const char *value) {
	int added = 0;

	RTIDNP3_ARRAY_ADD_ELEMENT_EXPAND(map->keys, map->keysData, strdup(key),
			added);
	if (!added) {
		return added;
	}
	RTIDNP3_ARRAY_ADD_ELEMENT_EXPAND(map->values, map->valuesData,
			strdup(value),
			added);

	map->size = map->keysData->currentSize;

	return added;
}

RTIDNP3_ArrayData* RTIDNP3_ArrayData_new(
		size_t dataSize,
		unsigned long initial,
		unsigned long max, unsigned long increment) {
	RTIDNP3_ArrayData* data;

	data = (struct RTIDNP3_ArrayData*) malloc(sizeof(struct RTIDNP3_ArrayData));

	data->size = initial;
	data->maxSize = max;
	data->sizeIncrement = increment;
	data->currentSize = 0;
	data->dataSize = dataSize;
	return data;
}

void RTIDNP3_ArrayData_delete(
		RTIDNP3_ArrayData* data) {
	free(data);
}

RTIDNP3_ReturnCode_t RTIDNP3_Array_expand(
		void** array,
		RTIDNP3_ArrayData* arrayData) {
	size_t dataSize;
	unsigned long newSize;

	dataSize = arrayData->dataSize;

	newSize = arrayData->size + arrayData->sizeIncrement;

	if (arrayData->maxSize == 0 || newSize <= arrayData->maxSize) {
		*array = realloc(*array, dataSize * newSize);

		if (*array == NULL) {
			return RTIDNP3_RETCODE_ERROR;
		} else {
			arrayData->size = newSize;
			return RTIDNP3_RETCODE_OK;
		}
	} else {
		return RTIDNP3_RETCODE_ERROR;
	}

}

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? a : b)
#endif

RTIDNP3_LogLevel_t RTIDNP3_LogLevel_fromString(
		const char* str) {

	size_t len = 0;

	if (str == NULL) {
		return RTIDNP3_LOG_LEV_NONE;
	}

	len = strlen(str);

	if (strncasecmp(str, "EVENT", MIN(len,strlen("EVENT"))) == 0) {
		return RTIDNP3_LOG_LEV_EVENT;
	}

	if (strncasecmp(str, "ERROR", MIN(len,strlen("ERROR"))) == 0) {
		return RTIDNP3_LOG_LEV_ERROR;
	}

	if (strncasecmp(str, "WARNING", MIN(len,strlen("WARNING"))) == 0) {
		return RTIDNP3_LOG_LEV_WARNING;
	}

	if (strncasecmp(str, "INFO", MIN(len,strlen("INFO"))) == 0) {
		return RTIDNP3_LOG_LEV_INFO;
	}

	if (strncasecmp(str, "INTERPRET", MIN(len,strlen("INTERPRET"))) == 0) {
		return RTIDNP3_LOG_LEV_INTERPRET;
	}

	if (strncasecmp(str, "COMM", MIN(len,strlen("COMM"))) == 0) {
		return RTIDNP3_LOG_LEV_COMM;
	}

	if (strncasecmp(str, "DEBUG", MIN(len,strlen("DEBUG"))) == 0) {
		return RTIDNP3_LOG_LEV_DEBUG;
	}

	return RTIDNP3_LOG_LEV_NONE;
}

}
