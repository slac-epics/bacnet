/*
 * BacNetSubscribeCovRequest.h
 *
 *  Created on: Dec 21, 2011
 *      Author: 8w4
 */

#ifndef BACNETSUBSCRIBECOVREQUEST_H_
#define BACNETSUBSCRIBECOVREQUEST_H_

#include <stdint.h>
#include "bacenum.h"
#include "bacstructures.h"
#include "BacNetBuffer.h"
#include "BacNetRequest.h"
#include "BacNetConfirmedRequestPdu.h"

class BacNetSubscribeCovRequest : public BacNetRequest, public BacNetConfirmedRequestPdu{
private:

	const uint32_t process_id;
	const BACNET_OBJECT_ID bac_obj_id;
	const bool confirmed_notifications;
	const unsigned lifetime;

public:
	BacNetSubscribeCovRequest(const uint32_t process_id, const BACNET_OBJECT_ID bac_obj_id,
			const bool confirmed_notifications, const unsigned lifetime, uint8_t invoke_id);
	virtual ~BacNetSubscribeCovRequest();
	virtual BacNetBuffer *encode_request(BacNetBuffer *buffer);
};

#endif /* BACNETSUBSCRIBECOVREQUEST_H_ */
