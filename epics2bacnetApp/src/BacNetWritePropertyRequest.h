/*
 * BacNetWritePropertyRequest.h
 *
 *  Created on: Nov 16, 2011
 *      Author: 8w4
 */

#ifndef BACNETWRITEPROPERTYREQUEST_H_
#define BACNETWRITEPROPERTYREQUEST_H_

#include "BacNetRequest.h"
#include "BacNetConfirmedRequestPdu.h"
#include "bacenum.h"
#include "bacstructures.h"
#include "BacNetBuffer.h"
#include "BacNetDevice.h"
#include "BacNetVariable.h"

class BacNetWritePropertyRequest : public BacNetRequest, public BacNetConfirmedRequestPdu{
private:
	const BACNET_OBJECT_ID *bacnet_obj_id;
	const uint32_t *property_id;
	const int *property_array_index;
	const uint8_t *write_priority;

	BACNET_APPLICATION_TAG application_tag;
	const BACNET_APPLICATION_DATA_VALUE *application_value;

public:
	BacNetWritePropertyRequest();
	virtual ~BacNetWritePropertyRequest();

	BacNetWritePropertyRequest(const BACNET_OBJECT_ID *bac_obj_id, const uint32_t *prop_id,
			const int *prop_array_index, uint8_t invoke_id, const uint8_t *priority, const BACNET_APPLICATION_TAG tag, const BACNET_APPLICATION_DATA_VALUE *value);

	virtual BacNetBuffer *encode_request(BacNetBuffer *buffer);
	void set_invoke_id(uint8_t new_id);
};

#endif /* BACNETWRITEPROPERTYREQUEST_H_ */
