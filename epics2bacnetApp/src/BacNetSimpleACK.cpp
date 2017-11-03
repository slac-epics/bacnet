/*
 * BacNetSimpleACK.cpp
 *
 *  Created on: Nov 16, 2011
 *      Author: 8w4
 */

#include "BacNetSimpleACK.h"

BacNetSimpleACK::BacNetSimpleACK()
:pdu_type(PDU_TYPE_SIMPLE_ACK)
{
	this->invoke_id = 0;
	this->svc = MAX_BACNET_CONFIRMED_SERVICE;
}

BacNetSimpleACK::BacNetSimpleACK(uint8_t invoke_id, BACNET_CONFIRMED_SERVICE service)
:pdu_type(PDU_TYPE_SIMPLE_ACK)
{
	this->invoke_id = invoke_id;
	this->svc = service;
}

BacNetSimpleACK::~BacNetSimpleACK() {
	// TODO Auto-generated destructor stub
}

BacNetBuffer *BacNetSimpleACK::encode_simple_ack(class BacNetBuffer *buffer, class BacNetDevice *dev) {

	uint8_t *pdu = buffer->getPointerToBuffer();

	*(pdu++) = this->pdu_type;	/** PDU type (Simple-ACK)*/
	*(pdu++) = this->invoke_id;
	*(pdu++) = this->svc;

	buffer->set_length(pdu);

	/** Build NPDU...*/
	BacNetNpdu *n = new BacNetNpdu();

	if(dev->getSlen()>0){
		n->set_npdu_destination_specifier(true);
		n->set_npdu_dlen(dev->getSlen());
		n->set_npdu_dnet(dev->getSnet());
		n->set_npdu_dadr(dev->getSadr());
		//n->toString();
	}
	n->encode_npdu(buffer);

	BacNetBVLC *bvlc = new BacNetBVLC();
	bvlc->encode_bvlc_originalUnicastNpdu(buffer);

	delete n;
	delete bvlc;

	return buffer;
}

BacNetBuffer *BacNetSimpleACK::decode_simple_ack(class BacNetBuffer *buffer){

	uint8_t *pdu = buffer->get_currentPosition();
	uint8_t type = (*pdu & 0xF0);
	if(type != PDU_TYPE_SIMPLE_ACK) {
		if(verbosity_level_1(verbosity_level))
			printf("\nPDU_TYPE(%X) is not PDU_TYPE_SIMPLE_ACK(0x20)\n", type);
		return NULL;
	}
	//this->pdu_type = (*pdu & 0xF0);
	++pdu;
	this->invoke_id = *(pdu++);
	this->svc = (BACNET_CONFIRMED_SERVICE) *(pdu++);

	buffer->set_currentPosition(pdu);

	return buffer;
}

BacNetBuffer *BacNetSimpleACK::decode_simple_ack(class BacNetBuffer *buffer, sockaddr_in *addr, class BacNetBVLC *bvlc,
		class BacNetNpdu *npdu, class BacNetDevice *dev) {

	decode_simple_ack(buffer);

	BacNetVariable *var = dev->getBacNetvariableByInvokeId(this->invoke_id);

	if(!var){
		if(verbosity_level_1(verbosity_level))
			printf("\nVar is invalid. this->invoke_id = %u\n", this->invoke_id);
		return buffer;
	}

	var->lock();
	var->setIsValid(true);
	var->unlock();
	dev->release_invoke_id_to_var(var);

	return buffer;
}
