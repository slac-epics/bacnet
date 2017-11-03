/*
 * BacNetBVLC.cpp
 *
 *  Created on: Sep 9, 2011
 *      Author: 8w4
 */

#include "BacNetBVLC.h"
#include "BacNetBuffer.h"
#include "common.h"


BacNetBVLC::BacNetBVLC() {
	this->type = 0;
	this->function = 0;
	this->length = 0;
	this->result = 0;
	memset(this->forwardAddr, 0, 6);

}

BacNetBVLC::~BacNetBVLC() {
	// TODO Auto-generated destructor stub
}

BacNetBuffer *BacNetBVLC::decode_bvlc(BacNetBuffer *buffer){

	uint8_t *pdu = buffer->getPointerToBuffer();

	/** Load the TYPE portion of the message; might need it later...*/
	this->type = *(pdu++);

	/** find the write BVLC FUNCTION code...*/
	switch(*pdu) {
		case BVLC_RESULT:
		case BVLC_WRITE_BROADCAST_DISTRIBUTION_TABLE:
		case BVLC_READ_BROADCAST_DIST_TABLE:
		case BVLC_READ_BROADCAST_DIST_TABLE_ACK:
			if(verbosity_level_3(verbosity_level))
				printf("\nCan't handle BVLC: %X\n", *pdu);
			break;
		case BVLC_FORWARDED_NPDU:
			this->function = *(pdu++);
			pdu = buffer->decode_unsigned16(pdu, &this->length);
			memcpy(this->forwardAddr, pdu, 6);
			pdu += 6;
			buffer->set_currentPosition(pdu);

			break;
		case BVLC_REGISTER_FOREIGN_DEVICE:
		case BVLC_READ_FOREIGN_DEVICE_TABLE:
		case BVLC_READ_FOREIGN_DEVICE_TABLE_ACK:
		case BVLC_DELETE_FOREIGN_DEVICE_TABLE_ENTRY:
		case BVLC_DISTRIBUTE_BROADCAST_TO_NETWORK:
			if(verbosity_level_3(verbosity_level))
				printf("\nCan't handle BVLC: %X\n", *pdu);
			break;
		case BVLC_ORIGINAL_UNICAST_NPDU :
			if(verbosity_level_3(verbosity_level))
				printf("It is a BVLC_ORIGINAL_UNICAST_NPDU message...\n");
			this->function = *(pdu++);
			pdu = buffer->decode_unsigned16(pdu, &this->length);
			buffer->set_currentPosition(pdu);
			break;

		case BVLC_ORIGINAL_BROADCAST_NPDU:
			if(verbosity_level_3(verbosity_level))
				printf("It is a BVLC_ORIGINAL_BROADCAST_NPDU message...\n");
			this->function = *(pdu++);
			pdu = buffer->decode_unsigned16(pdu, &this->length);
			buffer->set_currentPosition(pdu);
			break;
		default :
			if(verbosity_level_3(verbosity_level))
				printf("Message is not a UNICAST or BROADCAST, so I don't know what to do...\n");

		}
	return buffer;
}

BacNetBuffer *BacNetBVLC::decode_bvlc_forwardNpdu(BacNetBuffer *buffer){

	uint8_t *ptr = buffer->getPointerToBuffer();
	this->type = *(ptr++);
	this->function = *(ptr++);
	ptr = buffer->decode_unsigned16(ptr, &length);
	memcpy(this->forwardAddr, ptr, 6);

	return buffer;
}

BacNetBuffer *BacNetBVLC::encode_bvlc_originalBroadcastNpdu(BacNetBuffer *buffer){

	uint8_t tempPdu[buffer->getLength()];
	memcpy(tempPdu, buffer->getPointerToBuffer(), buffer->getLength());

	uint8_t *pdu = buffer->getPointerToBuffer();
	*(pdu++) = 0x81;	/** BACnet IP*/
	*(pdu++) = 0x0B;	/** Original Broadcast.*/

	pdu = buffer->encode_unsigned16(pdu, 4+sizeof(tempPdu));

	memcpy(pdu, tempPdu, sizeof(tempPdu));

	buffer->set_length(pdu+sizeof(tempPdu));

	return buffer;
}

BacNetBuffer *BacNetBVLC::encode_bvlc_originalUnicastNpdu(BacNetBuffer *buffer){

	uint8_t tempPdu[buffer->getLength()];
	memcpy(tempPdu, buffer->getPointerToBuffer(), buffer->getLength());

	uint8_t *pdu = buffer->getPointerToBuffer();
	*(pdu++) = 0x81;	/** BACnet IP*/
	*(pdu++) = 0x0A;	/** Original Unicast.*/

	pdu = buffer->encode_unsigned16(pdu, 4+sizeof(tempPdu));

	memcpy(pdu, tempPdu, sizeof(tempPdu));

	buffer->set_length(pdu+sizeof(tempPdu));

	return buffer;
}

uint8_t BacNetBVLC::get_bvlc_type() {
	return type;
}
uint8_t BacNetBVLC::get_bvlc_function() {
	return function;
}
uint16_t BacNetBVLC::get_bvlc_length() {
	return length;
}
uint8_t BacNetBVLC::get_bvlc_result() {
	return result;
}

uint8_t *BacNetBVLC::get_bvlc_forward_addr(){
	return this->forwardAddr;
}
