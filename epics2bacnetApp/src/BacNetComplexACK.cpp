/*
 * BacNetComplexACK.cpp
 *
 *  Created on: Sep 26, 2011
 *      Author: 8w4
 */

#include "BacNetComplexACK.h"
#include "common.h"

BacNetComplexACK::BacNetComplexACK() {
	this->pdu_type = PDU_TYPE_COMPLEX_ACK;
	this->segmented_msg = false;
	this->more_follows = false;
	this->invoke_id = 0;
	this->sequence_number = 0;
	this->prop_window_sized = 0;
	this->svc = (BACNET_CONFIRMED_SERVICE)0;

}

BacNetComplexACK::~BacNetComplexACK() {
	// TODO Auto-generated destructor stub
}

BacNetBuffer *BacNetComplexACK::decode_complex_ack(class BacNetBuffer *buffer){

	uint8_t *pdu = buffer->get_currentPosition();
	this->pdu_type = (*pdu & 0xF0);
	this->segmented_msg = ((*pdu & 0x0F) == 0x08) ? true : false;
	this->more_follows = ((*pdu & 0x0F) == 0x04) ? true : false;
	++pdu;
	this->invoke_id = *(pdu++);
	this->svc = (BACNET_CONFIRMED_SERVICE) *(pdu++);

	buffer->set_currentPosition(pdu);

	return buffer;
}

BacNetBuffer *BacNetComplexACK::decode_complex_ack(class BacNetBuffer *buffer, sockaddr_in *addr, class BacNetBVLC *bvlc,
		class BacNetNpdu *npdu, class BacNetDevice *dev) {

	BacNetConfirmedService *confirmedService;

	decode_complex_ack(buffer);

	BacNetVariable *var = dev->getBacNetvariableByInvokeId(this->invoke_id);

	if(!var){
		if(verbosity_level_1(verbosity_level))
			printf("\nVar is invalid. this->invoke_id = %u\n", this->invoke_id);
		return buffer;
	}


	uint8_t *pdu = buffer->get_currentPosition();

	var->lock();

	/** Check to see if we are supposed to write...*/
	if(!var->get_do_write()) {

		switch(*pdu){
		/* Alarm and Event Services */
		case SERVICE_CONFIRMED_ACKNOWLEDGE_ALARM:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_ACKNOWLEDGE_ALARM PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_COV_NOTIFICATION:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_COV_NOTIFICATION PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_EVENT_NOTIFICATION:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_EVENT_NOTIFICATION PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_GET_ALARM_SUMMARY:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_GET_ALARM_SUMMARY PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_GET_ENROLLMENT_SUMMARY:
				printf("\nSERVICE_CONFIRMED_GET_ENROLLMENT_SUMMARY PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_GET_EVENT_INFORMATION:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_GET_EVENT_INFORMATION PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_SUBSCRIBE_COV:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_SUBSCRIBE_COV PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_SUBSCRIBE_COV_PROPERTY:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_SUBSCRIBE_COV_PROPERTY type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_LIFE_SAFETY_OPERATION:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_LIFE_SAFETY_OPERATION PDU type: %X\n", *pdu);
			break;
			/* File Access Services */
		case SERVICE_CONFIRMED_ATOMIC_READ_FILE:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_ATOMIC_READ_FILE PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_ATOMIC_WRITE_FILE:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_ATOMIC_WRITE_FILE PDU type: %X\n", *pdu);
			break;
			/* Object Access Services */
		case SERVICE_CONFIRMED_ADD_LIST_ELEMENT:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_ADD_LIST_ELEMENT PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_REMOVE_LIST_ELEMENT:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_REMOVE_LIST_ELEMENT PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_CREATE_OBJECT:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_CREATE_OBJECT PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_DELETE_OBJECT:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_DELETE_OBJECT PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_READ_PROPERTY:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_READ_PROPERTY PDU type: %X\n", *pdu);
			confirmedService = new BacNetReadPropertyACK();

			((BacNetReadPropertyACK *)confirmedService)->decode_readProperty_ack(buffer, var);

			dev->release_invoke_id_to_var(var);
			dev->clear_retry_count();
			delete confirmedService;
			break;
		case SERVICE_CONFIRMED_READ_PROP_CONDITIONAL:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_READ_PROP_CONDITIONAL PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_READ_PROP_MULTIPLE:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_READ_PROP_MULTIPLE PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_READ_RANGE:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_READ_RANGE PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_WRITE_PROPERTY:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_WRITE_PROPERTY PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_WRITE_PROP_MULTIPLE:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_WRITE_PROP_MULTIPLE PDU type: %X\n", *pdu);
			break;
			/* Remote Device Management Services */
		case SERVICE_CONFIRMED_DEVICE_COMMUNICATION_CONTROL:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_DEVICE_COMMUNICATION_CONTROL PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_PRIVATE_TRANSFER:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_PRIVATE_TRANSFER PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_TEXT_MESSAGE:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_TEXT_MESSAGE PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_REINITIALIZE_DEVICE:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_REINITIALIZE_DEVICE PDU type: %X\n", *pdu);
			break;
			/* Virtual Terminal Services */
		case SERVICE_CONFIRMED_VT_OPEN:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_VT_OPEN PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_VT_CLOSE:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_VT_CLOSE PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_VT_DATA:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_VT_DATA PDU type: %X\n", *pdu);
			break;
			/* Security Services */
		case SERVICE_CONFIRMED_AUTHENTICATE:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_AUTHENTICATE PDU type: %X\n", *pdu);
			break;
		case SERVICE_CONFIRMED_REQUEST_KEY:
			if(verbosity_level_3(verbosity_level))
				printf("\nSERVICE_CONFIRMED_REQUEST_KEY PDU type: %X\n", *pdu);
			break;
		default:
			if(verbosity_level_3(verbosity_level))
				printf("\nComplex acknowledge PDU, unsupported SERVICE type: %X\n", *pdu);
			break;
		}
	}
	else {
		/** Wup!!! this variable is supposed to write... Guess I should ignore this read... Else it
		 * will clobber my value I'm writing...*/
		var->setIsValid(true);
		dev->release_invoke_id_to_var(var);
	}

	var->unlock();
	return buffer;
}
uint8_t BacNetComplexACK::get_pdu_type(){
	return this->pdu_type;
}
bool BacNetComplexACK::is_segmented(){
	return this->segmented_msg;
}
bool BacNetComplexACK::get_more_follows(){
	return this->more_follows;
}
uint8_t BacNetComplexACK::get_invoke_id(){
	return this->invoke_id;
}
uint8_t BacNetComplexACK::get_sequence_number(){
	return this->sequence_number;
}
uint8_t BacNetComplexACK::get_prop_window_size(){
	return this->prop_window_sized;
}
BACNET_CONFIRMED_SERVICE BacNetComplexACK::get_service_type(){
	return this->svc;
}




