/*
 * BacNetUnconfirmedRequestPdu.h
 *
 *  Created on: Sep 2, 2011
 *      Author: 8w4
 */

#ifndef BACNETUNCONFIRMEDREQUESTPDU_H_
#define BACNETUNCONFIRMEDREQUESTPDU_H_

#include <stdio.h>
#include <stdint.h>
#include "bacenum.h"
#include "Response.h"
#include "BacNetResponse.h"
#include "BacNetBuffer.h"
#include "BacNetDevice.h"
#include "BacNet_I_Am_Request.h"

class BacNetUnconfirmedRequestPdu : public BacNetResponse{

private:

public:
	BacNetUnconfirmedRequestPdu();
	//BacNetUnconfirmedRequestPdu(class Response *resp);
	virtual ~BacNetUnconfirmedRequestPdu();

	BacNetBuffer *decode_unconfirmed_request(class BacNetBuffer *buffer, sockaddr_in *addr, class BacNetBVLC *bvlc,
			class BacNetNpdu *npdu, class BacNetDevice *dev);
	//virtual void decode_unconfirmed_request(class BacNetBuffer *buffer);
};

#endif /* BACNETUNCONFIRMEDREQUESTPDU_H_ */
