/*
 * ReadPropertyRequest.h
 *
 *  Created on: Aug 26, 2011
 *      Author: 8w4
 */

#ifndef READPROPERTYREQUEST_H_
#define READPROPERTYREQUEST_H_

#include "BacNetRequest.h"
#include "BacNetConfirmedRequestPdu.h"
#include "bacenum.h"
#include "bacstructures.h"
#include "BacNetBuffer.h"
#include "BacNetDevice.h"
#include "BacNetVariable.h"


class BacNetReadPropertyRequest : public BacNetRequest, public BacNetConfirmedRequestPdu{

private:

	const BACNET_OBJECT_ID *bacnet_obj_id;
	const uint32_t *property_id;
	const int *property_array_index;

public:
	BacNetReadPropertyRequest();
	virtual ~BacNetReadPropertyRequest();

	BacNetReadPropertyRequest(const BACNET_OBJECT_ID *bac_obj_id, const uint32_t *prop_id, const int *prop_array_index, uint8_t invoke_id);

	virtual BacNetBuffer *encode_request(BacNetBuffer *buffer);
	BacNetBuffer *encode_read_property(BacNetBuffer *buffer, BacNetDevice *dev, BacNetVariable *var, uint8_t invoke_id);
	void set_invoke_id(uint8_t new_id);

};

#endif /* READPROPERTYREQUEST_H_ */
