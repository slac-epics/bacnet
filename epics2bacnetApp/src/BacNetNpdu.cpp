/*
 * BacNetNpdu.cpp
 *
 *  Created on: Sep 9, 2011
 *      Author: 8w4
 */
#include <stdio.h>
#include <string.h>

#include "BacNetNpdu.h"
#include "BacNetBuffer.h"
#include "common.h"

const uint8_t VERSION = 0x01;

BacNetNpdu::BacNetNpdu()
:version(VERSION)
{

	this->cntrl.priority = 0x00;
	this->cntrl.expectingReply = 0;
	this->cntrl.sourceSpec = 0;
	this->cntrl.reserved1 = 0;
	this->cntrl.destSpec = 0;
	this->cntrl.reserved2 = 0;
	this->cntrl.nsdu = 0;
	this->dnet = 0x00;
	this->dlen = 0;
	memset(this->dadr, 0, MAX_DLEN);
	this->snet = 0x00;
	this->slen = 0;
	memset(this->sadr, 0, MAX_DLEN);
	this->hop = 0xFE;
	this->msgType = 0;
	this->venId = 0x00;
}

BacNetNpdu::~BacNetNpdu() {
	// TODO Auto-generated destructor stub
}

BacNetBuffer *BacNetNpdu::decode_npdu(BacNetBuffer *buffer) {

	uint8_t *pdu = buffer->get_currentPosition();

	/** version is a constant...*/
	uint8_t v = *(pdu++);
	if(v != this->version){
		if(verbosity_level_3(verbosity_level))
			printf("\nVersion difference! local: %u, Remote: %u\n", this->version, v);
	}

	/** bits 0 & 1*/
	this->cntrl.priority = (*pdu & 0x03) ? 1 : 0;
	/** bit 2*/
	this->cntrl.expectingReply = (*pdu & 0x04) ? 1 : 0;
	/** bit 3*/
	this->cntrl.sourceSpec = (*pdu & 0x08) ? 1 : 0;
	/** bit 4*/
	this->cntrl.reserved1 = (*pdu & 0x10) ? 1 : 0;
	/** bit 5*/
	this->cntrl.destSpec = (*pdu & 0x20) ? 1 : 0;
	/** bit 6*/
	this->cntrl.reserved2 = (*pdu & 0x40) ? 1 : 0;
	/** bit 7*/
	this->cntrl.nsdu = (*pdu & 0x80) ? 1 : 0;

	(++pdu);

	/** Is there a destination specifier?*/
	if(this->cntrl.destSpec)
	{
		pdu = buffer->decode_unsigned16(pdu, &dnet);
		this->dlen = *(pdu++);
		if(this->dlen > 0) {
			pdu = buffer->decode_var_length(dadr, pdu, dlen);
		}
		else {
			memset(dadr, 0, 6);
		}

	}
	/** Guess not...*/
	else
	{
		this->dnet = 0;
		this->dlen = 0;
		memset(dadr, 0, 6);
		this->hop = 0;
	}

	/** Is there a source specifier?*/
	if(this->cntrl.sourceSpec)
	{
		pdu = buffer->decode_unsigned16(pdu, &snet);
		this->slen = *(pdu++);
		if(this->slen > 0) {
			pdu = buffer->decode_var_length(sadr, pdu, slen);
		}
		else {
			memset(sadr, 0, 6);
		}

	}
	/** Guess not...*/
	else
	{
		this->snet = 0;
		this->slen = 0;
		memset(sadr, 0, 6);
	}

	/** Load hop count if present...*/
	if(this->cntrl.destSpec) {
		this->hop = *(pdu++);
	}

	if(this->cntrl.nsdu) {
		this->msgType = *(pdu++);
		if(verbosity_level_3(verbosity_level))
			printf("\n**** NSDU message: %X ****\n", this->msgType);
		if(this->msgType >= 0x80) {
			pdu = buffer->decode_unsigned16(pdu, &venId);
			if(verbosity_level_3(verbosity_level))
				printf("\n**** Vendor ID message: %02X ****\n", this->venId);
		}
	}

	buffer->set_currentPosition(pdu);

	return buffer;
}

void BacNetNpdu::npdu_to_string() {
	printf("\n");

	printf("**** NPDU ****\n");
	printf("\tVersion: %X\n", version);
	printf("\tNetwork priority: %X\n", cntrl.priority);
	printf("\tExpecting reply: %X\n", cntrl.expectingReply);
	printf("\tSource specifier: %X\n", cntrl.sourceSpec);
	if(cntrl.sourceSpec) {
		printf("\tSNET: %d, SLEN: %d, SADR: ", snet, slen);
		int i;
		for(i=0; i<MAX_SLEN; i++) {
			if(i<5)
				printf("%X:", sadr[i]);
			else
				printf("%X\n", sadr[i]);
		}
	}
	printf("\tReserved1: %X\n", cntrl.reserved1);
	printf("\tDestination specifier: %X\n", cntrl.destSpec);
	if(cntrl.destSpec) {
		printf("\tDNET: %d, DLEN: %d, DADR: ", dnet, dlen);
		int i;
		for(i=0; i<MAX_DLEN; i++) {
			if(i<5)
				printf("%02X:", dadr[i]);
			else
				printf("%02X\n", dadr[i]);
		}
	}
	printf("\tReserved2: %X\n", cntrl.reserved2);
	printf("\tNSDU: %X\n", cntrl.nsdu);
	printf("\tHop: %X\n", this->hop);

	printf("**** END NPDU ****\n");
}

void BacNetNpdu::set_npdu_destination_specifier(bool bit){
	this->cntrl.destSpec = bit;
}

void BacNetNpdu::set_npdu_dnet(uint16_t net){
	this->dnet = net;
}

void BacNetNpdu::set_npdu_dlen(uint8_t len){
	this->dlen = len;
}

void BacNetNpdu::set_npdu_dadr(uint8_t *daddr){
	memcpy(this->dadr, daddr, this->dlen);
}
void BacNetNpdu::set_npdu_expecting_reply(bool bit){
	this->cntrl.expectingReply = bit;
}

BacNetBuffer *BacNetNpdu::encode_npdu(BacNetBuffer *buffer){

	uint8_t temp_apdu[buffer->getLength()];
	memcpy(temp_apdu, buffer->getPointerToBuffer(), buffer->getLength());

	uint8_t *ptrNPDU = buffer->getPointerToBuffer();

	/** Set the version octet...*/
	*(ptrNPDU++) = this->version;

	/** Set all the bits for the control octet...*/
	*ptrNPDU = (this->cntrl.priority & 0x03);

	if(this->cntrl.expectingReply)
		*ptrNPDU |= 0x04;

	if(this->cntrl.sourceSpec)
		*ptrNPDU |= 0x08;

	/** Reserved bit cleared...*/
	*ptrNPDU &= ~0x10;

	if(this->cntrl.destSpec)
		*ptrNPDU |= 0x20;

	/** Reserved bit cleared...*/
	*ptrNPDU &= ~0x40;

	if(this->cntrl.nsdu)
		*ptrNPDU |= 0x80;

	/** move to DNET octet...*/
	ptrNPDU++;

	if(this->cntrl.destSpec){
		if(this->dlen == 0){
			ptrNPDU = buffer->encode_unsigned16(ptrNPDU, 0xFFFF);
			*(ptrNPDU++) = this->dlen;
		}
		else{
			ptrNPDU = buffer->encode_unsigned16(ptrNPDU, this->dnet);
			*(ptrNPDU++) = this->dlen;
			memcpy(ptrNPDU, this->dadr, this->dlen);
			ptrNPDU += this->dlen;
		}
	}

	if(this->cntrl.sourceSpec){
		ptrNPDU = buffer->encode_unsigned16(ptrNPDU, this->snet);
		*(ptrNPDU++) = this->slen;
		memcpy(ptrNPDU, this->sadr, this->slen);
		ptrNPDU++;
	}

	if(this->cntrl.destSpec){
		*(ptrNPDU++) = this->hop;
	}

	if(this->cntrl.nsdu){
		*(ptrNPDU++) = this->msgType;
		if(this->msgType > 0x80){
			ptrNPDU = buffer->encode_unsigned16(ptrNPDU, this->venId);
		}
	}

	buffer->set_length(ptrNPDU);

	/** Now put in order...*/
	memcpy(ptrNPDU, temp_apdu, sizeof(temp_apdu));

	buffer->set_length(ptrNPDU+sizeof(temp_apdu));

	return buffer;
}

uint16_t BacNetNpdu::get_npdu_snet(){
	return this->snet;
}

uint8_t BacNetNpdu::get_npdu_slen(){
	return this->slen;
}

uint8_t *BacNetNpdu::get_npdu_sadr(){
	return this->sadr;
}

bool BacNetNpdu::get_npdu_source_specifier(){
	return this->cntrl.sourceSpec;
}

