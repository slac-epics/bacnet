/*
 * BacNetVariableDigital.h
 *
 *  Created on: Nov 15, 2011
 *      Author: 8w4
 */

#ifndef BACNETVARIABLEDIGITAL_H_
#define BACNETVARIABLEDIGITAL_H_

#include <stdint.h>

#include "BacNetVariable.h"
#include "BacNetCOV.h"
#include "BacNetBuffer.h"
#include "common.h"

class BacNetVariableDigital: public BacNetVariable {
private:

public:
	BacNetVariableDigital();
	BacNetVariableDigital(uint16_t object_type, uint32_t object_instance, uint32_t property_id, int obj_index, uint8_t write_priority,
			class BacNetCOV *cov);

	virtual void setValue(BACNET_APPLICATION_DATA_VALUE &val);
	virtual void printValue(void);

	unsigned long getValue();
	void setValue(unsigned long newValue);
	void write_value(unsigned long newValue);
	virtual ~BacNetVariableDigital();
	virtual void toString();
};

#endif /* BACNETVARIABLEDIGITAL_H_ */
