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

#include "APDU.h"
#include <string.h>
#include <stdlib.h>

extern "C" {

RTIDNP3_ReturnCode_t RTIDNP3_APDU_init(RTIDNP3_APDU *apdu,
		int src, unsigned char is_master, int dest, RTIDNP3_APDUType_t type,
		size_t buffer_size) {

	bzero(apdu, sizeof(RTIDNP3_APDU));

	apdu->info.src = src;
	apdu->info.master = is_master;
	apdu->info.dest = dest;
	apdu->info.type = type;

	apdu->buffer_size = buffer_size;
	apdu->buffer = malloc(buffer_size);

	return RTIDNP3_RETCODE_OK;

}

void RTIDNP3_APDU_finalize(RTIDNP3_APDU* apdu) {

	if (apdu->buffer != NULL) {
		free(apdu->buffer);
		apdu->buffer = NULL;
	}


}

}
