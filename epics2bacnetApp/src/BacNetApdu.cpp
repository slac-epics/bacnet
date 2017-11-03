/*
 * BacNetApdu.cpp
 *
 *  Created on: Aug 26, 2011
 *      Author: 8w4
 */

#include "BacNetApdu.h"

BacNetApdu::BacNetApdu() {
}


BacNetApdu::~BacNetApdu() {
	// TODO Auto-generated destructor stub
}

BACNET_PDU_TYPE BacNetApdu::get_pdu_type(){
	return this->pdu_type;
}
void BacNetApdu::set_pdu_type(BACNET_PDU_TYPE type){
	this->pdu_type = type;
}

BacNetBuffer *BacNetApdu::decode_apdu(BacNetBuffer *buffer){

	return buffer;
}
