/*
 * BacNetNpdu.h
 *
 *  Created on: Sep 9, 2011
 *      Author: 8w4
 */

#ifndef BACNETNPDU_H_
#define BACNETNPDU_H_

#include <stdint.h>

#include "BacNetDevice.h"
#include "BacNetBuffer.h"

#define MAX_DLEN 6
#define MAX_SLEN 6


struct control {
	unsigned priority : 2;	/** 2 bits 0-3*/
	unsigned expectingReply : 1;	/** 1 bit 0-1*/
	unsigned sourceSpec : 1;	/** 1 bit 0-1*/
	unsigned reserved1 : 1;	/** 1 bit 0-1*/
	unsigned destSpec : 1;	/** 1 bit 0-1*/
	unsigned reserved2 : 1;	/** 1 bit 0-1*/
	unsigned nsdu : 1;	/** 1 bit 0-1*/
};

class BacNetNpdu {
private:
	uint8_t version;
	control cntrl;
	uint16_t dnet;
	uint8_t dlen;
	uint8_t dadr[MAX_DLEN];
	uint16_t snet;
	uint8_t slen;
	uint8_t sadr[MAX_SLEN];
	uint8_t hop;
	uint8_t msgType;
	uint16_t venId;


public:
	BacNetNpdu();
	virtual ~BacNetNpdu();
	BacNetBuffer *decode_npdu(BacNetBuffer *buffer);
	BacNetBuffer *encode_npdu(BacNetBuffer *buffer);

	void set_npdu_destination_specifier(bool bit);
	void set_npdu_expecting_reply(bool bit);
	void set_npdu_dnet(uint16_t net);
	void set_npdu_dlen(uint8_t len);
	void set_npdu_dadr(uint8_t *daddr);

	void npdu_to_string();
	uint16_t get_npdu_snet();
	uint8_t get_npdu_slen();
	uint8_t *get_npdu_sadr();
	bool get_npdu_source_specifier();
};

#endif /* BACNETNPDU_H_ */
