/*
 * BacNetSimpleACK.h
 *
 *  Created on: Nov 16, 2011
 *      Author: 8w4
 */

#ifndef BACNETSIMPLEACK_H_
#define BACNETSIMPLEACK_H_

#include <stdint.h>
#include "BacNetResponse.h"
#include "BacNetConfirmedService.h"
#include "bacenum.h"
#include "BacNetBuffer.h"
#include "BacNetBVLC.h"
#include "BacNetNpdu.h"
#include "BacNetDevice.h"
#include "BacNetVariable.h"
#include "BacNetReadPropertyACK.h"

class BacNetSimpleACK : public BacNetResponse {
private:
	const uint8_t pdu_type;
	uint8_t invoke_id;
	BACNET_CONFIRMED_SERVICE svc;

public:
	BacNetSimpleACK();
	BacNetSimpleACK(uint8_t invoke_id, BACNET_CONFIRMED_SERVICE service);
	virtual ~BacNetSimpleACK();

	BacNetBuffer *encode_simple_ack(class BacNetBuffer *buffer, class BacNetDevice *dev);
	BacNetBuffer *decode_simple_ack(class BacNetBuffer *buffer);
	BacNetBuffer *decode_simple_ack(class BacNetBuffer *buffer, sockaddr_in *addr, class BacNetBVLC *bvlc,
			class BacNetNpdu *npdu, class BacNetDevice *dev);
};

#endif /* BACNETSIMPLEACK_H_ */
