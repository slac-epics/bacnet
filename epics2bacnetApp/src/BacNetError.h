/*
 * BacNetError.h
 *
 *  Created on: Nov 18, 2011
 *      Author: 8w4
 */

#ifndef BACNETERROR_H_
#define BACNETERROR_H_

#include "bacenum.h"
#include "BacNetResponse.h"
#include "BacNetBuffer.h"
#include "BacNetBVLC.h"
#include "BacNetNpdu.h"
#include "BacNetDevice.h"

class BacNetError: public BacNetResponse {
private:
	uint8_t pdu_type;
	uint8_t original_invoke_id;
	BACNET_CONFIRMED_SERVICE svc;
	BACNET_ERROR_CLASS error_class;
	BACNET_ERROR_CODE error_code;

	BacNetBuffer *decode_error_class(BacNetBuffer *buffer);
	BacNetBuffer *decode_error_code(BacNetBuffer *buffer);

public:
	BacNetError();
	virtual ~BacNetError();

	BacNetBuffer *decode_bacnet_error(class BacNetBuffer *buffer);
	BacNetBuffer *decode_bacnet_error(class BacNetBuffer *buffer, sockaddr_in *addr, class BacNetBVLC *bvlc,
			class BacNetNpdu *npdu, class BacNetDevice *dev);
};

#endif /* BACNETERROR_H_ */
