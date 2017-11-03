/*
 * BacNetBVLC.h
 *
 *  Created on: Sep 9, 2011
 *      Author: 8w4
 */

#ifndef BACNETBVLC_H_
#define BACNETBVLC_H_

#include <stdint.h>
#include <string>

#include "bacenum.h"
#include "BacNetBuffer.h"

class BacNetBVLC {
private:
	uint8_t type;
	uint8_t function;
	uint16_t length;
	uint8_t result;
	uint8_t forwardAddr[6];
public:
	BacNetBVLC();
	virtual ~BacNetBVLC();
	BacNetBuffer *encode_bvlc_originalBroadcastNpdu(BacNetBuffer *buffer);
	BacNetBuffer *encode_bvlc_originalUnicastNpdu(BacNetBuffer *buffer);
	BacNetBuffer *decode_bvlc_forwardNpdu(BacNetBuffer *buffer);
	BacNetBuffer *decode_bvlc(BacNetBuffer *buffer);
	uint8_t get_bvlc_type();
	uint8_t get_bvlc_function();
	uint16_t get_bvlc_length();
	uint8_t get_bvlc_result();
	uint8_t *get_bvlc_forward_addr();
};

#endif /* BACNETBVLC_H_ */
