/*
 * BacNetWritePropertyRequest.cpp
 *
 *  Created on: Nov 16, 2011
 *      Author: 8w4
 */

#include "BacNetWritePropertyRequest.h"
#include "BacNetNpdu.h"
#include "BacNetBVLC.h"
#include "common.h"


#define CONTEXTSPECIFIC 1

BacNetWritePropertyRequest::BacNetWritePropertyRequest()
: BacNetConfirmedRequestPdu(SERVICE_CONFIRMED_WRITE_PROPERTY, 0)
{

}

BacNetWritePropertyRequest::~BacNetWritePropertyRequest() {
	// TODO Auto-generated destructor stub
}

BacNetWritePropertyRequest::BacNetWritePropertyRequest(const BACNET_OBJECT_ID *bac_obj_id, const uint32_t *prop_id, const int *prop_array_index, uint8_t invoke_id, const uint8_t *priority, const BACNET_APPLICATION_TAG tag, const BACNET_APPLICATION_DATA_VALUE *value)
: BacNetConfirmedRequestPdu(SERVICE_CONFIRMED_WRITE_PROPERTY, invoke_id), bacnet_obj_id(bac_obj_id), property_id(prop_id), property_array_index(prop_array_index), write_priority(priority), application_tag(tag), application_value(value)
{

}

void BacNetWritePropertyRequest::set_invoke_id(uint8_t new_id){
	this->setInvoke_id(new_id);
}


BacNetBuffer *BacNetWritePropertyRequest::encode_request(BacNetBuffer *buffer){

	if(verbosity_level_3(verbosity_level))
		printf("\nBacNetWritePropertyRequest::encode_request\n");
	/** Set buffer to begining...*/
	buffer->set_currentPosition(buffer->getPointerToBuffer());

	/** Encode the Confirmed-Service portion of the message...*/
	this->encode_confirmed_service_req(buffer);

	uint8_t *pdu = buffer->get_currentPosition();

	/** Build BACnet object identifier and encode in a context-specific tag...*/
	uint32_t tmp32 = 0;
	buffer->encode_bacnet_objectIdentifier(this->bacnet_obj_id->type, this->bacnet_obj_id->instance, &tmp32);

	pdu = buffer->encode_bacnet_tag(pdu, 0, CONTEXTSPECIFIC, tmp32, 4);

	if(verbosity_level_3(verbosity_level))
		buffer->hexdump(buffer->getPointerToBuffer(), pdu-buffer->getPointerToBuffer());

	/** Build Property Identifier and encode tag...*/
	pdu = buffer->encode_bacnet_tag(pdu, 1, CONTEXTSPECIFIC, *this->property_id, 1);
	if(verbosity_level_3(verbosity_level))
		buffer->hexdump(buffer->getPointerToBuffer(), pdu-buffer->getPointerToBuffer());

	/** Check to see if we are writing to a "property-array"...
	 * If so, we need to load index of array, if not, skip...*/

	if(*this->property_array_index >= 0){
		/** Build Property Identifier and encode tag...*/
		pdu = buffer->encode_bacnet_tag(pdu, 2, CONTEXTSPECIFIC, *this->property_array_index, 1);
	}

	uint32_t hack4ui32 = 0;

	/** Now build the property-value portion...*/
	switch(this->application_tag) {
	case BACNET_APPLICATION_TAG_NULL:
		break;
	case BACNET_APPLICATION_TAG_BOOLEAN:
			break;
	case BACNET_APPLICATION_TAG_UNSIGNED_INT:
		if(verbosity_level_3(verbosity_level))
			printf("\nEncoding BACNET_APPLICATION_TAG_UNSIGNED_INT >>>>\n");

		hack4ui32 = this->application_value->type.Real;
		pdu = buffer->encode_bacnet_application_tag_unsignedInt(pdu, 3, hack4ui32);
		if(verbosity_level_3(verbosity_level)) {
			buffer->hexdump(buffer->getPointerToBuffer(), pdu - buffer->getPointerToBuffer());
			printf("\nEncoding BACNET_APPLICATION_TAG_UNSIGNED_INT <<<<\n");
		}
			break;
	case BACNET_APPLICATION_TAG_SIGNED_INT:
			break;
	case BACNET_APPLICATION_TAG_REAL:
		if(verbosity_level_3(verbosity_level))
			printf("\nEncoding BACNET_APPLICATION_TAG_REAL >>>>\n");
		pdu = buffer->encode_bacnet_application_tag_real(pdu, 3, (BACNET_APPLICATION_DATA_VALUE *)this->application_value);
		if(verbosity_level_3(verbosity_level)) {
			buffer->hexdump(buffer->getPointerToBuffer(), pdu - buffer->getPointerToBuffer());
			printf("\nEncoding BACNET_APPLICATION_TAG_REAL <<<<\n");
		}
			break;
	case BACNET_APPLICATION_TAG_DOUBLE:
			break;
	case BACNET_APPLICATION_TAG_OCTET_STRING:
			break;
	case BACNET_APPLICATION_TAG_CHARACTER_STRING:
			break;
	case BACNET_APPLICATION_TAG_BIT_STRING:
			break;
	case BACNET_APPLICATION_TAG_ENUMERATED:
		if(verbosity_level_3(verbosity_level))
			printf("\nEncoding BACNET_APPLICATION_TAG_ENUMERATED >>>>\n");
		pdu = buffer->encode_bacnet_application_tag_enumeration(pdu, 3, (BACNET_APPLICATION_DATA_VALUE *)this->application_value);
		if(verbosity_level_3(verbosity_level)) {
			buffer->hexdump(buffer->getPointerToBuffer(), pdu - buffer->getPointerToBuffer());
			printf("\nEncoding BACNET_APPLICATION_TAG_ENUMERATED <<<<\n");
		}
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
		if(verbosity_level_3(verbosity_level))
			printf("\nCan't encode BACNET_APPLICATION_TAG: \n");
		break;
	}

	/** Priority!!! Don't forget it!!!*/
	pdu = buffer->encode_bacnet_tag(pdu, 4, CONTEXTSPECIFIC, *this->write_priority, 1);



	buffer->set_length(pdu);

	if(verbosity_level_3(verbosity_level))
		buffer->hexdump(buffer->getPointerToBuffer(), pdu-buffer->getPointerToBuffer());

	return buffer;
}
