/*
 * BacNet_I_Am_Request.cpp
 *
 *  Created on: Sep 12, 2011
 *      Author: 8w4
 */


#include "BacNet_I_Am_Request.h"
#include "common.h"

#include <stdio.h>

BacNet_I_Am_Request::BacNet_I_Am_Request() {
	this->bacnet_object_id.type = 0;
	this->bacnet_object_id.instance = 0;
	this->max_apdu_length_accepted = 0;
	this->segmentation_supported = (BACNET_SEGMENTATION) 0;
	this->vendor_id = 0;

}

BacNet_I_Am_Request::~BacNet_I_Am_Request() {
	// TODO Auto-generated destructor stub
}

BacNetBuffer *BacNet_I_Am_Request::decode_response(BacNetBuffer *buffer) {
	uint8_t *pdu = buffer->get_currentPosition();

	if(*(pdu++) != PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST){
		printf("\nBacNet_I_Am_Request::decode_I_Am_Request. Wrong PDU type: %X\n", *pdu);
		return NULL;
	}

	/** pass us a pointer to the I-AM request value should be 0x00*/
	if(*pdu != SERVICE_UNCONFIRMED_I_AM) {
		printf("\nBacNet_I_Am_Request::decode_I_Am_Request. Wrong service type: %X\n", *pdu);
		return NULL;
	}
	else
	{
		buffer->hexdump(pdu, 5);
		/** Go to next element...*/
		++pdu;

		/** Decode BacNet Object ID...*/
		uint8_t tag_number;
		uint8_t tag_class;
		uint32_t tag_lvt;
		buffer->hexdump(pdu, 5);
		pdu = buffer->decode_bacnet_tag(pdu, &tag_number, &tag_class, &tag_lvt);

		if(tag_number != BACNET_APPLICATION_TAG_OBJECT_ID) {
			return NULL;
		}

		uint16_t objType;
		uint32_t instanceNumber;

		pdu = buffer->decode_bacnet_object_id(pdu, &objType, &instanceNumber);

		buffer->hexdump(pdu, 5);
		printf("\nObjType: %u, Instance: %u\n", objType, instanceNumber);

		/** Decode MAX APDU length...*/
		pdu = buffer->decode_bacnet_tag(pdu, &tag_number, &tag_class, &tag_lvt);
		if(tag_number != BACNET_APPLICATION_TAG_UNSIGNED_INT) {
			return NULL;
		}

		printf("\nTagNumber: %u, TagClass: %u, TagLVT: %u\n", tag_number, tag_class, tag_lvt);
		uint32_t uVal = 0;
		pdu = buffer->decode_bacnet_unsigned_int(pdu, &uVal, tag_lvt);
		printf("\nUVAL: %u\n", uVal);

		/** Decode Segmentation Supported...*/
		pdu = buffer->decode_bacnet_tag(pdu, &tag_number, &tag_class, &tag_lvt);
		if(tag_number != BACNET_APPLICATION_TAG_ENUMERATED) {
			return NULL;
		}
		printf("\nTagNumber: %u, TagClass: %u, TagLVT: %u\n", tag_number, tag_class, tag_lvt);
		unsigned segSup;
		pdu = buffer->decode_bacnet_enumeration(pdu, &segSup, tag_lvt);
		printf("\nENUMVAL: %u\n", segSup);

		/** Decode Vendor ID...*/
		pdu = buffer->decode_bacnet_tag(pdu, &tag_number, &tag_class, &tag_lvt);
		if(tag_number != BACNET_APPLICATION_TAG_UNSIGNED_INT) {
			return NULL;
		}
		printf("\nTagNumber: %u, TagClass: %u, TagLVT: %u\n", tag_number, tag_class, tag_lvt);
		unsigned vend;
		pdu = buffer->decode_bacnet_enumeration(pdu, &vend, tag_lvt);
		printf("\nVENDOR: %u\n", vend);

		this->bacnet_object_id.type = objType;
		this->bacnet_object_id.instance = instanceNumber;
		this->max_apdu_length_accepted = uVal;
		this->segmentation_supported = (BACNET_SEGMENTATION)segSup;
		this->vendor_id = vend;
	}

	buffer->set_currentPosition(pdu);
	return buffer;
}

void BacNet_I_Am_Request::decode_unconfirmed_request(class BacNetBuffer *buffer) {

}

BacNetBuffer *BacNet_I_Am_Request::decode_iam_request(BacNetBuffer *buffer) {

	uint8_t *pdu = buffer->get_currentPosition();

	if(*(pdu++) != PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST){
		if(verbosity_level_2(verbosity_level))
			printf("\nBacNet_I_Am_Request::decode_I_Am_Request. Wrong PDU type: %X\n", *pdu);
		return NULL;
	}

	/** pass us a pointer to the I-AM request value should be 0x00*/
	if(*pdu != SERVICE_UNCONFIRMED_I_AM) {
		if(verbosity_level_2(verbosity_level))
		printf("\nBacNet_I_Am_Request::decode_I_Am_Request. Wrong service type: %X\n", *pdu);
		return NULL;
	}
	else
	{
		if(verbosity_level_3(verbosity_level))
			buffer->hexdump(pdu, 5);
		/** Go to next element...*/
		++pdu;

		/** Decode BacNet Object ID...*/
		uint8_t tag_number;
		uint8_t tag_class;
		uint32_t tag_lvt;
		if(verbosity_level_3(verbosity_level))
			buffer->hexdump(pdu, 5);
		pdu = buffer->decode_bacnet_tag(pdu, &tag_number, &tag_class, &tag_lvt);

		if(tag_number != BACNET_APPLICATION_TAG_OBJECT_ID) {
			return NULL;
		}

		uint16_t objType;
		uint32_t instanceNumber;

		pdu = buffer->decode_bacnet_object_id(pdu, &objType, &instanceNumber);

		if(verbosity_level_3(verbosity_level)) {
			buffer->hexdump(pdu, 5);
			printf("\nObjType: %u, Instance: %u\n", objType, instanceNumber);
		}

		/** Decode MAX APDU length...*/
		pdu = buffer->decode_bacnet_tag(pdu, &tag_number, &tag_class, &tag_lvt);
		if(tag_number != BACNET_APPLICATION_TAG_UNSIGNED_INT) {
			return NULL;
		}

		if(verbosity_level_3(verbosity_level))
			printf("\nTagNumber: %u, TagClass: %u, TagLVT: %u\n", tag_number, tag_class, tag_lvt);
		uint32_t uVal = 0;
		pdu = buffer->decode_bacnet_unsigned_int(pdu, &uVal, tag_lvt);
		if(verbosity_level_3(verbosity_level))
			printf("\nUVAL: %u\n", uVal);

		/** Decode Segmentation Supported...*/
		pdu = buffer->decode_bacnet_tag(pdu, &tag_number, &tag_class, &tag_lvt);
		if(tag_number != BACNET_APPLICATION_TAG_ENUMERATED) {
			return NULL;
		}
		if(verbosity_level_3(verbosity_level))
			printf("\nTagNumber: %u, TagClass: %u, TagLVT: %u\n", tag_number, tag_class, tag_lvt);
		unsigned segSup;
		pdu = buffer->decode_bacnet_enumeration(pdu, &segSup, tag_lvt);
		if(verbosity_level_3(verbosity_level))
			printf("\nENUMVAL: %u\n", segSup);

		/** Decode Vendor ID...*/
		pdu = buffer->decode_bacnet_tag(pdu, &tag_number, &tag_class, &tag_lvt);
		if(tag_number != BACNET_APPLICATION_TAG_UNSIGNED_INT) {
			return NULL;
		}
		if(verbosity_level_3(verbosity_level))
			printf("\nTagNumber: %u, TagClass: %u, TagLVT: %u\n", tag_number, tag_class, tag_lvt);
		unsigned vend;
		pdu = buffer->decode_bacnet_enumeration(pdu, &vend, tag_lvt);
		if(verbosity_level_3(verbosity_level))
			printf("\nVENDOR: %u\n", vend);

		this->bacnet_object_id.type = objType;
		this->bacnet_object_id.instance = instanceNumber;
		this->max_apdu_length_accepted = uVal;
		this->segmentation_supported = (BACNET_SEGMENTATION)segSup;
		this->vendor_id = vend;
	}

	buffer->set_currentPosition(pdu);
	return buffer;
}

uint16_t BacNet_I_Am_Request::get_object_type() const{
	return this->bacnet_object_id.type;
}
uint32_t BacNet_I_Am_Request::get_object_instance() const{
	return this->bacnet_object_id.instance;
}
uint32_t BacNet_I_Am_Request::get_max_apdu_length() const{
	return this->max_apdu_length_accepted;
}
uint8_t BacNet_I_Am_Request::get_segmentation_supported() const{
	return this->segmentation_supported;
}
uint32_t BacNet_I_Am_Request::get_vendor_id() const{
	return this->vendor_id;
}

