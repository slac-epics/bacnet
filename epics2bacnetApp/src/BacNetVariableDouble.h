/*
 * BacNetVariableDouble.h
 *
 *  Created on: Aug 19, 2011
 *      Author: 8w4
 */

#ifndef BACNETVARIABLEDOUBLE_H_
#define BACNETVARIABLEDOUBLE_H_

#include <stdint.h>

#include "BacNetVariable.h"
#include "BacNetCOV.h"
#include "BacNetBuffer.h"
#include "common.h"

class BacNetVariableDouble: public BacNetVariable {
private:
	//static BACNET_APPLICATION_TAG calcDataType(uint16_t objecType);
public:
	BacNetVariableDouble();
	BacNetVariableDouble(uint16_t object_type, uint32_t object_instance, uint32_t property_id, int obj_index, uint8_t write_priority,
			class BacNetCOV *cov);

	virtual void setValue(BACNET_APPLICATION_DATA_VALUE &val);
	virtual void printValue(void);

	double getValue();
	void setValue(double newValue);
	void write_value(double newValue);
	virtual ~BacNetVariableDouble();
	virtual void toString();
};

#endif /* BACNETVARIABLEDOUBLE_H_ */
