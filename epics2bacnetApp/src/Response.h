/*
 * Response.h
 *
 *  Created on: Nov 14, 2011
 *      Author: 8w4
 */

#ifndef RESPONSE_H_
#define RESPONSE_H_

#include "BacNetBuffer.h"

class Response {
public:
	Response();
	virtual ~Response();
	virtual BacNetBuffer *decode_response(class BacNetBuffer *buffer) = 0;
};

#endif /* RESPONSE_H_ */
