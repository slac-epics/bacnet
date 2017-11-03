/*
 * BacNetWhoIsRequest.h
 *
 *  Created on: Aug 26, 2011
 *      Author: 8w4
 */

#ifndef BACNETWHOISREQUEST_H_
#define BACNETWHOISREQUEST_H_

#include <stdint.h>

#include "BacNetRequest.h"
#include "BacNetDevice.h"
#include "BacNetBuffer.h"



class BacNetWhoIsRequest : public BacNetRequest{

private:
	uint32_t deviceInstanceRangeLowLimit;
	uint32_t deviceInstanceRangeHighLimit;

public:
	BacNetWhoIsRequest(uint32_t highRange, uint32_t lowRange);
	virtual ~BacNetWhoIsRequest();

	BacNetBuffer *encode_request(BacNetBuffer *buffer);
	BacNetBuffer *encode_whoIs_request(BacNetBuffer *buffer);
};

#endif /* BACNETWHOISREQUEST_H_ */
