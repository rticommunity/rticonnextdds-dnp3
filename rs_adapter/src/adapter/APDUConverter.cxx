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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include "APDUConverter.h"

#ifdef __cplusplus
}
#endif

#include <DNP3/APDU.h>
#include <DNP3/ObjectReadIterator.h>

APDUConverter* APDUConverter_new() {
	APDUConverter *converter = NULL;
	struct DDS_DynamicDataTypeProperty_t typeProps =
			DDS_DYNAMIC_DATA_TYPE_PROPERTY_DEFAULT;
	DDS_TypeCode *typeCode = NULL;

	converter = (APDUConverter*) malloc(sizeof(APDUConverter));

	typeCode = com_rti_dnp3_SimpleAPDU_get_typecode();

	converter->simpleAPDUSupport = DDS_DynamicDataTypeSupport_new(typeCode,
			&typeProps);

	converter->simpleAPDU = DDS_DynamicDataTypeSupport_create_data(
			converter->simpleAPDUSupport);

	return converter;
}

void APDUConverter_delete(APDUConverter* converter) {
	DDS_DynamicDataTypeSupport_delete_data(converter->simpleAPDUSupport,
			converter->simpleAPDU);
	DDS_DynamicDataTypeSupport_delete(converter->simpleAPDUSupport);
	free(converter);
}
#if 0
com_rti_dnp3_FunctionCode APDUConverter_convertToDDS_FC(
		enum apl::dnp::FunctionCodes fc) {
	com_rti_dnp3_FunctionCode result = APDU_FC_UNKNOWN;

	switch (fc) {
	case apl::dnp::FC_CONFIRM:
		result = APDU_FC_CONFIRM;
		break;
	case apl::dnp::FC_READ:
		result = APDU_FC_READ;
		break;
	case apl::dnp::FC_WRITE:
		result = APDU_FC_WRITE;
		break;
	case apl::dnp::FC_SELECT:
		result = APDU_FC_SELECT;
		break;
	case apl::dnp::FC_OPERATE:
		result = APDU_FC_OPERATE;
		break;
	case apl::dnp::FC_DIRECT_OPERATE:
		result = APDU_FC_OPERATE_DIR;
		break;
	case apl::dnp::FC_DIRECT_OPERATE_NO_ACK:
		result = APDU_FC_OPERATE_DIR_NOACK;
		break;
	case apl::dnp::FC_FREEZE:
		result = APDU_FC_FREEZE_IMM;
		break;
	case apl::dnp::FC_FREEZE_NO_ACK:
		result = APDU_FC_FREEZE_IMM_NOACK;
		break;
	case apl::dnp::FC_FREEZE_CLEAR:
		result = APDU_FC_FREEZE_CLEAR;
		break;
	case apl::dnp::FC_FREEZE_CLEAR_NO_ACK:
		result = APDU_FC_FREEZE_CLEAR_NOACK;
		break;
	case apl::dnp::FC_FREEZE_AT_TIME:
		result = APDU_FC_FREEZE_W_TIME;
		break;
	case apl::dnp::FC_FREEZE_AT_TIME_NO_ACK:
		result = APDU_FC_FREEZE_W_TIME_NOACK;
		break;
	case apl::dnp::FC_COLD_RESTART:
		result = APDU_FC_RESTART_COLD;
		break;
	case apl::dnp::FC_WARM_RESTART:
		result = APDU_FC_RESTART_WARM;
		break;
	case apl::dnp::FC_INITIALIZE_DATA:
		result = APDU_FC_INIT_DEFAULTS;
		break;
	case apl::dnp::FC_INITIALIZE_APPLICATION:
		result = APDU_FC_APP_INIT;
		break;
	case apl::dnp::FC_START_APPLICATION:
		result = APDU_FC_APP_START;
		break;
	case apl::dnp::FC_STOP_APPLICATION:
		result = APDU_FC_APP_STOP;
		break;
	case apl::dnp::FC_SAVE_CONFIGURATION:
		result = APDU_FC_SAVE_CONFIG;
		break;
	case apl::dnp::FC_ENABLE_UNSOLICITED:
		result = APDU_FC_ENABLE_UNSOL;
		break;
	case apl::dnp::FC_DISABLE_UNSOLICITED:
		result = APDU_FC_DISABLE_UNSOL;
		break;
	case apl::dnp::FC_ASSIGN_CLASS:
		result = APDU_FC_ASSIGN_CLASS;
		break;
	case apl::dnp::FC_DELAY_MEASURE:
		result = APDU_FC_MEASURE_DELAY;
		break;
	case apl::dnp::FC_RECORD_TIME:
		result = APDU_FC_RECORD_TIME;
		break;
	case apl::dnp::FC_FILE_OPEN:
		result = APDU_FC_FILE_OPEN;
		break;
	case apl::dnp::FC_FILE_CLOSE:
		result = APDU_FC_FILE_CLOSE;
		break;
	case apl::dnp::FC_FILE_DELETE:
		result = APDU_FC_FILE_DELETE;
		break;
	case apl::dnp::FC_FILE_INFO:
		result = APDU_FC_FILE_INFO;
		break;
	case apl::dnp::FC_FILE_AUTHENTICATE:
		result = APDU_FC_AUTH;
		break;
	case apl::dnp::FC_FILE_ABORT:
		result = APDU_FC_ABORT;
		break;
	case apl::dnp::FC_PROPRIETARY_VTO_TRANSFER:
		result = APDU_FC_UNKNOWN;
		break;
	case apl::dnp::FC_RESPONSE:
		result = APDU_FC_RESPONSE;
		break;
	case apl::dnp::FC_UNSOLICITED_RESPONSE:
		result = APDU_FC_UNSOL;
		break;
	case apl::dnp::FC_UNKNOWN:
		result = APDU_FC_UNKNOWN;
		break;
	default:
		result = APDU_FC_UNKNOWN;
		break;
	}

	return result;
}

com_rti_dnp3_QualifierCode APDUConverter_convertToDDS_QC(
		enum apl::dnp::QualifierCode qc) {
	com_rti_dnp3_QualifierCode result = APDU_QC_UNDEFINED;

	switch (qc) {
	case apl::dnp::QC_1B_START_STOP:
		result = APDU_QC_1B_START_STOP;
		break;
	case apl::dnp::QC_2B_START_STOP:
		result = APDU_QC_2B_START_STOP;
		break;
	case apl::dnp::QC_4B_START_STOP:
		result = APDU_QC_4B_START_STOP;
		break;
	case apl::dnp::QC_ALL_OBJ:
		result = APDU_QC_ALL_OBJ;
		break;
	case apl::dnp::QC_1B_CNT:
		result = APDU_QC_1B_CNT;
		break;
	case apl::dnp::QC_2B_CNT:
		result = APDU_QC_2B_CNT;
		break;
	case apl::dnp::QC_4B_CNT:
		result = APDU_QC_4B_CNT;
		break;
	case apl::dnp::QC_1B_CNT_1B_INDEX:
		result = APDU_QC_1B_CNT_1B_INDEX;
		break;
	case apl::dnp::QC_2B_CNT_2B_INDEX:
		result = APDU_QC_2B_CNT_2B_INDEX;
		break;
	case apl::dnp::QC_4B_CNT_4B_INDEX:
		result = APDU_QC_4B_CNT_4B_INDEX;
		break;
	case apl::dnp::QC_1B_VCNT_1B_SIZE:
		result = APDU_QC_1B_VCNT_1B_SIZE;
		break;
	case apl::dnp::QC_1B_VCNT_2B_SIZE:
		result = APDU_QC_1B_VCNT_2B_SIZE;
		break;
	case apl::dnp::QC_1B_VCNT_4B_SIZE:
		result = APDU_QC_1B_VCNT_4B_SIZE;
		break;
	case apl::dnp::QC_UNDEFINED:
		result = APDU_QC_UNDEFINED;
		break;
	default:
		result = APDU_QC_UNDEFINED;
		break;
	}

	return result;
}


RTIDNP3_ReturnCode_t APDUConverter_convertToDDS_advanced_writeObjects(
		APDUConverter *converter, apl::dnp::APDU *apdu,
		struct DDS_DynamicData *dyndata) {
	const char *METHOD_NAME = "APDUConverter_convertToDDS_advanced_writeObjects";
	RTIDNP3_ReturnCode_t result = RTIDNP3_RETCODE_ERROR;
	apl::dnp::HeaderReadIterator header_iter = apdu->BeginRead();
	struct com_rti_dnp3_ASDUEntrySeq asdu;
	struct com_rti_dnp3_ASDUEntry *asdu_entries, *asdu_entry;
	bool unknown = false;
	int j = 0, i = 0;

	if (header_iter.Count() > 0) {
		asdu_entries = (struct com_rti_dnp3_ASDUEntry*) calloc(
				header_iter.Count(), sizeof(struct com_rti_dnp3_ASDUEntry));
	} else {
		return RTIDNP3_RETCODE_ERROR;
	}

	for (; !header_iter.IsEnd(); header_iter++) {
		struct com_rti_dnp3_DNP3Object *dnp3_objects = NULL;
		apl::dnp::ObjectReadIterator obj_iter = header_iter.BeginRead();
		apl::dnp::HeaderInfo header_info = header_iter.info();

		/*asdu_entry = (struct com_rti_dnp3_ASDUEntry*) malloc(
				sizeof(struct com_rti_dnp3_ASDUEntry));*/
		asdu_entry = &(asdu_entries[i]);
		i++;
		com_rti_dnp3_ASDUEntry_initialize(asdu_entry);

		asdu_entry->header.group = header_info.GetGroup();
		asdu_entry->header.variation = header_info.GetVariation();
		asdu_entry->header.q_code = APDUConverter_convertToDDS_QC(
				header_info.GetQualifier());
		asdu_entry->header.i_size = header_info.GetPrefixSize();

		if (obj_iter.Count() > 0) {
			dnp3_objects = (struct com_rti_dnp3_DNP3Object**) calloc(
					obj_iter.Count(), sizeof(struct com_rti_dnp3_DNP3Object));
		} else {
			continue;
		}

		for (; !obj_iter.IsEnd(); obj_iter++) {
			struct com_rti_dnp3_DNP3Object *dnp3Obj = NULL;
			apl::dnp::HeaderInfo obj_header = obj_iter.Header();
			int group = 0, var = 0;

			/*dnp3Obj = (struct com_rti_dnp3_DNP3Object*) malloc(
			 sizeof(struct com_rti_dnp3_DNP3Object));*/
			dnp3Obj = &(dnp3_objects[i]);

			group = obj_header.GetGroup();
			switch (group) {
			case 1: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group1Var1 *obj =
							static_cast<const apl::dnp::Group1Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_1_1;

					/* HOW TO READ DATA IN OBJECT?! */
					//dnp3Obj->_u.obj_1_1 = obj11->Read();
					break;
				}
				case 2: {
					const apl::dnp::Group1Var2 *obj =
							static_cast<const apl::dnp::Group1Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_1_2;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 2: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group2Var1 *obj =
							static_cast<const apl::dnp::Group2Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_2_1;

					break;
				}
				case 2: {
					const apl::dnp::Group2Var2 *obj =
							static_cast<const apl::dnp::Group2Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_2_2;

					break;
				}
				case 3: {
					const apl::dnp::Group2Var3 *obj =
							static_cast<const apl::dnp::Group2Var3*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_2_3;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 10: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group10Var1 *obj =
							static_cast<const apl::dnp::Group10Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_10_1;

					break;
				}
				case 2: {
					const apl::dnp::Group10Var2 *obj =
							static_cast<const apl::dnp::Group10Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_10_2;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 12: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group12Var1 *obj =
							static_cast<const apl::dnp::Group12Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_12_1;

					break;
				}
				case 2: {
					const apl::dnp::Group12Var2 *obj =
							static_cast<const apl::dnp::Group12Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_12_2;

					break;
				}
				case 3: {
					const apl::dnp::Group12Var3 *obj =
							static_cast<const apl::dnp::Group12Var3*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_12_3;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 20: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group20Var1 *obj =
							static_cast<const apl::dnp::Group20Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_20_1;

					break;
				}
				case 2: {
					const apl::dnp::Group20Var2 *obj =
							static_cast<const apl::dnp::Group20Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_20_2;

					break;
				}
				case 3: {
					const apl::dnp::Group20Var3 *obj =
							static_cast<const apl::dnp::Group20Var3*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_20_3;

					break;
				}
				case 4: {
					const apl::dnp::Group20Var4 *obj =
							static_cast<const apl::dnp::Group20Var4*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_20_4;

					break;
				}
				case 5: {
					const apl::dnp::Group20Var5 *obj =
							static_cast<const apl::dnp::Group20Var5*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_20_5;

					break;
				}
				case 6: {
					const apl::dnp::Group20Var6 *obj =
							static_cast<const apl::dnp::Group20Var6*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_20_6;

					break;
				}
				case 7: {
					const apl::dnp::Group20Var7 *obj =
							static_cast<const apl::dnp::Group20Var7*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_20_7;

					break;
				}
				case 8: {
					const apl::dnp::Group20Var8 *obj =
							static_cast<const apl::dnp::Group20Var8*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_20_8;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 21: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group21Var1 *obj =
							static_cast<const apl::dnp::Group21Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_21_1;

					break;
				}
				case 2: {
					const apl::dnp::Group21Var2 *obj =
							static_cast<const apl::dnp::Group21Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_21_2;

					break;
				}
				case 3: {
					const apl::dnp::Group21Var3 *obj =
							static_cast<const apl::dnp::Group21Var3*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_21_3;

					break;
				}
				case 4: {
					const apl::dnp::Group21Var4 *obj =
							static_cast<const apl::dnp::Group21Var4*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_21_4;

					break;
				}
				case 5: {
					const apl::dnp::Group21Var5 *obj =
							static_cast<const apl::dnp::Group21Var5*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_21_5;

					break;
				}
				case 6: {
					const apl::dnp::Group21Var6 *obj =
							static_cast<const apl::dnp::Group21Var6*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_21_6;

					break;
				}
				case 7: {
					const apl::dnp::Group21Var7 *obj =
							static_cast<const apl::dnp::Group21Var7*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_21_7;

					break;
				}
				case 8: {
					const apl::dnp::Group21Var8 *obj =
							static_cast<const apl::dnp::Group21Var8*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_21_8;

					break;
				}
				case 9: {
					const apl::dnp::Group21Var9 *obj =
							static_cast<const apl::dnp::Group21Var9*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_21_9;

					break;
				}
				case 10: {
					const apl::dnp::Group21Var10 *obj =
							static_cast<const apl::dnp::Group21Var10*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_21_10;

					break;
				}
				case 11: {
					const apl::dnp::Group21Var11 *obj =
							static_cast<const apl::dnp::Group21Var11*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_21_11;

					break;
				}
				case 12: {
					const apl::dnp::Group21Var12 *obj =
							static_cast<const apl::dnp::Group21Var12*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_21_12;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 22: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group22Var1 *obj =
							static_cast<const apl::dnp::Group22Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_22_1;

					break;
				}
				case 2: {
					const apl::dnp::Group22Var2 *obj =
							static_cast<const apl::dnp::Group22Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_22_2;

					break;
				}
				case 3: {
					const apl::dnp::Group22Var3 *obj =
							static_cast<const apl::dnp::Group22Var3*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_22_3;

					break;
				}
				case 4: {
					const apl::dnp::Group22Var4 *obj =
							static_cast<const apl::dnp::Group22Var4*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_22_4;

					break;
				}
				case 5: {
					const apl::dnp::Group22Var5 *obj =
							static_cast<const apl::dnp::Group22Var5*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_22_5;

					break;
				}
				case 6: {
					const apl::dnp::Group22Var6 *obj =
							static_cast<const apl::dnp::Group22Var6*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_22_6;

					break;
				}
				case 7: {
					const apl::dnp::Group22Var7 *obj =
							static_cast<const apl::dnp::Group22Var7*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_22_7;

					break;
				}
				case 8: {
					const apl::dnp::Group22Var8 *obj =
							static_cast<const apl::dnp::Group22Var8*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_22_8;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 23: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group23Var1 *obj =
							static_cast<const apl::dnp::Group23Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_23_1;

					break;
				}
				case 2: {
					const apl::dnp::Group23Var2 *obj =
							static_cast<const apl::dnp::Group23Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_23_2;

					break;
				}
				case 3: {
					const apl::dnp::Group23Var3 *obj =
							static_cast<const apl::dnp::Group23Var3*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_23_3;

					break;
				}
				case 4: {
					const apl::dnp::Group23Var4 *obj =
							static_cast<const apl::dnp::Group23Var4*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_23_4;

					break;
				}
				case 5: {
					const apl::dnp::Group23Var5 *obj =
							static_cast<const apl::dnp::Group23Var5*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_23_5;

					break;
				}
				case 6: {
					const apl::dnp::Group23Var6 *obj =
							static_cast<const apl::dnp::Group23Var6*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_23_6;

					break;
				}
				case 7: {
					const apl::dnp::Group23Var7 *obj =
							static_cast<const apl::dnp::Group23Var7*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_23_7;

					break;
				}
				case 8: {
					const apl::dnp::Group23Var8 *obj =
							static_cast<const apl::dnp::Group23Var8*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_23_8;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 30: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group30Var1 *obj =
							static_cast<const apl::dnp::Group30Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_30_1;

					break;
				}
				case 2: {
					const apl::dnp::Group30Var2 *obj =
							static_cast<const apl::dnp::Group30Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_30_2;

					break;
				}
				case 3: {
					const apl::dnp::Group30Var3 *obj =
							static_cast<const apl::dnp::Group30Var3*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_30_3;

					break;
				}
				case 4: {
					const apl::dnp::Group30Var4 *obj =
							static_cast<const apl::dnp::Group30Var4*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_30_4;

					break;
				}
				case 5: {
					const apl::dnp::Group30Var5 *obj =
							static_cast<const apl::dnp::Group30Var5*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_30_5;

					break;
				}
				case 6: {
					const apl::dnp::Group30Var6 *obj =
							static_cast<const apl::dnp::Group30Var6*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_30_6;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 31: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group31Var1 *obj =
							static_cast<const apl::dnp::Group31Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_31_1;

					break;
				}
				case 2: {
					const apl::dnp::Group31Var2 *obj =
							static_cast<const apl::dnp::Group31Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_31_2;

					break;
				}
				case 3: {
					const apl::dnp::Group31Var3 *obj =
							static_cast<const apl::dnp::Group31Var3*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_31_3;

					break;
				}
				case 4: {
					const apl::dnp::Group31Var4 *obj =
							static_cast<const apl::dnp::Group31Var4*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_31_4;

					break;
				}
				case 5: {
					const apl::dnp::Group31Var5 *obj =
							static_cast<const apl::dnp::Group31Var5*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_31_5;

					break;
				}
				case 6: {
					const apl::dnp::Group31Var6 *obj =
							static_cast<const apl::dnp::Group31Var6*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_31_6;

					break;
				}
				case 7: {
					const apl::dnp::Group31Var7 *obj =
							static_cast<const apl::dnp::Group31Var7*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_31_7;

					break;
				}
				case 8: {
					const apl::dnp::Group31Var8 *obj =
							static_cast<const apl::dnp::Group31Var8*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_31_8;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 32: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group32Var1 *obj =
							static_cast<const apl::dnp::Group32Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_32_1;

					break;
				}
				case 2: {
					const apl::dnp::Group32Var2 *obj =
							static_cast<const apl::dnp::Group32Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_32_2;

					break;
				}
				case 3: {
					const apl::dnp::Group32Var3 *obj =
							static_cast<const apl::dnp::Group32Var3*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_32_3;

					break;
				}
				case 4: {
					const apl::dnp::Group32Var4 *obj =
							static_cast<const apl::dnp::Group32Var4*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_32_4;

					break;
				}
				case 5: {
					const apl::dnp::Group32Var5 *obj =
							static_cast<const apl::dnp::Group32Var5*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_32_5;

					break;
				}
				case 6: {
					const apl::dnp::Group32Var6 *obj =
							static_cast<const apl::dnp::Group32Var6*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_32_6;

					break;
				}
				case 7: {
					const apl::dnp::Group32Var7 *obj =
							static_cast<const apl::dnp::Group32Var7*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_32_7;

					break;
				}
				case 8: {
					const apl::dnp::Group32Var8 *obj =
							static_cast<const apl::dnp::Group32Var8*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_32_8;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 33: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group33Var1 *obj =
							static_cast<const apl::dnp::Group33Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_33_1;

					break;
				}
				case 2: {
					const apl::dnp::Group33Var2 *obj =
							static_cast<const apl::dnp::Group33Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_33_2;

					break;
				}
				case 3: {
					const apl::dnp::Group33Var3 *obj =
							static_cast<const apl::dnp::Group33Var3*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_33_3;

					break;
				}
				case 4: {
					const apl::dnp::Group33Var4 *obj =
							static_cast<const apl::dnp::Group33Var4*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_33_4;

					break;
				}
				case 5: {
					const apl::dnp::Group33Var5 *obj =
							static_cast<const apl::dnp::Group33Var5*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_33_5;

					break;
				}
				case 6: {
					const apl::dnp::Group33Var6 *obj =
							static_cast<const apl::dnp::Group33Var6*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_33_6;

					break;
				}
				case 7: {
					const apl::dnp::Group33Var7 *obj =
							static_cast<const apl::dnp::Group33Var7*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_33_7;

					break;
				}
				case 8: {
					const apl::dnp::Group33Var8 *obj =
							static_cast<const apl::dnp::Group33Var8*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_33_8;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 34: {
				var = obj_header.GetVariation();
				switch (var) {
				default:
					printf(
							"%s - unsupported object: Group %d Var %d (unsupported by OpenDNP3)\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 40: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group40Var1 *obj =
							static_cast<const apl::dnp::Group40Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_40_1;

					break;
				}
				case 2: {
					const apl::dnp::Group40Var2 *obj =
							static_cast<const apl::dnp::Group40Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_40_2;

					break;
				}
				case 3: {
					const apl::dnp::Group40Var3 *obj =
							static_cast<const apl::dnp::Group40Var3*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_40_3;

					break;
				}
				case 4: {
					const apl::dnp::Group40Var4 *obj =
							static_cast<const apl::dnp::Group40Var4*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_40_4;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 41: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group41Var1 *obj =
							static_cast<const apl::dnp::Group41Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_41_1;

					break;
				}
				case 2: {
					const apl::dnp::Group41Var2 *obj =
							static_cast<const apl::dnp::Group41Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_41_2;

					break;
				}
				case 3: {
					const apl::dnp::Group41Var3 *obj =
							static_cast<const apl::dnp::Group41Var3*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_41_3;

					break;
				}
				case 4: {
					const apl::dnp::Group41Var4 *obj =
							static_cast<const apl::dnp::Group41Var4*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_41_4;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 50: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group50Var1 *obj =
							static_cast<const apl::dnp::Group50Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_50_1;

					break;
				}
				case 2: {
					const apl::dnp::Group50Var2 *obj =
							static_cast<const apl::dnp::Group50Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_50_2;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 51: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group51Var1 *obj =
							static_cast<const apl::dnp::Group51Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_51_1;

					break;
				}
				case 2: {
					const apl::dnp::Group51Var2 *obj =
							static_cast<const apl::dnp::Group51Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_51_2;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 52: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group52Var1 *obj =
							static_cast<const apl::dnp::Group52Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_52_1;

					break;
				}
				case 2: {
					const apl::dnp::Group52Var2 *obj =
							static_cast<const apl::dnp::Group52Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_52_2;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			case 60: {
				var = obj_header.GetVariation();
				switch (var) {
				case 1: {
					const apl::dnp::Group60Var1 *obj =
							static_cast<const apl::dnp::Group60Var1*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_60_1;

					break;
				}
				case 2: {
					const apl::dnp::Group60Var2 *obj =
							static_cast<const apl::dnp::Group60Var2*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_60_2;

					break;
				}
				case 3: {
					const apl::dnp::Group60Var3 *obj =
							static_cast<const apl::dnp::Group60Var3*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_60_3;

					break;
				}
				case 4: {
					const apl::dnp::Group60Var4 *obj =
							static_cast<const apl::dnp::Group60Var4*>(obj_header.GetBaseObject());
					dnp3Obj->_d = DNP3_OBJ_60_4;

					break;
				}
				default:
					printf("%s - unsupported object: Group %d Var %d\n",
							METHOD_NAME, group, var);
					unknown = true;
					break;
				}
				break;
			}
			default:
				printf("%s - unsupported object: Group %d Var %d\n",
						METHOD_NAME, group, var);
				unknown = true;
				break;
			}

			if (unknown) {
				dnp3Obj->_d = DNP3_OBJ_UNKNOWN;
			} else {
				dnp3_objects[j] = dnp3Obj;
				j++;
			}

		}

		com_rti_dnp3_DNP3ObjectSeq_from_array(&(asdu_entry->data.objects), dnp3_objects,
				j + 1);
		j = 0;

	}

	com_rti_dnp3_ASDUEntrySeq_from_array(&asdu, asdu_entries, i+1);
	DDS_DynamicData_set_complex_member(dyndata, "asdu", DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED, &asdu);


	return result;
}

RTIDNP3_ReturnCode_t APDUConverter_convertToDDS_advanced(
		APDUConverter *converter, RTIDNP3_APDU *dnp3APDU,
		struct DDS_DynamicData *ddsAPDU) {
	RTIDNP3_ReturnCode_t result = RTIDNP3_RETCODE_ERROR;
	com_rti_dnp3_EndpointId srcId;
	com_rti_dnp3_EndpointId destId;
	com_rti_dnp3_APDUType type;
	com_rti_dnp3_APCI header;
	com_rti_dnp3_ASDUEntry asdu_entry;
	DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;
	apl::dnp::APDU *inputAPDU = NULL;
	apl::dnp::AppControlField ctrlField;
	enum apl::dnp::FunctionCodes fc;

	com_rti_dnp3_EndpointId_initialize(&srcId);
	com_rti_dnp3_EndpointId_initialize(&destId);

	srcId.address = dnp3APDU->info.src;
	srcId.is_master = dnp3APDU->info.master;
	destId.address = dnp3APDU->info.dest;
	destId.is_master = !srcId.is_master;

	DDS_DynamicData_clear_all_members(ddsAPDU);

	if (!APDUConverter_DDSHelpers_writeEndpointId_simple(converter, ddsAPDU,
			&srcId, "src")) {
		goto done;
	}

	if (!APDUConverter_DDSHelpers_writeEndpointId_simple(converter, ddsAPDU,
			&destId, "dest")) {
		goto done;
	}

	type = APDUConverter_DDSHelpers_convertAPDUTypeToDDS(dnp3APDU->info.type);
	retcode = DDS_DynamicData_set_long(ddsAPDU, "type",
			DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED, type);

	if (retcode != DDS_RETCODE_OK) {
		goto done;
	}

	printf(
			"[APDUConverter]--[converted APDU from DNP3 to DDS (complex)]-[src=%u; dest=%u; type=%d]\n",
			srcId.address, destId.address, type);

	inputAPDU = new apl::dnp::APDU(2048);
	inputAPDU->Write((uint8_t*) dnp3APDU->buffer, dnp3APDU->buffer_size);
	inputAPDU->Interpret();

	ctrlField = inputAPDU->GetControl();
	fc = inputAPDU->GetFunction();

	com_rti_dnp3_APCI_initialize(&header);
	header._d = type;
	switch (type) {
	case APDU_TYPE_REQUEST:
		header._u.request.AC.CON = ctrlField.CON;
		header._u.request.AC.FIN = ctrlField.FIN;
		header._u.request.AC.FIR = ctrlField.FIR;
		header._u.request.AC.SEQ = ctrlField.SEQ;
		header._u.request.FC = APDUConverter_convertToDDS_FC(fc);
		break;
	case APDU_TYPE_RESPONSE:
		header._u.response.AC.CON = ctrlField.CON;
		header._u.response.AC.FIN = ctrlField.FIN;
		header._u.response.AC.FIR = ctrlField.FIR;
		header._u.response.AC.SEQ = ctrlField.SEQ;
		header._u.request.FC = APDUConverter_convertToDDS_FC(fc);
		break;
	default:
		break;
	}

	result = APDUConverter_convertToDDS_advanced_writeObjects(converter,
			inputAPDU, ddsAPDU);

	done:

	com_rti_dnp3_EndpointId_finalize(&srcId);
	com_rti_dnp3_EndpointId_finalize(&destId);

	if (inputAPDU != NULL) {
		delete (inputAPDU);
	}

	return result;
}
#endif

RTIDNP3_ReturnCode_t APDUConverter_convertToDDS_simple(APDUConverter *converter,
		RTIDNP3_APDU *dnp3APDU, struct DDS_DynamicData *ddsAPDU) {
	com_rti_dnp3_EndpointId srcId;
	com_rti_dnp3_EndpointId destId;
	com_rti_dnp3_APDUType type;
	com_rti_dnp3_APDUBuffer bufferSeq;
	DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;

	com_rti_dnp3_EndpointId_initialize(&srcId);
	com_rti_dnp3_EndpointId_initialize(&destId);

	srcId.address = dnp3APDU->info.src;
	srcId.is_master = dnp3APDU->info.master;
	destId.address = dnp3APDU->info.dest;
	destId.is_master = !srcId.is_master;

	DDS_DynamicData_clear_all_members(ddsAPDU);

	if (!APDUConverter_DDSHelpers_writeEndpointId_simple(converter, ddsAPDU,
			&srcId, "src")) {
		goto error;
	}

	if (!APDUConverter_DDSHelpers_writeEndpointId_simple(converter, ddsAPDU,
			&destId, "dest")) {
		goto error;
	}

	retcode = DDS_DynamicData_set_ushort(ddsAPDU, "buffer_size",
			DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED, dnp3APDU->buffer_size);

	if (retcode != DDS_RETCODE_OK) {
		goto error;
	}

	DDS_OctetSeq_from_array((struct DDS_OctetSeq*) &bufferSeq,
			(const DDS_Octet*) dnp3APDU->buffer, dnp3APDU->buffer_size);

	retcode = DDS_DynamicData_set_octet_seq(ddsAPDU, "buffer",
			DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED, &bufferSeq);

	if (retcode != DDS_RETCODE_OK) {
		goto error;
	}

	type = APDUConverter_DDSHelpers_convertAPDUTypeToDDS(dnp3APDU->info.type);
	retcode = DDS_DynamicData_set_long(ddsAPDU, "type",
			DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED, type);

	if (retcode != DDS_RETCODE_OK) {
		goto error;
	}

	printf(
			"[APDUConverter]--[converted APDU from DNP3 to DDS]-[src=%u; dest=%u; type=%d]\n",
			srcId.address, destId.address, type);

	com_rti_dnp3_EndpointId_finalize(&srcId);
	com_rti_dnp3_EndpointId_finalize(&destId);

	return RTIDNP3_RETCODE_OK;

	error:

	com_rti_dnp3_EndpointId_finalize(&srcId);
	com_rti_dnp3_EndpointId_finalize(&destId);

	return RTIDNP3_RETCODE_ERROR;
}

RTIDNP3_ReturnCode_t APDUConverter_convertToDNP3_simple(
		APDUConverter *converter, struct DDS_DynamicData *ddsAPDU,
		RTIDNP3_APDU *dnp3APDU) {

	com_rti_dnp3_EndpointId srcId;
	com_rti_dnp3_EndpointId destId;
	com_rti_dnp3_APDUType type;
	DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;
	DDS_UnsignedShort buffer_size;
	com_rti_dnp3_APDUBuffer bufferSeq;
	DDS_Octet buffer[2048];
	DDS_UnsignedLong array_size;
	RTIDNP3_ReturnCode_t dnp3Retcode = RTIDNP3_RETCODE_ERROR;

	dnp3APDU->buffer = NULL;

	com_rti_dnp3_EndpointId_initialize(&srcId);
	com_rti_dnp3_EndpointId_initialize(&destId);

	if (!APDUConverter_DDSHelpers_readEndpointId_simple(converter, ddsAPDU,
			&srcId, "src")) {
		goto error;
	}

	if (!APDUConverter_DDSHelpers_readEndpointId_simple(converter, ddsAPDU,
			&destId, "dest")) {
		goto error;
	}

	retcode = DDS_DynamicData_get_ushort(ddsAPDU, &buffer_size, "buffer_size",
			DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED);

	if (retcode != DDS_RETCODE_OK) {
		goto error;
	}

	retcode = DDS_DynamicData_get_long(ddsAPDU, (DDS_Long*) &type, "type",
			DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED);

	if (retcode != DDS_RETCODE_OK) {
		goto error;
	}

	retcode = DDS_DynamicData_get_octet_seq(ddsAPDU, &bufferSeq, "buffer",
			DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED);

	if (retcode != DDS_RETCODE_OK) {
		goto error;
	}

	DDS_OctetSeq_to_array(&bufferSeq, buffer,
			DDS_OctetSeq_get_length(&bufferSeq));

	dnp3Retcode = RTIDNP3_APDU_init(dnp3APDU, srcId.address, srcId.is_master,
			destId.address,
			APDUConverter_DDSHelpers_convertAPDUTypeToDNP3(type), buffer_size);

	memcpy(dnp3APDU->buffer, buffer, buffer_size);

	if (dnp3Retcode != RTIDNP3_RETCODE_OK) {
		goto error;
	}

	memcpy(dnp3APDU->buffer, (const void*) buffer, buffer_size);

	printf(
			"[APDUConverter]--[converted APDU from DDS to DNP3]-[src=%u; dest=%u; type=%d]\n",
			srcId.address, destId.address, type);

	com_rti_dnp3_EndpointId_finalize(&srcId);
	com_rti_dnp3_EndpointId_finalize(&destId);

	return RTIDNP3_RETCODE_OK;

	error:

	com_rti_dnp3_EndpointId_finalize(&srcId);
	com_rti_dnp3_EndpointId_finalize(&destId);

	if (dnp3APDU->buffer != NULL) {
		free(dnp3APDU->buffer);
	}

	return RTIDNP3_RETCODE_ERROR;
}

int APDUConverter_DDSHelpers_writeEndpointId_simple(APDUConverter *converter,
		struct DDS_DynamicData *ddsAPDU, com_rti_dnp3_EndpointId *endpointId,
		const char *memberName) {
	DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;
	char bound = 0;

	retcode = DDS_DynamicData_bind_complex_member(ddsAPDU,
			converter->simpleAPDU, memberName,
			DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED);

	if (retcode != DDS_RETCODE_OK) {
		goto error;
	}

	bound = 1;

	retcode = DDS_DynamicData_set_short(converter->simpleAPDU, "address",
			DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED, endpointId->address);
	if (retcode != DDS_RETCODE_OK) {
		goto error;
	}

	retcode = DDS_DynamicData_set_boolean(converter->simpleAPDU, "is_master",
			DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED, endpointId->is_master);
	if (retcode != DDS_RETCODE_OK) {
		goto error;
	}

	DDS_DynamicData_unbind_complex_member(ddsAPDU, converter->simpleAPDU);

	return 1;

	error:

	if (bound) {
		DDS_DynamicData_unbind_complex_member(ddsAPDU, converter->simpleAPDU);
	}

	return 0;
}

int APDUConverter_DDSHelpers_readEndpointId_simple(APDUConverter *converter,
		struct DDS_DynamicData *ddsAPDU, com_rti_dnp3_EndpointId *endptId,
		const char *memberName) {
	DDS_ReturnCode_t retcode = DDS_RETCODE_ERROR;
	char bound = 0;

	retcode = DDS_DynamicData_bind_complex_member(ddsAPDU,
			converter->simpleAPDU, memberName,
			DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED);

	if (retcode != DDS_RETCODE_OK) {
		goto error;
	}

	bound = 1;

	retcode = DDS_DynamicData_get_short(converter->simpleAPDU,
			&endptId->address, "address",
			DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED);
	if (retcode != DDS_RETCODE_OK) {
		goto error;
	}

	retcode = DDS_DynamicData_get_boolean(converter->simpleAPDU,
			&endptId->is_master, "is_master",
			DDS_DYNAMIC_DATA_MEMBER_ID_UNSPECIFIED);
	if (retcode != DDS_RETCODE_OK) {
		goto error;
	}

	DDS_DynamicData_unbind_complex_member(ddsAPDU, converter->simpleAPDU);

	return 1;

	error:

	if (bound) {
		DDS_DynamicData_unbind_complex_member(ddsAPDU, converter->simpleAPDU);
	}

	return 0;

}

com_rti_dnp3_APDUType APDUConverter_DDSHelpers_convertAPDUTypeToDDS(
		RTIDNP3_APDUType_t type) {

	switch (type) {
	case RTIDNP3_APDU_REQUEST:
		return APDU_TYPE_REQUEST;
	case RTIDNP3_APDU_RESPONSE:
		return APDU_TYPE_RESPONSE;
		/*case APDU_TYPE_UNSOL_RESPONSE:
		 return RTIDNP3_APDU_UNSOL_RESPONSE;*/
	default:
		return (com_rti_dnp3_APDUType) -1;
	}
}

RTIDNP3_APDUType_t APDUConverter_DDSHelpers_convertAPDUTypeToDNP3(
		com_rti_dnp3_APDUType type) {

	switch (type) {
	case APDU_TYPE_REQUEST:
		return RTIDNP3_APDU_REQUEST;
	case APDU_TYPE_RESPONSE:
		return RTIDNP3_APDU_RESPONSE;
		/*case RTIDNP3_APDU_UNSOL_RESPONSE:
		 return APDU_TYPE_UNSOL_RESPONSE;*/
	default:
		return (RTIDNP3_APDUType_t) -1;
	}

}
