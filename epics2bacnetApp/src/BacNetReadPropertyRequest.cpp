/*
 * ReadPropertyRequest.cpp
 *
 *  Created on: Aug 26, 2011
 *      Author: 8w4
 */

#include "BacNetReadPropertyRequest.h"
#include "BacNetNpdu.h"
#include "BacNetBVLC.h"
#include "common.h"

#define CONTEXTSPECIFIC 1

BacNetReadPropertyRequest::BacNetReadPropertyRequest()
: BacNetConfirmedRequestPdu(SERVICE_CONFIRMED_READ_PROPERTY, 0)
{

}

BacNetReadPropertyRequest::~BacNetReadPropertyRequest() {
	// TODO Auto-generated destructor stub
}

BacNetReadPropertyRequest::BacNetReadPropertyRequest(const BACNET_OBJECT_ID *bac_obj_id, const uint32_t *prop_id, const int *prop_array_index, uint8_t invoke_id)
: BacNetConfirmedRequestPdu(SERVICE_CONFIRMED_READ_PROPERTY, invoke_id), bacnet_obj_id(bac_obj_id), property_id(prop_id), property_array_index(prop_array_index)
{

}

void BacNetReadPropertyRequest::set_invoke_id(uint8_t new_id){
	this->setInvoke_id(new_id);
}

BacNetBuffer *BacNetReadPropertyRequest::encode_read_property(BacNetBuffer *buffer, BacNetDevice *dev, BacNetVariable *var, uint8_t invoke_id){

	this->encode_confirmed_service_req(buffer);

	uint8_t *pdu = buffer->get_currentPosition();

	/** Clause 21 ReadProperty-Request ::= SEQUENCE {*/

	/** Build "BACnetObjectIdentifier" and encode in a context-specific tag...*/
	uint32_t tmp32 = 0;
	buffer->encode_bacnet_objectIdentifier(var->getObjectType(), var->getObject_instance(), &tmp32);

	pdu = buffer->encode_bacnet_tag(pdu, 0, CONTEXTSPECIFIC, tmp32, 4);

	if(verbosity_level_3(verbosity_level))
		buffer->hexdump(buffer->getPointerToBuffer(), pdu-buffer->getPointerToBuffer());

	/** Encode "BACnetPropertyIdentifier" in a context-specific tag...*/
	pdu = buffer->encode_bacnet_tag(pdu, 1, CONTEXTSPECIFIC, var->getProperty_Id(), 1);

	if(verbosity_level_3(verbosity_level))
		buffer->hexdump(buffer->getPointerToBuffer(), pdu-buffer->getPointerToBuffer());

	buffer->set_length(pdu);

	/** Build NPDU...*/
	BacNetNpdu *n = new BacNetNpdu();
	n->set_npdu_expecting_reply(true);

	if(dev->getSlen()>0){
		//printf("\nThis (%s) is behind a router.\n", dev->getName());
		//printf("\nSLEN: %u, SNET: %u, SADR: %u\n", dev->getSlen(), dev->getSnet(), *dev->getSadr());
		n->set_npdu_destination_specifier(true);
		n->set_npdu_dlen(dev->getSlen());
		n->set_npdu_dnet(dev->getSnet());
		n->set_npdu_dadr(dev->getSadr());
		//n->toString();
	}
	n->encode_npdu(buffer);

	if(verbosity_level_3(verbosity_level))
		buffer->hexdump(buffer->getPointerToBuffer(), buffer->getLength());

	BacNetBVLC *bvlc = new BacNetBVLC();
	bvlc->encode_bvlc_originalUnicastNpdu(buffer);

	if(verbosity_level_3(verbosity_level))
		buffer->hexdump(buffer->getPointerToBuffer(), buffer->getLength());

	return buffer;
}

BacNetBuffer *BacNetReadPropertyRequest::encode_request(BacNetBuffer *buffer){

	/** Set buffer to begining...*/
	buffer->set_currentPosition(buffer->getPointerToBuffer());

	/** Encode the Confirmed-Service portion of the message...*/
	this->encode_confirmed_service_req(buffer);

	uint8_t *pdu = buffer->get_currentPosition();

	/** Clause 21 ReadProperty-Request ::= SEQUENCE {*/

	/** Build "BACnetObjectIdentifier" and encode in a context-specific tag...*/
	uint32_t tmp32 = 0;
	buffer->encode_bacnet_objectIdentifier(this->bacnet_obj_id->type, this->bacnet_obj_id->instance, &tmp32);

	pdu = buffer->encode_bacnet_tag(pdu, 0, CONTEXTSPECIFIC, tmp32, 4);

	if(verbosity_level_3(verbosity_level))
		buffer->hexdump(buffer->getPointerToBuffer(), pdu-buffer->getPointerToBuffer());

	/** Build "BACnetPropertyIdentifier" and encode tag...*/
	pdu = buffer->encode_bacnet_tag(pdu, 1, CONTEXTSPECIFIC, *this->property_id, 1);

	if(verbosity_level_3(verbosity_level))
		buffer->hexdump(buffer->getPointerToBuffer(), pdu-buffer->getPointerToBuffer());

	/** Check to see if we are reading a "property-array"...
	 * If so, we need to load index of array, if not, skip...*/

	if(*this->property_array_index >= 0){
		/** Build "PropertyArrayIndex" and encode tag...*/
		pdu = buffer->encode_bacnet_tag(pdu, 2, CONTEXTSPECIFIC, *this->property_array_index, 1);
	}

	buffer->set_length(pdu);

	if(verbosity_level_3(verbosity_level))
		buffer->hexdump(buffer->getPointerToBuffer(), pdu-buffer->getPointerToBuffer());

	return buffer;
}

