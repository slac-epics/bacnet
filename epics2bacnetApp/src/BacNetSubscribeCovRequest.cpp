/*
 * BacNetSubscribeCovRequest.cpp
 *
 *  Created on: Dec 21, 2011
 *      Author: 8w4
 */

#include "BacNetSubscribeCovRequest.h"
#include "common.h"

#define CONTEXTSPECIFIC 1

BacNetSubscribeCovRequest::BacNetSubscribeCovRequest(const uint32_t pid, const BACNET_OBJECT_ID obj_id,
		const bool confirmed_not, const unsigned lt, uint8_t invokeId)
:BacNetConfirmedRequestPdu(SERVICE_CONFIRMED_SUBSCRIBE_COV, invokeId), process_id(pid), bac_obj_id(obj_id), confirmed_notifications(confirmed_not), lifetime(lt)
{
	// TODO Auto-generated constructor stub
	//printf("\nThis is COV; lifetime is: %u\n", lt);

}

BacNetSubscribeCovRequest::~BacNetSubscribeCovRequest() {
	// TODO Auto-generated destructor stub
}

BacNetBuffer *BacNetSubscribeCovRequest::encode_request(BacNetBuffer *buffer) {

	/** Set buffer to begining...*/
	buffer->set_currentPosition(buffer->getPointerToBuffer());

	/** Encode the Confirmed-Service portion of the message...*/
	this->encode_confirmed_service_req(buffer);

	uint8_t *pdu = buffer->get_currentPosition();

	/** Clause 21 SubscribeCOV-Request ::= SEQUENCE {*/

	/** Build subscriberProcessIdentifier...*/
	unsigned int l = buffer->get_bacnet_unsigned_length(this->process_id);
	pdu = buffer->encode_bacnet_tag(pdu, 0, CONTEXTSPECIFIC, this->process_id, l);
	//buffer->hexdump(buffer->getPointerToBuffer(), pdu-buffer->getPointerToBuffer());

	/** Build "BACnetObjectIdentifier" and encode in a context-specific tag...*/
	uint32_t tmp32 = 0;
	buffer->encode_bacnet_objectIdentifier(this->bac_obj_id.type, this->bac_obj_id.instance, &tmp32);

	pdu = buffer->encode_bacnet_tag(pdu, 1, CONTEXTSPECIFIC, tmp32, 4);

	if(verbosity_level_3(verbosity_level))
		buffer->hexdump(buffer->getPointerToBuffer(), pdu-buffer->getPointerToBuffer());

	/** Build "issueConfirmedNotifications" and encode tag...*/
	pdu = buffer->encode_bacnet_tag(pdu, 2, CONTEXTSPECIFIC, this->confirmed_notifications, 1);

	/** Build lifetime subscription...*/
	l = buffer->get_bacnet_unsigned_length(this->lifetime);
	pdu = buffer->encode_bacnet_tag(pdu, 3, CONTEXTSPECIFIC, this->lifetime, l);



	buffer->set_length(pdu);

	if(verbosity_level_3(verbosity_level))
		buffer->hexdump(buffer->getPointerToBuffer(), pdu-buffer->getPointerToBuffer());

	return buffer;
}
