/*
 * BacNetConfirmedRequestPdu.cpp
 *
 *  Created on: Aug 26, 2011
 *      Author: 8w4
 */

#include "BacNetConfirmedRequestPdu.h"
#include "common.h"

BacNetConfirmedRequestPdu::BacNetConfirmedRequestPdu()
: pdu_type(PDU_TYPE_CONFIRMED_SERVICE_REQUEST)
{
	this->segmented_message = false;
	this->more_follows = false;
	this->segmented_response_accepted = false;
	this->max_segments_accepted = 0x00;		/** Unspecified 20.1.2.4*/
	this->max_apdu_length_accepted = 0x00;	/** 0 octets 20.1.2.5*/
	this->invoke_id = 0;
	this->sequence_number = 0;
	this->proposed_window_size = 0;
}

BacNetConfirmedRequestPdu::BacNetConfirmedRequestPdu(uint8_t svc_choice, uint8_t invoke)
: pdu_type(PDU_TYPE_CONFIRMED_SERVICE_REQUEST), service_choice(svc_choice)
{

	this->segmented_message = false;
	this->more_follows = false;
	this->segmented_response_accepted = false;
	this->max_segments_accepted = 0x00;		/** Unspecified 20.1.2.4*/
	this->max_apdu_length_accepted = 0x05;	/** 1476 octets 20.1.2.5*/
	this->invoke_id = invoke;
	this->sequence_number = 0;
	this->proposed_window_size = 0;
}

BacNetBuffer *BacNetConfirmedRequestPdu::encode_confirmed_service_req(BacNetBuffer *buffer){

	uint8_t *pdu = buffer->get_currentPosition();

	*pdu = this->pdu_type << 4;
	if(this->segmented_message)
		*pdu |= 0x08;
	if(this->more_follows)
		*pdu |= 0x04;
	if(this->segmented_response_accepted)
		*pdu |= 0x02;

	++pdu;

	/** Max segments accepted? 0=Unspecified. 20.1.2.4*/
	*pdu = 0 << 4;
	/** Max APDU length accepted? 5=1476 octets. 20.1.2.5*/
	*pdu |= 0x05;

	++pdu;

	/** Invoke ID 0-255. 20.1.2.6*/
	//*(pdu++) = 0x01;
	*(pdu++) = invoke_id;

	/** Sequence number <optional>... Not used right now...
	 * *(pdu++) = some sequence number...*/

	/** Proposed window size <optional>... Not used right now...
	 * *(pdu++) = some window size...*/

	*(pdu++) = this->service_choice;

	buffer->set_currentPosition(pdu);
	buffer->set_length(pdu);

	return buffer;
}

BacNetBuffer *BacNetConfirmedRequestPdu::decode_confirmed_request(class BacNetBuffer *buffer) {

	uint8_t *pdu = buffer->get_currentPosition();

	/** Should be pointing to Confirmed PDU type; move to Service Choice octet...*/
	if(*pdu == PDU_TYPE_CONFIRMED_SERVICE_REQUEST) {

		/** Strip off the PDU flags...*/
		this->segmented_message = ((*pdu & 0x08) == 0x08) ? 1 : 0;
		this->more_follows = ((*pdu & 0x04) == 0x04) ? 1 : 0;
		this->segmented_message = ((*pdu & 0x02) == 0x02) ? 1 : 0;

		/** Next octet please...*/
		pdu++;
		this->max_segments_accepted = *pdu & 0xF0;
		this->max_apdu_length_accepted = *pdu & 0x0F;

		/** Next octet please...*/
		pdu++;
		this->invoke_id = *(pdu++);

		this->service_choice = *(pdu++);

		buffer->set_currentPosition(pdu);

	}
	else {
		/** This is not a Confirmed Service PDU... So, Leave now!*/
		if(verbosity_level_1(verbosity_level))
			printf("\nTried to decode Confirmed Service PDU, but this (%X) isn't it...\n", *pdu);
		return NULL;
	}

	return buffer;
}

BacNetConfirmedRequestPdu::~BacNetConfirmedRequestPdu() {
	// TODO Auto-generated destructor stub
}

uint8_t BacNetConfirmedRequestPdu::getPdu_type() const
{
	return pdu_type;
}

bool BacNetConfirmedRequestPdu::getSegmented_message() const
{
	return segmented_message;
}

void BacNetConfirmedRequestPdu::setSegmented_message(bool segmented_message)
{
	this->segmented_message = segmented_message;
}

bool BacNetConfirmedRequestPdu::getMore_follows() const
{
	return more_follows;
}

void BacNetConfirmedRequestPdu::setMore_follows(bool more_follows)
{
	this->more_follows = more_follows;
}

bool BacNetConfirmedRequestPdu::getSegmented_response_accepted() const
{
	return segmented_response_accepted;
}

void BacNetConfirmedRequestPdu::setSegmented_response_accepted(bool segmented_response_accepted)
{
	this->segmented_response_accepted = segmented_response_accepted;
}

uint8_t BacNetConfirmedRequestPdu::getMax_segments_accepted() const
{
	return max_segments_accepted;
}

void BacNetConfirmedRequestPdu::setMax_segments_accepted(uint8_t max_segments_accepted)
{
	this->max_segments_accepted = max_segments_accepted;
}

uint8_t BacNetConfirmedRequestPdu::getMax_apdu_length_accepted() const
{
	return max_apdu_length_accepted;
}

void BacNetConfirmedRequestPdu::setMax_apdu_length_accepted(uint8_t max_apdu_length_accepted)
{
	this->max_apdu_length_accepted = max_apdu_length_accepted;
}

uint8_t BacNetConfirmedRequestPdu::getInvoke_id() const
{
	return invoke_id;
}

void BacNetConfirmedRequestPdu::setInvoke_id(uint8_t invoke_id)
{
	this->invoke_id = invoke_id;
}

uint8_t BacNetConfirmedRequestPdu::getSequence_number() const
{
	return sequence_number;
}

void BacNetConfirmedRequestPdu::setSequence_number(uint8_t sequence_number)
{
	this->sequence_number = sequence_number;
}

uint8_t BacNetConfirmedRequestPdu::getProposed_window_size() const
{
	return proposed_window_size;
}

void BacNetConfirmedRequestPdu::setProposed_window_size(uint8_t proposed_window_size)
{
	this->proposed_window_size = proposed_window_size;
}

uint8_t BacNetConfirmedRequestPdu::getService_choice() const
{
	return service_choice;
}






