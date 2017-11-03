/*
 * BacNetError.cpp
 *
 *  Created on: Nov 18, 2011
 *      Author: 8w4
 */

#include "BacNetError.h"
#include "BacNetConfirmedService.h"
#include "BacNetVariable.h"

BacNetError::BacNetError() {
	this->error_class = (BACNET_ERROR_CLASS) 0;
	this->error_code = (BACNET_ERROR_CODE) 0;
	this->pdu_type = PDU_TYPE_ERROR;
	this->original_invoke_id = 0;
	this->svc = (BACNET_CONFIRMED_SERVICE) 0;
}

BacNetError::~BacNetError() {
	// TODO Auto-generated destructor stub
}

BacNetBuffer *BacNetError::decode_bacnet_error(class BacNetBuffer *buffer) {
	/** Decode something...*/
	uint8_t *pdu = buffer->get_currentPosition();
	this->pdu_type = (*(pdu++) & 0xF0);
	this->original_invoke_id = *(pdu++);
	this->svc = (BACNET_CONFIRMED_SERVICE) *(pdu++);

	buffer->set_currentPosition(pdu);
	return buffer;
}

BacNetBuffer *BacNetError::decode_error_class(class BacNetBuffer *buffer) {
	uint8_t *pdu = buffer->get_currentPosition();

	uint8_t tagNumber = 0;
	uint8_t tagClass = 0;
	uint32_t tagLenValType = 0;

	pdu = buffer->decode_bacnet_tag(pdu, &tagNumber, &tagClass, &tagLenValType);

	if(tagNumber != 9 || tagClass != 0){
		printf("\nBad packet! Should be BACNET_APPLICATION_TAG_ENUMERATION: %u\n", tagLenValType);
		return NULL;
	}

	if(tagLenValType >= 4)
		pdu = buffer->decode_unsigned32(pdu, (uint32_t *)&this->error_class);
	else if(tagLenValType >= 3)
		pdu = buffer->decode_unsigned24(pdu, (uint32_t *)&this->error_class);
	else if(tagLenValType >= 2)
		pdu = buffer->decode_unsigned16(pdu, (uint16_t *)&this->error_class);
	else
		this->error_class = (BACNET_ERROR_CLASS)*(pdu++);
	printf("\nthis->error_class: %u\n", this->error_class);
	buffer->set_currentPosition(pdu);

	return buffer;
}

BacNetBuffer *BacNetError::decode_error_code(class BacNetBuffer *buffer) {
	uint8_t *pdu = buffer->get_currentPosition();

	uint8_t tagNumber = 0;
	uint8_t tagClass = 0;
	uint32_t tagLenValType = 0;

	pdu = buffer->decode_bacnet_tag(pdu, &tagNumber, &tagClass, &tagLenValType);

	if(tagNumber != 9 || tagClass != 0){
		printf("\nBad packet! Should be BACNET_APPLICATION_TAG_ENUMERATION: %u\n", tagLenValType);
		return NULL;
	}

	if(tagLenValType >= 4)
		pdu = buffer->decode_unsigned32(pdu, (uint32_t *)&this->error_code);
	else if(tagLenValType >= 3)
		pdu = buffer->decode_unsigned24(pdu, (uint32_t *)&this->error_code);
	else if(tagLenValType >= 2)
		pdu = buffer->decode_unsigned16(pdu, (uint16_t *)&this->error_code);
	else
		this->error_code = (BACNET_ERROR_CODE)*(pdu++);
	printf("\nthis->error_code: %u\n", this->error_code);
	buffer->set_currentPosition(pdu);
	return buffer;
}

BacNetBuffer *BacNetError::decode_bacnet_error(class BacNetBuffer *buffer, sockaddr_in *addr, class BacNetBVLC *bvlc,
		class BacNetNpdu *npdu, class BacNetDevice *dev) {

	decode_bacnet_error(buffer);

	BacNetVariable *var = dev->getBacNetvariableByInvokeId(this->original_invoke_id);

	if(!var){
		if(verbosity_level_1(verbosity_level))
			printf("\nVar is invalid. this->invoke_id = %u\n", this->original_invoke_id);
		return buffer;
	}

	//uint8_t *pdu = buffer->get_currentPosition();

	switch(this->svc){
	/* Alarm and Event Services */
	case SERVICE_CONFIRMED_ACKNOWLEDGE_ALARM:
		printf("\nSERVICE_CONFIRMED_ACKNOWLEDGE_ALARM PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_COV_NOTIFICATION:
		printf("\nSERVICE_CONFIRMED_COV_NOTIFICATION PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_EVENT_NOTIFICATION:
		printf("\nSERVICE_CONFIRMED_EVENT_NOTIFICATION PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_GET_ALARM_SUMMARY:
		printf("\nSERVICE_CONFIRMED_GET_ALARM_SUMMARY PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_GET_ENROLLMENT_SUMMARY:
		printf("\nSERVICE_CONFIRMED_GET_ENROLLMENT_SUMMARY PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_GET_EVENT_INFORMATION:
		printf("\nSERVICE_CONFIRMED_GET_EVENT_INFORMATION PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_SUBSCRIBE_COV:
		printf("\nSERVICE_CONFIRMED_SUBSCRIBE_COV PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_SUBSCRIBE_COV_PROPERTY:
		printf("\nSERVICE_CONFIRMED_SUBSCRIBE_COV_PROPERTY type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_LIFE_SAFETY_OPERATION:
		printf("\nSERVICE_CONFIRMED_LIFE_SAFETY_OPERATION PDU type: %X\n", this->svc);
		break;
		/* File Access Services */
	case SERVICE_CONFIRMED_ATOMIC_READ_FILE:
		printf("\nSERVICE_CONFIRMED_ATOMIC_READ_FILE PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_ATOMIC_WRITE_FILE:
		printf("\nSERVICE_CONFIRMED_ATOMIC_WRITE_FILE PDU type: %X\n", this->svc);
		break;
		/* Object Access Services */
	case SERVICE_CONFIRMED_ADD_LIST_ELEMENT:
		printf("\nSERVICE_CONFIRMED_ADD_LIST_ELEMENT PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_REMOVE_LIST_ELEMENT:
		printf("\nSERVICE_CONFIRMED_REMOVE_LIST_ELEMENT PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_CREATE_OBJECT:
		printf("\nSERVICE_CONFIRMED_CREATE_OBJECT PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_DELETE_OBJECT:
		printf("\nSERVICE_CONFIRMED_DELETE_OBJECT PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_READ_PROPERTY:
		printf("\nSERVICE_CONFIRMED_READ_PROPERTY (ERROR) PDU type: %X\n", this->svc);
		buffer = decode_error_class(buffer);
		buffer = decode_error_code(buffer);

		var->setIsValid(false);
		dev->release_invoke_id_to_var(var);

		printf("\nVariable(%u, %u) is invalid (Error class: %u, Error code: %u)\n", var->getObjectType(), var->getObject_instance(),
				this->error_class, this->error_code);
		break;
	case SERVICE_CONFIRMED_READ_PROP_CONDITIONAL:
		printf("\nSERVICE_CONFIRMED_READ_PROP_CONDITIONAL PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_READ_PROP_MULTIPLE:
		printf("\nSERVICE_CONFIRMED_READ_PROP_MULTIPLE PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_READ_RANGE:
		printf("\nSERVICE_CONFIRMED_READ_RANGE PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_WRITE_PROPERTY:
		printf("\nSERVICE_CONFIRMED_WRITE_PROPERTY PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_WRITE_PROP_MULTIPLE:
		printf("\nSERVICE_CONFIRMED_WRITE_PROP_MULTIPLE PDU type: %X\n", this->svc);
		break;
		/* Remote Device Management Services */
	case SERVICE_CONFIRMED_DEVICE_COMMUNICATION_CONTROL:
		printf("\nSERVICE_CONFIRMED_DEVICE_COMMUNICATION_CONTROL PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_PRIVATE_TRANSFER:
		printf("\nSERVICE_CONFIRMED_PRIVATE_TRANSFER PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_TEXT_MESSAGE:
		printf("\nSERVICE_CONFIRMED_TEXT_MESSAGE PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_REINITIALIZE_DEVICE:
		printf("\nSERVICE_CONFIRMED_REINITIALIZE_DEVICE PDU type: %X\n", this->svc);
		break;
		/* Virtual Terminal Services */
	case SERVICE_CONFIRMED_VT_OPEN:
		printf("\nSERVICE_CONFIRMED_VT_OPEN PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_VT_CLOSE:
		printf("\nSERVICE_CONFIRMED_VT_CLOSE PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_VT_DATA:
		printf("\nSERVICE_CONFIRMED_VT_DATA PDU type: %X\n", this->svc);
		break;
		/* Security Services */
	case SERVICE_CONFIRMED_AUTHENTICATE:
		printf("\nSERVICE_CONFIRMED_AUTHENTICATE PDU type: %X\n", this->svc);
		break;
	case SERVICE_CONFIRMED_REQUEST_KEY:
		printf("\nSERVICE_CONFIRMED_REQUEST_KEY PDU type: %X\n", this->svc);
		break;
	default:
		printf("\nPDU_TYPE_ERROR PDU, unsupported SERVICE type: %X\n", this->svc);
		break;
	}
	return buffer;
}
