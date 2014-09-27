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

#ifndef _APDUCONVERTER_H
#define _APDUCONVERTER_H

#ifdef __cplusplus
extern "C" {
#endif


#include <ndds/ndds_c.h>

#include <dnp3msgbus/Utils.h>
#include <dnp3msgbus/MessageBus.h>

#include "dds_c/SimpleAPDU.h"

typedef struct APDUConverter {
	struct DDS_DynamicDataTypeSupport *simpleAPDUSupport;
	struct DDS_DynamicData *simpleAPDU;
} APDUConverter;

APDUConverter* APDUConverter_new();

void APDUConverter_delete(
		APDUConverter* converter);

RTIDNP3_ReturnCode_t APDUConverter_convertToDDS_advanced(
		APDUConverter *converter,
		RTIDNP3_APDU *dnp3APDU,
		struct DDS_DynamicData *ddsAPDU);

RTIDNP3_ReturnCode_t APDUConverter_convertToDDS_simple(
		APDUConverter *converter,
		RTIDNP3_APDU *dnp3APDU,
		struct DDS_DynamicData *ddsAPDU);

RTIDNP3_ReturnCode_t APDUConverter_convertToDNP3_simple(
		APDUConverter *converter,
		struct DDS_DynamicData *ddsAPDU,
		RTIDNP3_APDU *dnp3APDU);

com_rti_dnp3_APDUType APDUConverter_DDSHelpers_convertAPDUTypeToDDS(
		RTIDNP3_APDUType_t type);

RTIDNP3_APDUType_t APDUConverter_DDSHelpers_convertAPDUTypeToDNP3(
		com_rti_dnp3_APDUType type);

int APDUConverter_DDSHelpers_writeEndpointId_simple(
		APDUConverter *converter,
		struct DDS_DynamicData *ddsAPDU,
		com_rti_dnp3_EndpointId *endpointId,
		const char *memberName);

int APDUConverter_DDSHelpers_readEndpointId_simple(
		APDUConverter *converter,
		struct DDS_DynamicData *ddsAPDU,
		com_rti_dnp3_EndpointId *endpointId,
		const char *memberName);

#ifdef __cplusplus
}
#endif


#endif
