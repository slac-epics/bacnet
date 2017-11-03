/*
 * BacNetResponse.h
 *
 *  Created on: Nov 11, 2011
 *      Author: 8w4
 */

#ifndef BACNETRESPONSE_H_
#define BACNETRESPONSE_H_

#include "Response.h"
#include "BacNetBVLC.h"
#include "BacNetNpdu.h"
#include "BacNetBuffer.h"
#include "BacNetDevice.h"


class BacNetResponse {
public:
	BacNetResponse();
	virtual ~BacNetResponse();

	virtual BacNetBuffer *decode_response(class BacNetBuffer *buffer);
	//virtual BacNetBuffer *decode_response(class BacNetBuffer *buffer, class BacNetDevice *dev) = 0;
};

#endif /* BACNETRESPONSE_H_ */
