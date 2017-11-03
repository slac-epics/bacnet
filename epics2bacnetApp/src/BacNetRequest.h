/*
 * BacNetRequest.h
 *
 *  Created on: Oct 31, 2011
 *      Author: 8w4
 */

#ifndef BACNETREQUEST_H_
#define BACNETREQUEST_H_

#include "BacNetBuffer.h"

class BacNetRequest {
private:
public:
	BacNetRequest();
	virtual ~BacNetRequest();

	virtual BacNetBuffer *encode_request(BacNetBuffer *buffer) =0;
};

#endif /* BACNETREQUEST_H_ */
