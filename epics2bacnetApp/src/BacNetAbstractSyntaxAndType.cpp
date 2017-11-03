/*
 * BacNetAbstractSyntaxAndType.cpp
 *
 *  Created on: Jan 3, 2012
 *      Author: 8w4
 */

#include "BacNetAbstractSyntaxAndType.h"

BacNetAbstractSyntaxAndType::BacNetAbstractSyntaxAndType() {
	this->abstract_type = (BACNET_APPLICATION_TAG) 0;
	this->abstract_value.type.Character[0] = '\0';
	this->next = NULL;

}

BacNetAbstractSyntaxAndType::~BacNetAbstractSyntaxAndType() {
	// TODO Auto-generated destructor stub
}

BacNetAbstractSyntaxAndType::BacNetAbstractSyntaxAndType(BACNET_APPLICATION_TAG *tag, BACNET_APPLICATION_DATA_VALUE *appValue, class BacNetAbstractSyntaxAndType *next) {

	this->abstract_type = *tag;
	this->abstract_value = *appValue;
	this->next = next;
}

class BacNetAbstractSyntaxAndType *BacNetAbstractSyntaxAndType::getNext() {
	return this->next;
}

void BacNetAbstractSyntaxAndType::setNext(class BacNetAbstractSyntaxAndType *next) {
	this->next = next;
}

BACNET_APPLICATION_TAG BacNetAbstractSyntaxAndType::get_abstract_type(void) {
	return this->abstract_type;
}

BACNET_APPLICATION_DATA_VALUE *BacNetAbstractSyntaxAndType::get_abstract_value(void) {
	return &this->abstract_value;
}

