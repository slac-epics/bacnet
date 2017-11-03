/*
 * BacNetReadPropertyACK.h
 *
 *  Created on: Sep 26, 2011
 *      Author: 8w4
 */

#ifndef BACNETREADPROPERTYACK_H_
#define BACNETREADPROPERTYACK_H_

#include <stdint.h>

#include "BacNetConfirmedService.h"
#include "bacenum.h"
#include "bacstructures.h"
#include "BacNetBuffer.h"


class BacNetReadPropertyACK : public BacNetConfirmedService{
private:
	BACNET_OBJECT_ID object_id;
	BACNET_PROPERTY_ID property_id;
	int property_array_index;


public:
	BacNetReadPropertyACK();
	virtual ~BacNetReadPropertyACK();
	BacNetBuffer *decode_readProperty_ack(BacNetBuffer *buffer, BacNetVariable *var);
};

#endif /* BACNETREADPROPERTYACK_H_ */
