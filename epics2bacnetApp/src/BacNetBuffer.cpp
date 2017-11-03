/*
 * BacNetBuffer.cpp
 *
 *  Created on: Aug 29, 2011
 *      Author: 8w4
 */

#include "BacNetBuffer.h"
#include "common.h"

BacNetBuffer::BacNetBuffer() {
	memset(this->buffer, 0, 2500);
	ptrBuff = buffer;

}

BacNetBuffer::~BacNetBuffer() {

}

uint8_t *BacNetBuffer::getPointerToBuffer(){
	return buffer;
}

size_t BacNetBuffer::getLength(uint8_t *pos){
	return pos - buffer;
}

size_t BacNetBuffer::getLength(){
	return this->length;
}

void BacNetBuffer::set_length(uint8_t *pos){
	this->length = pos - this->buffer;
}

void BacNetBuffer::set_currentPosition(uint8_t *ptr){
	this->currentPosition = ptr;
}
uint8_t *BacNetBuffer::get_currentPosition(){
	return this->currentPosition;
}

void BacNetBuffer::hexdump(const uint8_t *buffer, size_t length)
{
	size_t i;
	for (i=0; i<length; ++i)
	{
		printf(" %02X", *(buffer++));
		if (((i+1) % 16) == 0)
			printf("\n");
	}
	printf("\n");
}

uint8_t *BacNetBuffer::decode_var_length(uint8_t *destination, uint8_t *source, uint8_t length)
{
	int i;
	for(i=0; i<length; i++)
	{
		*(destination++) = *(source++);
	}
	return source;
}

int BacNetBuffer::get_bacnet_unsigned_length(uint32_t some_value) {

	int len = 0;

	if (some_value < 0x100) {
		len = 1;
	} else if (some_value < 0x10000) {
		len = 2;
	} else if (some_value < 0x1000000) {
		len = 3;
	} else {
		len = 4;
	}

	return len;
}

uint8_t *BacNetBuffer::encode_unsigned16(uint8_t *pdu, uint16_t value)
{
	*(pdu++) = (uint8_t) ((value & 0xff00) >> 8);
	*(pdu++) = (uint8_t) (value & 0x00ff);
	return pdu;
}

uint8_t *BacNetBuffer::decode_unsigned16(uint8_t *p, uint16_t *value)
{
    uint8_t high = *(p++);
    uint8_t low  = *(p++);
    *value = ((uint16_t)high) << 8 | low;
    return p;
}

uint8_t *BacNetBuffer::encode_unsigned24(uint8_t *apdu, uint32_t value)
{
    *(apdu++) = (uint8_t) ((value & 0xff0000) >> 16);
    *(apdu++) = (uint8_t) ((value & 0x00ff00) >> 8);
    *(apdu++) = (uint8_t) (value & 0x0000ff);

    return apdu;
}

uint8_t *BacNetBuffer::decode_unsigned24(uint8_t *apdu, uint32_t *value)
{
	*value = ((uint32_t) ((((uint32_t) *(apdu++)) << 16) & 0x00ff0000));
	*value |= (uint32_t) ((((uint32_t) *(apdu++)) << 8) & 0x0000ff00);
	*value |= ((uint32_t) (((uint32_t) *(apdu++)) & 0x000000ff));

    return apdu;
}

uint8_t *BacNetBuffer::encode_unsigned32(uint8_t *apdu, uint32_t value)
{
	*(apdu++) = (uint8_t) ((value & 0xff000000) >> 24);
	*(apdu++) = (uint8_t) ((value & 0x00ff0000) >> 16);
	*(apdu++) = (uint8_t) ((value & 0x0000ff00) >> 8);
	*(apdu++) = (uint8_t) (value & 0x000000ff);

    return apdu;
}

uint8_t *BacNetBuffer::decode_unsigned32(uint8_t *apdu, uint32_t *value)
{
    if (value) {
        *value = ((uint32_t) ((((uint32_t) *(apdu++)) << 24) & 0xff000000));
        *value |= ((uint32_t) ((((uint32_t) *(apdu++)) << 16) & 0x00ff0000));
        *value |= ((uint32_t) ((((uint32_t) *(apdu++)) << 8) & 0x0000ff00));
        *value |= ((uint32_t) (((uint32_t) *(apdu++)) & 0x000000ff));
    }

    return apdu;
}

uint8_t *BacNetBuffer::decode_bacnet_tag(uint8_t *pdu, uint8_t *tagNumber, uint8_t *tagClass, uint32_t *tagLenValType) {

	uint8_t tNumber = (*pdu & 0xF0) >> 4;
	*tagClass = (*pdu & 0x8) ? 1 : 0;
	uint32_t tLenValType = *pdu & 0x07;

	/** Application Tag?*/
	if(*tagClass == 0) {
		/** Then, tagNumber and tagClass is fixed so, only length/value/type can change...*/
		*tagNumber = (*pdu & 0xF0) >> 4;

		if(tLenValType == 0x05) {
			/** Flag for extension.*/
			++pdu;
			if(*pdu == 0xFF) {
				/** Must be 32-bit number...*/
				pdu = decode_unsigned32(pdu, tagLenValType);
			}
			else if(*pdu == 0xFE) {
				/** Must be 16-bit number...*/
				pdu = decode_unsigned16(pdu, (uint16_t *)tagLenValType);
			}
			else {
				/** Must be 8-bit number...*/
				*tagLenValType = *(pdu++);
			}

		}
		else {
			*tagLenValType = tLenValType;
			++pdu;
		}

	}
	/** Context Specific Tag?*/
	else {
		/** tagNumber can be extended and tagLenValType can be variable too. tagClass is fixed...*/
		if(tNumber == 0xF) {
			++pdu;
			*tagNumber = *(pdu++);
		}
		else {
			*tagNumber = (*pdu & 0xF0) >> 4;
		}

		if(tLenValType == 0x05) {
			/** Flag for extension.*/

			if(*pdu == 0xFF) {
				/** Must be 32-bit number...*/
				pdu = decode_unsigned32(pdu, tagLenValType);
			}
			else if(*pdu == 0xFE) {
				/** Must be 16-bit number...*/
				pdu = decode_unsigned16(pdu, (uint16_t *)tagLenValType);
			}
			else {
				/** Must be 8-bit number...*/
				*tagLenValType = *(pdu++);
			}

		}
		else {
			*tagLenValType = tLenValType;
			++pdu;
		}

	}


	return pdu;
}

uint8_t *BacNetBuffer::decode_bacnet_object_id(uint8_t *pdu, uint16_t *type, uint32_t *instanceNumber) {

	uint32_t temp32 = 0;

	pdu = decode_unsigned32(pdu, &temp32);
	*instanceNumber = (temp32 & 0x3FFFFF);
	temp32 >>= 22;
	*type = (temp32 & 0x3FF);
	return pdu;
}

uint8_t *BacNetBuffer::decode_bacnet_unsigned_int(uint8_t *pdu, uint32_t *uint_value, uint32_t len) {

	switch(len) {
	case 1 :
		*uint_value = *(pdu++);
		break;
	case 2 :
		pdu = decode_unsigned16(pdu, (uint16_t *) uint_value);
		break;
	case 3 :
		pdu = decode_unsigned24(pdu, (uint32_t *) uint_value);
		break;
	case 4 :
		pdu = decode_unsigned32(pdu, uint_value);
		break;
	default :
		if(verbosity_level_3(verbosity_level))
			printf("\n\t**** Unsigned Int invalid size ****\n");
		break;
		}

	return pdu;
}

uint8_t *BacNetBuffer::decode_bacnet_bit_string(uint8_t *pdu, uint32_t *uValue, uint32_t len) {

	/** Right now... We are only going to support a max-length of 2 (i.e. 16bit int)*/

	/** According to 20.2.10 we need to find out how many bits are not used...
	 * This was tested against the "status-flags" property...*/
	uint8_t unused = *(pdu++);

	*uValue = *(pdu++) >> unused;
	return pdu;
}

uint8_t *BacNetBuffer::decode_bacnet_boolean(uint8_t *pdu, bool *uValue, uint32_t len) {
	return pdu;
}

uint8_t *BacNetBuffer::decode_bacnet_character_string(uint8_t *pdu, char *stringValue, uint32_t len)
{
	uint8_t *p;
	p = (pdu + len);

	/** This is the character-set code used. I only support 0x00 (ANSI X3.4)*/
	if(*pdu == 0x00)
	{
		pdu++;
		memcpy(stringValue, pdu, len-1);
		stringValue[len - 1] = '\0';
	}
	else
	{
		memcpy(stringValue, "Not Supported", 14);
	}

	return pdu += len-1;
}

uint8_t *BacNetBuffer::decode_bacnet_enumeration(uint8_t *pdu, uint32_t *enum_value, uint32_t len) {

	switch(len) {
	case 1 :
		*enum_value = *(pdu++);
		break;
	case 2 :
		pdu = decode_unsigned16(pdu, (uint16_t *) enum_value);
		break;
	case 3 :
		pdu = decode_unsigned24(pdu, (uint32_t *) enum_value);
		break;
	case 4 :
		pdu = decode_unsigned32(pdu, enum_value);
		break;
	default :
		if(verbosity_level_3(verbosity_level))
			printf("\n\t**** Enumeration invalid size ****\n");
		break;
		}

	return pdu;
}

uint8_t *BacNetBuffer::encode_bacnet_enumeration(uint8_t *pdu, uint32_t value)
{

	if (value < 0x100) {
		*(pdu++) = (uint8_t) value;
	} else if (value < 0x10000) {
		pdu = encode_unsigned16(pdu, (uint16_t) value);
	} else if (value < 0x1000000) {
		pdu = encode_unsigned24(pdu, value);
	} else {
		pdu = encode_unsigned32(pdu, value);
	}
	return pdu;
}

uint8_t *BacNetBuffer::decode_bacnet_real(uint8_t *apdu, float *p)
{

	union {
	        uint8_t byte[4];
	        float real_value;
	    } my_data;

	    my_data.byte[3] = *(apdu++);
	    my_data.byte[2] = *(apdu++);
	    my_data.byte[1] = *(apdu++);
	    my_data.byte[0] = *(apdu++);

	    *p = my_data.real_value;

	return apdu;
}

uint8_t *BacNetBuffer::encode_bacnet_real(uint8_t *apdu, BACNET_APPLICATION_DATA_VALUE *val)
{

	union {
	        uint8_t byte[4];
	        float real_value;
	    } my_data;

	    my_data.real_value = val->type.Real;
	    *(apdu++) = my_data.byte[3];
	    *(apdu++) = my_data.byte[2];
	    *(apdu++) = my_data.byte[1];
	    *(apdu++) = my_data.byte[0];

	return apdu;
}

uint8_t *BacNetBuffer::encode_bacnet_context_tag(uint8_t *pdu, uint8_t tagNumber, uint32_t tagLenValType, unsigned int lvt_len){

	if(verbosity_level_3(verbosity_level))
		printf("\ntagNumber: %u, tagLenValType: %u\n",tagNumber,tagLenValType);
	const int TAG_CLASS = ((CONTEXT_SPECIFIC << 3) & 0x08);
	uint8_t *ptrTag = pdu;

	*ptrTag = (TAG_CLASS & 0xFF);

	++pdu;

	/** See if tag number portion needs to be extended or not 0-14 inclusively.*/
	if(tagNumber > 0x0E){
		/** Extend...*/
		*ptrTag |= 0xF0;
		*(pdu++) = tagNumber;
	}
	else{
		/** It fits...*/
		*ptrTag |= (tagNumber << 4);
	}
/*
	if(tagLenValType == OPENING_TAG){
		*ptrTag |= OPENING_TAG;
	}

	if(tagLenValType == CLOSING_TAG){
		*ptrTag |= CLOSING_TAG;
	}
*/
	//if(tagLenValType != OPENING_TAG && tagLenValType != CLOSING_TAG) {
	if(true) {
		if(lvt_len <= 0x04){
			*ptrTag |= lvt_len;
			switch(lvt_len){
			case 0 :
				break;
			case 1 :
				*(pdu++) = tagLenValType;
				break;
			case 2 :
				pdu = encode_unsigned16(pdu, tagLenValType);
				break;
			case 3 :
				pdu = encode_unsigned24(pdu, tagLenValType);
				break;
			case 4 :
				pdu = encode_unsigned32(pdu, tagLenValType);
				break;
			}
		}
		else {
			*ptrTag |= 0x05;

			if(lvt_len > 0x04 && lvt_len <= 0xFD){
				*(pdu++) = lvt_len;
			}

			if(lvt_len > 0xFD && lvt_len <= 0xFFFF){
				*(pdu++) = 0xFE;
				pdu = encode_unsigned16(pdu, lvt_len);
			}

			if(lvt_len > 0xFFFF && lvt_len <= 0xFFFFFFFF){
				*(pdu++) = 0xFF;
				pdu = encode_unsigned32(pdu, lvt_len);
			}

		}
	}
	return pdu;
}

uint8_t *BacNetBuffer::encode_bacnet_application_tag(uint8_t *pdu, uint8_t tagNumber, uint32_t tagLenValType){

	if(verbosity_level_3(verbosity_level))
		printf("\ntagNumber: %u, tagLenValType: %u\n",tagNumber,tagLenValType);
	const int TAG_CLASS = ((APPLICATION_SPECIFIC << 3) & 0x08);

	*pdu = (tagNumber << 4);

	*pdu |= (TAG_CLASS & 0xFF);

	*pdu |= (tagLenValType & 0xFF);

	pdu++;

	return pdu;
}

uint8_t *BacNetBuffer::encode_bacnet_tag(uint8_t *pdu, uint8_t tagNumber, uint8_t tagClass, uint32_t tagLenValType, unsigned int lvt_len){

	if(tagClass == APPLICATION_SPECIFIC){
		pdu = encode_bacnet_application_tag(pdu, tagNumber, tagLenValType);
	}
	else{
		pdu = encode_bacnet_context_tag(pdu, tagNumber, tagLenValType, lvt_len);
	}

	return pdu;
}

uint32_t BacNetBuffer::encode_bacnet_objectIdentifier(uint16_t objType, uint32_t instanceNumber, uint32_t *bacObjId){

	uint32_t tmp32 = 0;

	tmp32 = (uint32_t)((uint32_t)objType << 22);

	tmp32 |= (instanceNumber & 0x3FFFFF);

	*bacObjId = tmp32;

	return tmp32;
}

uint8_t *BacNetBuffer::encode_bacnet_application_tag_real(uint8_t *pdu, uint8_t tagNumber, BACNET_APPLICATION_DATA_VALUE *val) {

	/** Encode opening tag...*/
	if(verbosity_level_3(verbosity_level))
		printf("\nEncoding opening tag...\n");
	uint8_t *ptr = pdu;
	pdu = encode_bacnet_opening_tag(pdu, tagNumber, OPENING_TAG, 1);
	if(verbosity_level_3(verbosity_level))
		this->hexdump(ptr, pdu-ptr);

	/** Encode application tag...*/
	if(verbosity_level_3(verbosity_level))
		printf("\nEncoding application tag...\n");
	pdu = encode_bacnet_tag(pdu, BACNET_APPLICATION_TAG_REAL, APPLICATION_SPECIFIC, BACNET_APPLICATION_TAG_REAL, 0);
	if(verbosity_level_3(verbosity_level))
		this->hexdump(ptr, pdu-ptr);
	pdu = encode_bacnet_real(pdu, val);

	/** Encode closing tag...*/
	pdu = encode_bacnet_closing_tag(pdu, tagNumber, CLOSING_TAG, 1);

	return pdu;
}

uint8_t *BacNetBuffer::encode_bacnet_application_tag_enumeration(uint8_t *pdu, uint8_t tagNumber, BACNET_APPLICATION_DATA_VALUE *val) {

	uint32_t val_enum = val->type.Enumerated;
	/** Encode opening tag...*/
	if(verbosity_level_3(verbosity_level))
		printf("\nEncoding opening tag...\n");
	uint8_t *ptr = pdu;
	pdu = encode_bacnet_opening_tag(pdu, tagNumber, OPENING_TAG, 1);
	if(verbosity_level_3(verbosity_level))
		this->hexdump(ptr, pdu-ptr);

	/** Encode application tag...*/
	if(verbosity_level_3(verbosity_level))
		printf("\nEncoding application tag...\n");

	pdu = encode_bacnet_tag(pdu, BACNET_APPLICATION_TAG_ENUMERATED, APPLICATION_SPECIFIC, get_bacnet_unsigned_length(val_enum), 0);
	if(verbosity_level_3(verbosity_level))
		this->hexdump(ptr, pdu-ptr);

	/** Encode enumerated octet...*/
	pdu = encode_bacnet_enumeration(pdu, val_enum);

	/** Encode closing tag...*/
	pdu = encode_bacnet_closing_tag(pdu, tagNumber, CLOSING_TAG, 1);

	return pdu;
}

uint8_t *BacNetBuffer::encode_bacnet_application_tag_unsignedInt(uint8_t *pdu, uint8_t tagNumber, uint32_t val) {

	/** Encode opening tag...*/
	if(verbosity_level_3(verbosity_level))
		printf("\nEncoding opening tag...\n");
	uint8_t *ptr = pdu;
	pdu = encode_bacnet_opening_tag(pdu, tagNumber, OPENING_TAG, 1);
	if(verbosity_level_3(verbosity_level))
		this->hexdump(ptr, pdu-ptr);

	/** Encode application tag...*/
	if(verbosity_level_3(verbosity_level))
		printf("\nEncoding application tag...\n");

	pdu = encode_bacnet_tag(pdu, BACNET_APPLICATION_TAG_UNSIGNED_INT, APPLICATION_SPECIFIC, get_bacnet_unsigned_length(val), 0);
	if(verbosity_level_3(verbosity_level))
		this->hexdump(ptr, pdu-ptr);

	/** Encode unsigned octet...*/
	pdu = encode_bacnet_enumeration(pdu, val);

	/** Encode closing tag...*/
	pdu = encode_bacnet_closing_tag(pdu, tagNumber, CLOSING_TAG, 1);

	return pdu;
}

//uint8_t *BacNetBuffer::encode_bacnet_constructed_tag(uint8_t *pdu, uint8_t tagNumber, BACNET_APPLICATION_TAG tag,
//		BACNET_APPLICATION_DATA_VALUE *val, uint32_t length) {

	/** Encode opening tag...*/
//	pdu = encode_bacnet_tag(pdu, tagNumber, CONTEXT_SPECIFIC, OPENING_TAG, 1);

	/** Encode application value...*/
	//pdu = encode_bacnet_application_tag_real(pdu, val);

	/** Encode closing tag...*/
//	pdu = encode_bacnet_tag(pdu, tagNumber, CONTEXT_SPECIFIC, CLOSING_TAG, 1);

//	return pdu;
//}

uint8_t *BacNetBuffer::encode_bacnet_opening_tag(uint8_t *pdu, uint8_t tagNumber, uint32_t tagLenValType, unsigned int lvt_len) {

	if(verbosity_level_3(verbosity_level))
		printf("\ntagNumber: %u, tagLenValType: %u\n",tagNumber,tagLenValType);
	const int TAG_CLASS = ((CONTEXT_SPECIFIC << 3) & 0x08);
	uint8_t *ptrTag = pdu;

	*ptrTag = (TAG_CLASS & 0xFF);

	++pdu;

	/** See if tag number portion needs to be extended or not 0-14 inclusively.*/
	if(tagNumber > 0x0E){
		/** Extend...*/
		*ptrTag |= 0xF0;
		*(pdu++) = tagNumber;
	}
	else{
		/** It fits...*/
		*ptrTag |= (tagNumber << 4);
	}

	if(tagLenValType == OPENING_TAG){
		*ptrTag |= OPENING_TAG;
	}

	if(tagLenValType == CLOSING_TAG){
		*ptrTag |= CLOSING_TAG;
	}

	return pdu;
}

uint8_t *BacNetBuffer::encode_bacnet_closing_tag(uint8_t *pdu, uint8_t tagNumber, uint32_t tagLenValType, unsigned int lvt_len) {

	if(verbosity_level_3(verbosity_level))
		printf("\ntagNumber: %u, tagLenValType: %u\n",tagNumber,tagLenValType);
	const int TAG_CLASS = ((CONTEXT_SPECIFIC << 3) & 0x08);
	uint8_t *ptrTag = pdu;

	*ptrTag = (TAG_CLASS & 0xFF);

	++pdu;

	/** See if tag number portion needs to be extended or not 0-14 inclusively.*/
	if(tagNumber > 0x0E){
		/** Extend...*/
		*ptrTag |= 0xF0;
		*(pdu++) = tagNumber;
	}
	else{
		/** It fits...*/
		*ptrTag |= (tagNumber << 4);
	}

	if(tagLenValType == OPENING_TAG){
		*ptrTag |= OPENING_TAG;
	}

	if(tagLenValType == CLOSING_TAG){
		*ptrTag |= CLOSING_TAG;
	}

	return pdu;
}

uint8_t *BacNetBuffer::decode_bacnet_abstract_syntax_type(uint8_t *pdu, BACNET_APPLICATION_DATA_VALUE *value, BACNET_APPLICATION_TAG *tag) {
	/** It is constructed data. It could have many taged elements that follow.
	 * 0x06 represents "opening-tag" and 0x07 represents closing tag. 20.2.1.3.2*/

	uint8_t tagNumber = 0;
	uint8_t tagClass = 0;
	uint32_t tagLVT = 0;

	pdu = decode_bacnet_tag(pdu, &tagNumber, &tagClass, &tagLVT);

	switch(tagNumber){
	case BACNET_APPLICATION_TAG_NULL:
		break;
	case BACNET_APPLICATION_TAG_BOOLEAN:
		break;
	case BACNET_APPLICATION_TAG_UNSIGNED_INT:
		break;
	case BACNET_APPLICATION_TAG_SIGNED_INT:
		break;
	case BACNET_APPLICATION_TAG_REAL:
		*tag = BACNET_APPLICATION_TAG_REAL;
		pdu = decode_bacnet_real(pdu, &value->type.Real);
		break;
	case BACNET_APPLICATION_TAG_DOUBLE:
		break;
	case BACNET_APPLICATION_TAG_OCTET_STRING:
		break;
	case BACNET_APPLICATION_TAG_CHARACTER_STRING:
		break;
	case BACNET_APPLICATION_TAG_BIT_STRING:
		*tag = BACNET_APPLICATION_TAG_BIT_STRING;
		pdu = decode_bacnet_bit_string(pdu, &value->type.Unsigned_Int, tagLVT);
		break;
	case BACNET_APPLICATION_TAG_ENUMERATED:
		*tag = BACNET_APPLICATION_TAG_ENUMERATED;
		pdu = decode_bacnet_enumeration(pdu,&value->type.Enumerated, tagLVT);
		break;
	case BACNET_APPLICATION_TAG_DATE:
		break;
	case BACNET_APPLICATION_TAG_TIME:
		break;
	case BACNET_APPLICATION_TAG_OBJECT_ID:
		break;
	case BACNET_APPLICATION_TAG_RESERVE1:
	case BACNET_APPLICATION_TAG_RESERVE2:
	case BACNET_APPLICATION_TAG_RESERVE3:
	default:
		if(verbosity_level_1(verbosity_level))
			printf("Data type (%u): Unknown! Now what???\n", tagNumber);
		break;
	}

	return pdu;
}





