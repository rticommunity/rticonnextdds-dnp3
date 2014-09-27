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

#ifndef RTIDNP3_UTILS_H_
#define RTIDNP3_UTILS_H_

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RTIDNP3_ARRAY_DEFAULT_SIZE 10
#define RTIDNP3_ARRAY_DEFAULT_MAX 0
#define RTIDNP3_ARRAY_DEFAULT_INCR 10

typedef enum RTIDNP3_LogLevel {
	RTIDNP3_LOG_LEV_NONE = 0x00,
	RTIDNP3_LOG_LEV_EVENT = 0x01,
	RTIDNP3_LOG_LEV_ERROR = 0x02,
	RTIDNP3_LOG_LEV_WARNING = 0x04,
	RTIDNP3_LOG_LEV_INFO = 0x08,
	RTIDNP3_LOG_LEV_INTERPRET = 0x10,
	RTIDNP3_LOG_LEV_COMM = 0x20,
	RTIDNP3_LOG_LEV_DEBUG = 0x40
} RTIDNP3_LogLevel_t;

RTIDNP3_LogLevel_t RTIDNP3_LogLevel_fromString(
		const char* str);

typedef enum RTIDNP3_ReturnCode {
	RTIDNP3_RETCODE_OK, RTIDNP3_RETCODE_ERROR
} RTIDNP3_ReturnCode_t;

typedef struct RTIDNP3_ArrayData {
	/*e currently allocated size */
	unsigned long size;
	/*e maximum allocated size */
	unsigned long maxSize;
	/*e rate of size increment */
	unsigned long sizeIncrement;
	/*e currently used array's slots */
	unsigned long currentSize;
	/*e size of each array's element */
	size_t dataSize;
} RTIDNP3_ArrayData;

#define RTIDNP3_ARRAY_FREE(array,arrayData) \
	{ \
		if (array != NULL) {\
			free(array); \
			array = NULL; \
		} \
		if (arrayData != NULL) {\
			RTIDNP3_ArrayData_delete(arrayData); \
			arrayData= NULL; \
		}\
	}

#define RTIDNP3_ARRAY_FREE_DEEP(array,arrayData,elementDisposeBlock) \
	{ \
		if (array != NULL && arrayData != NULL) {\
			int _el_i = 0;\
			for (;_el_i < arrayData->currentSize; _el_i++){\
				void *_el = (void*) array[_el_i];\
				elementDisposeBlock;\
			}\
		} \
		RTIDNP3_ARRAY_FREE(array,arrayData);\
	}

#define RTIDNP3_ARRAY_ALLOCATE(array,arrayData,type,initial,max,increment) \
	{ \
		(arrayData) = RTIDNP3_ArrayData_new(sizeof(type),initial,max,increment);\
		if (arrayData != NULL) { \
			array = (type*) calloc((arrayData)->size,sizeof(type)); \
		} \
	}

#define RTIDNP3_ARRAY_ADD_ELEMENT(array,arrayData,element,ok)\
	{ \
		if ((arrayData)->currentSize + 1 <= (arrayData)->size) { \
			(arrayData)->currentSize++;\
			array[(arrayData)->currentSize - 1] = element; \
			ok = 1; \
		} else { \
			ok = 0; \
		} \
	}

#define RTIDNP3_ARRAY_ADD_ELEMENT_EXPAND(array,arrayData,element,ok)\
	{ \
		RTIDNP3_ARRAY_EXPAND_IF_NECESSARY(array, arrayData,ok);\
		if (ok) { \
			(arrayData)->currentSize++;\
			array[(arrayData)->currentSize - 1] = element; \
		} \
	}

#define RTIDNP3_ARRAY_EXPAND_IF_NECESSARY(array,arrayData,ok) \
	if ((arrayData)->currentSize == (arrayData)->size) { \
		if (RTIDNP3_Array_expand((void**) &(array),arrayData) == RTIDNP3_RETCODE_OK) { \
			ok = 1; \
		} else { \
			ok = 0; \
		} \
	} else { \
		ok = 1; \
	}

#define RTIDNP3_ARRAY_REMOVE_ELEMENT_BY_INDEX(array,arrayData,elementIndex,removed)\
	if (elementIndex < (arrayData)->currentSize) { \
		unsigned long rm_el_byi_i = elementIndex; \
		while (rm_el_byi_i < (arrayData)->currentSize - 1) { \
			array[rm_el_byi_i] = array[rm_el_byi_i+1]; \
			++rm_el_byi_i; \
		} \
		--(arrayData)->currentSize;\
		removed = 1;\
	} else { \
		removed = 0;\
	}

#define RTIDNP3_ARRAY_REMOVE_ELEMENT(array, arrayData, element, removed) \
		{ \
			removed = 0; \
			if ((arrayData)->currentSize > 0) { \
				unsigned long rm_el_i = 0; \
				for (;rm_el_i < (arrayData)->currentSize;++rm_el_i) { \
					if (array[rm_el_i] == element) { \
						RTIDNP3_ARRAY_REMOVE_ELEMENT_BY_INDEX(array,arrayData,rm_el_i,removed); \
						break; \
					} \
				}\
			} \
		}

#define RTIDNP3_ARRAY_CONTAINS_ELEMENT(array, arrayData, element, contained) \
		{\
			unsigned long elementIndex = 0;\
			RTIDNP3_ARRAY_GET_ELEMENT_INDEX(array,arrayData,element,elementIndex,contained);\
		}

#define RTIDNP3_ARRAY_GET_ELEMENT_INDEX(array,arrayData,element,elementIndex,found) \
	{\
		found = 0;\
		elementIndex = 0; \
		if ((arrayData)->currentSize > 0) { \
			for (; elementIndex < (arrayData)->currentSize;++elementIndex) { \
				if (array[elementIndex] == element) { \
					found = 1; \
					break; \
				} \
			} \
		} \
	}

#define RTIDNP3_ARRAY_GET(array,elementIndex) \
	(\
	(array)[(elementIndex)] \
	)

#define RTIDNP3_ARRAY_ITERATE(array, arrayData, codeBlock)\
	{ \
		if (array != NULL && arrayData != NULL) {\
			int _el_i = 0;\
			for (;_el_i < arrayData->currentSize; _el_i++){\
				void *_el = array[_el_i];\
				codeBlock;\
			}\
		} \
	}

RTIDNP3_ReturnCode_t RTIDNP3_Array_expand(
		void** array,
		struct RTIDNP3_ArrayData* array_data);

RTIDNP3_ArrayData* RTIDNP3_ArrayData_new(
		size_t dataSize,
		unsigned long initial,
		unsigned long max, unsigned long increment);

void RTIDNP3_ArrayData_delete(
		RTIDNP3_ArrayData* arrayData);

typedef struct RTIDNP3_Map {
	const char **keys;
	const char **values;
	int size;
	RTIDNP3_ArrayData *keysData;
	RTIDNP3_ArrayData *valuesData;
} RTIDNP3_Map;

RTIDNP3_Map* RTIDNP3_Map_new();

RTIDNP3_Map* RTIDNP3_Map_init(
		const char **keys, const char **values, int size);
int RTIDNP3_Map_add(
		RTIDNP3_Map *map,
		const char *key, const char *value);
const char* RTIDNP3_Map_get(
		RTIDNP3_Map *map, const char *key);

void RTIDNP3_Map_delete(RTIDNP3_Map *map);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_H_ */
