/*
 * BacNetAbstractSyntaxAndType.h
 *
 *  Created on: Jan 3, 2012
 *      Author: 8w4
 */

#ifndef BACNETABSTRACTSYNTAXANDTYPE_H_
#define BACNETABSTRACTSYNTAXANDTYPE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bacenum.h"
#include "bacstructures.h"

class BacNetAbstractSyntaxAndType {
private:
	BACNET_APPLICATION_DATA_VALUE abstract_value;
	BACNET_APPLICATION_TAG abstract_type;
	class BacNetAbstractSyntaxAndType *next;
public:
	BacNetAbstractSyntaxAndType();
	virtual ~BacNetAbstractSyntaxAndType();

	BacNetAbstractSyntaxAndType(BACNET_APPLICATION_TAG *tag, BACNET_APPLICATION_DATA_VALUE *appValue, class BacNetAbstractSyntaxAndType *next);
	friend class BacNetPropertyValue;
	class BacNetAbstractSyntaxAndType *getNext();
	void setNext(class BacNetAbstractSyntaxAndType *next);
	BACNET_APPLICATION_TAG get_abstract_type(void);
	BACNET_APPLICATION_DATA_VALUE *get_abstract_value(void);

};

#endif /* BACNETABSTRACTSYNTAXANDTYPE_H_ */
