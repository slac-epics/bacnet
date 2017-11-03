/*
 * BacNetVariableString.h
 *
 *  Created on: Nov 21, 2011
 *      Author: 8w4
 */

#ifndef BACNETVARIABLESTRING_H_
#define BACNETVARIABLESTRING_H_

#include <stdint.h>
#include <string.h>

#include "BacNetVariable.h"
#include "BacNetBuffer.h"
#include "common.h"

class BacNetVariableString: public BacNetVariable {
private:

public:
	BacNetVariableString();
	BacNetVariableString(uint16_t object_type, uint32_t object_instance, uint32_t property_id, int obj_index, uint8_t write_priority);

	virtual void setValue(BACNET_APPLICATION_DATA_VALUE &val);
	virtual void printValue(void);

	const char *getValue() const;
	void setValue(char *newValue);
	void write_value(char *newValue);
	virtual ~BacNetVariableString();
	virtual void toString();
};

#endif /* BACNETVARIABLESTRING_H_ */
