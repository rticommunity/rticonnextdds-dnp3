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

#ifndef RTIDNP3_APDU_H_
#define RTIDNP3_APDU_H_

#include <stddef.h>
#include "Utils.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum RTIDNP3_APDUType {
	RTIDNP3_APDU_REQUEST, RTIDNP3_APDU_RESPONSE, RTIDNP3_APDU_UNSOL_RESPONSE
} RTIDNP3_APDUType_t;

typedef struct RTIDNP3_AppCtrlField {

} RTIDNP3_AppCtrlField;

typedef struct RTIDNP3_IINField {

} RTIDNP3_IINField;

typedef struct RTIDNP3_APDUHeaderInfo {

} RTIDNP3_APDUHeaderInfo;

typedef struct RTIDNP3_ObjectHeader {

} RTIDNP3_ObjectHeader;

typedef struct RTIDNP3_Object {

} RTIDNP3_Object;

typedef struct RTIDNP3_APDUInfo {
	int src;
	int dest;
	unsigned char master;
	RTIDNP3_APDUType_t type;
} RTIDNP3_APDUInfo;

typedef struct RTIDNP3_APDU {
	RTIDNP3_APDUInfo info;
	void *buffer;
	size_t buffer_size;
} RTIDNP3_APDU;

RTIDNP3_ReturnCode_t RTIDNP3_APDU_init(
		RTIDNP3_APDU *apdu,
		int src, unsigned char is_master, int dest, RTIDNP3_APDUType_t type,
		size_t buffer_size);

void RTIDNP3_APDU_finalize(
		RTIDNP3_APDU* apdu);

#ifdef __cplusplus
}
#endif

#endif
