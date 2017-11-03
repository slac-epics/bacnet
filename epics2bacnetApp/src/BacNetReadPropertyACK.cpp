/*
 * BacNetReadPropertyACK.cpp
 *
 *  Created on: Sep 26, 2011
 *      Author: 8w4
 */

#include "BacNetReadPropertyACK.h"
#include "BacNetVariable.h"
#include "BacNetVariableDouble.h"
#include "common.h"

BacNetReadPropertyACK::BacNetReadPropertyACK() {

	this->object_id.type = 0;
	this->object_id.instance = 0;
	this->property_array_index = -1;
	this->property_id = (BACNET_PROPERTY_ID) 0;
}

BacNetReadPropertyACK::~BacNetReadPropertyACK() {
	// TODO Auto-generated destructor stub
}

BacNetBuffer *BacNetReadPropertyACK::decode_readProperty_ack(BacNetBuffer *buffer, BacNetVariable *var){

	uint8_t *pdu = buffer->get_currentPosition();


	uint8_t tagNumber = 0;
	uint8_t tagClass = 0;
	uint32_t tagLVT = 0;

	/** Clause 21 states that a "ReadProperty-ACK" will return an ordered sequence:
	 * [0] BACnetObjectIdentifier
	 * [1] BACnetPropertyIdentifier
	 * [2] Unsigned OPTIONAL --Array index if array is read
	 * [3] PropertyValue ABSTRACT-SYNTAX.&TYPE
	 * */

	/** Object ID...*/
	pdu = buffer->decode_bacnet_tag(pdu, &tagNumber, &tagClass, &tagLVT);
	if(tagNumber != 0 || tagClass != 1 || tagLVT != 4){
		if(verbosity_level_2(verbosity_level))
		printf("\nBad packet! Should be BACNET_APPLICATION_TAG_OBJECT_ID: %u\n", tagLVT);
		return NULL;
	}
	pdu = buffer->decode_bacnet_object_id(pdu, &this->object_id.type, &this->object_id.instance);
	if(verbosity_level_2(verbosity_level))
		printf("\nthis->object_id.type: %u, this->object_id.instance: %u\n", this->object_id.type, this->object_id.instance);

	/** Property ID...*/
	pdu = buffer->decode_bacnet_tag(pdu, &tagNumber, &tagClass, &tagLVT);
	if(tagNumber != 1 || tagClass != 1 || tagLVT <= 0){
		if(verbosity_level_2(verbosity_level))
			printf("\nBad packet! Should be BACNET_PROPERTY_ID: %u\n", tagLVT);
		return NULL;
	}
	pdu = buffer->decode_bacnet_unsigned_int(pdu, (uint32_t *)&this->property_id, tagLVT);
	if(verbosity_level_2(verbosity_level))
		printf("\nthis->property_id: %u\n", this->property_id);

	/** Property Array Index is optional. I did not have a device that implemented arrays
	 * so, unable to test it... If it is there I will skip it...*/
	pdu = buffer->decode_bacnet_tag(pdu, &tagNumber, &tagClass, &tagLVT);
	if(tagNumber == 2) {
		/** There is an array index...*/
		if(tagNumber != 2 || tagClass != 1 || tagLVT <= 0){
			if(verbosity_level_2(verbosity_level))
				printf("\nBad packet! Should be BACNET_PROPERTY_ID: %u\n", tagLVT);
			return NULL;
		}
		pdu = buffer->decode_bacnet_unsigned_int(pdu, (uint32_t *)&this->property_array_index, tagLVT);
		if(verbosity_level_2(verbosity_level))
			printf("\nthis->property_array_index: %u\n", this->property_array_index);
		pdu = buffer->decode_bacnet_tag(pdu, &tagNumber, &tagClass, &tagLVT);
	}

	if(tagNumber != 3 || tagClass != 1 || tagLVT != 6){
		if(verbosity_level_2(verbosity_level))
			printf("\nBad packet! Should be constructed data: tagNumber %u, tagClass %u, tagLVT %u\n", tagNumber, tagClass, tagLVT);
		return NULL;
	}

	/** It is constructed data. It could have many taged elements that follow.
	 * 0x06 represents "opening-tag" and 0x07 represents closing tag. 20.2.1.3.2*/
	BACNET_APPLICATION_DATA_VALUE val;
	uint32_t hack4ui32 = 0;

	pdu = buffer->decode_bacnet_tag(pdu, &tagNumber, &tagClass, &tagLVT);
	switch(tagNumber){
	case BACNET_APPLICATION_TAG_NULL:
		if(verbosity_level_3(verbosity_level))
			printf("Data type: BACNET_APPLICATION_TAG_NULL\n");
		var->setValue(val);
		var->setIsValid(true);

		if(verbosity_level_3(verbosity_level))
			printf("\nValue: %s\n", (tagLVT == 0) ? "null" : "Should be \"null\" But isn't...");
		break;
	case BACNET_APPLICATION_TAG_BOOLEAN:
		if(verbosity_level_3(verbosity_level))
			printf("Data type: BACNET_APPLICATION_TAG_BOOLEAN\n");
		val.type.Boolean = tagLVT ? true : false;
		var->setValue(val);
		var->setIsValid(true);

		if(verbosity_level_3(verbosity_level))
			printf("\nValue: %s\n", val.type.Boolean ? "true" : "false");
		break;
	case BACNET_APPLICATION_TAG_UNSIGNED_INT:
		if(verbosity_level_3(verbosity_level))
			printf("Data type: BACNET_APPLICATION_TAG_UNSIGNED_INT\n");

		//TODO Fix this hack for ui32...
		pdu = buffer->decode_bacnet_unsigned_int(pdu, &hack4ui32, tagLVT);

		val.type.Real = hack4ui32;
		var->setValue(val);
		var->setIsValid(true);

		if(verbosity_level_3(verbosity_level))
			printf("\nValue: %u\n", hack4ui32);
		break;
	case BACNET_APPLICATION_TAG_SIGNED_INT:
		if(verbosity_level_3(verbosity_level))
			printf("Data type: BACNET_APPLICATION_TAG_SIGNED_INT\n");
		break;
	case BACNET_APPLICATION_TAG_REAL:
		if(verbosity_level_3(verbosity_level))
			printf("Data type: BACNET_APPLICATION_TAG_REAL\n");

		pdu = buffer->decode_bacnet_real(pdu, &val.type.Real);
		var->setValue(val);
		var->setIsValid(true);

		if(verbosity_level_3(verbosity_level))
			printf("\nValue: %f\n", val.type.Real);
		break;
	case BACNET_APPLICATION_TAG_DOUBLE:
		if(verbosity_level_3(verbosity_level))
			printf("Data type: BACNET_APPLICATION_TAG_DOUBLE\n");
		break;
	case BACNET_APPLICATION_TAG_OCTET_STRING:
		if(verbosity_level_3(verbosity_level))
			printf("Data type: BACNET_APPLICATION_TAG_OCTET_STRING\n");
		break;
	case BACNET_APPLICATION_TAG_CHARACTER_STRING:
		if(verbosity_level_3(verbosity_level))
			printf("Data type: BACNET_APPLICATION_TAG_CHARACTER_STRING\n");
		pdu = buffer->decode_bacnet_character_string(pdu, val.type.Character, tagLVT);
		var->setValue(val);
		var->setIsValid(true);

		if(verbosity_level_3(verbosity_level))
			printf("\nValue: %s\n", val.type.Character);
		break;
	case BACNET_APPLICATION_TAG_BIT_STRING:
		if(verbosity_level_3(verbosity_level))
			printf("Data type: BACNET_APPLICATION_TAG_BIT_STRING\n");
		pdu = buffer->decode_bacnet_bit_string(pdu, &val.type.Unsigned_Int, tagLVT);

		var->setValue(val);
		var->setIsValid(true);

		if(verbosity_level_3(verbosity_level))
			printf("\nValue: %u\n", val.type.Unsigned_Int);
		break;
	case BACNET_APPLICATION_TAG_ENUMERATED:
		if(verbosity_level_3(verbosity_level))
			printf("Data type: BACNET_APPLICATION_TAG_ENUMERATED\n");

		pdu = buffer->decode_bacnet_enumeration(pdu, &val.type.Enumerated, tagLVT);

		var->setValue(val);
		var->setIsValid(true);

		if(verbosity_level_3(verbosity_level))
			printf("\nValue: %d\n", val.type.Enumerated);
		break;
	case BACNET_APPLICATION_TAG_DATE:
		if(verbosity_level_3(verbosity_level))
			printf("Data type: BACNET_APPLICATION_TAG_DATE\n");
		break;
	case BACNET_APPLICATION_TAG_TIME:
		if(verbosity_level_3(verbosity_level))
			printf("Data type: BACNET_APPLICATION_TAG_TIME\n");
		break;
	case BACNET_APPLICATION_TAG_OBJECT_ID:
		if(verbosity_level_3(verbosity_level))
			printf("Data type: BACNET_APPLICATION_TAG_OBJECT_ID\n");
		break;
	case BACNET_APPLICATION_TAG_RESERVE1:
	case BACNET_APPLICATION_TAG_RESERVE2:
	case BACNET_APPLICATION_TAG_RESERVE3:
	default:
		if(verbosity_level_3(verbosity_level))
			printf("Data type (%u): Unknown! Now what???\n", tagNumber);
		break;
	}

	return buffer;
}












