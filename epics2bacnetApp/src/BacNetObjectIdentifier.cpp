/*
 * BacNetObjectIdentifier.cpp
 *
 *  Created on: Aug 26, 2011
 *      Author: 8w4
 */

#include "BacNetObjectIdentifier.h"

BacNetObjectIdentifier::BacNetObjectIdentifier(BACNET_OBJECT_TYPE object_type, uint32_t instance_number) {
	this->objectType = object_type;
	this->instanceNumber = instance_number;
}

BACNET_OBJECT_TYPE BacNetObjectIdentifier::getObjectType(){
	return objectType;
}

uint32_t BacNetObjectIdentifier::getInstanceNumber() {
	return instanceNumber;
}

class BacNetObjectIdentifier *BacNetObjectIdentifier::getBacNetObjID() {
	return this;
}

BacNetObjectIdentifier::~BacNetObjectIdentifier() {
	// TODO Auto-generated destructor stub
}
