/*
 * BacNetPropertyValue.h
 *
 *  Created on: Dec 30, 2011
 *      Author: 8w4
 */
#include <stdio.h>
#include <stdint.h>

#include "bacenum.h"
#include "bacstructures.h"
#include "BacNetAbstractSyntaxAndType.h"

#ifndef BACNETPROPERTYVALUE_H_
#define BACNETPROPERTYVALUE_H_

class BacNetPropertyValue {
private:
	BACNET_PROPERTY_ID property_identifier;
	int property_array_index;
	class BacNetAbstractSyntaxAndType *value;
	int priority;
	class BacNetPropertyValue *next;

public:
	BacNetPropertyValue();
	virtual ~BacNetPropertyValue();

	BacNetPropertyValue(uint32_t property_id, int prop_array_index, BacNetAbstractSyntaxAndType *value_list, int priority, class BacNetPropertyValue *next);

    BACNET_PROPERTY_ID getProperty_identifier() const;

    void setProperty_identifier(BACNET_PROPERTY_ID property_identifier);

    int getProperty_array_index() const;

    void setProperty_array_index(int property_array_index);

    BacNetAbstractSyntaxAndType *getValue();

    void setValue(BacNetAbstractSyntaxAndType *value);

    int getPriority() const;

    void setPriority(int priority);

    void set_next(class BacNetPropertyValue *next);

    class BacNetPropertyValue *get_next(void);
};

#endif /* BACNETPROPERTYVALUE_H_ */
