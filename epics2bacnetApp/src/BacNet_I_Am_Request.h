/*
 * BacNet_I_Am_Request.h
 *
 *  Created on: Sep 12, 2011
 *      Author: 8w4
 */

#ifndef BACNET_I_AM_REQUEST_H_
#define BACNET_I_AM_REQUEST_H_

#include <stdint.h>

//#include "BacNetUnconfirmedRequestPdu.h"
#include "bacenum.h"
#include "bacstructures.h"
#include "BacNetBuffer.h"

class BacNet_I_Am_Request {
private:
	BACNET_OBJECT_ID bacnet_object_id;
	uint32_t max_apdu_length_accepted;
	BACNET_SEGMENTATION segmentation_supported;
	uint32_t vendor_id;
public:
	BacNet_I_Am_Request();
	virtual ~BacNet_I_Am_Request();

	BacNetBuffer *decode_iam_request(BacNetBuffer *buffer);
	BacNetBuffer *decode_response(class BacNetBuffer *buffer);
	virtual void decode_unconfirmed_request(class BacNetBuffer *buffer);
	uint16_t get_object_type() const;
	uint32_t get_object_instance() const;
	uint32_t get_max_apdu_length() const;
	uint8_t get_segmentation_supported() const;
	uint32_t get_vendor_id() const;
};

#endif /* BACNET_I_AM_REQUEST_H_ */
