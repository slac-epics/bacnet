/*
 * BacNetVariableDigital.cpp
 *
 *  Created on: Nov 15, 2011
 *      Author: 8w4
 */

#include "BacNetVariableDigital.h"
#include "common.h"

BacNetVariableDigital::BacNetVariableDigital()
:BacNetVariable(0,0,0,0,0, BACNET_APPLICATION_TAG_ENUMERATED, NULL)
{
	// TODO Auto-generated constructor stub

}

BacNetVariableDigital::BacNetVariableDigital(uint16_t object_type, uint32_t object_instance, uint32_t property_id, int obj_index, uint8_t write_priority,
		class BacNetCOV *covArg)
: BacNetVariable(object_type, object_instance, property_id, obj_index, write_priority, BACNET_APPLICATION_TAG_ENUMERATED, covArg)
{

}

BacNetVariableDigital::~BacNetVariableDigital() {
	// TODO Auto-generated destructor stub
}

unsigned long BacNetVariableDigital::getValue(){
	return this->BAC_APP_DATA_VALUE.type.Enumerated;
}

void BacNetVariableDigital::setValue(unsigned long newValue){
	this->BAC_APP_DATA_VALUE.type.Enumerated = newValue;
}

void BacNetVariableDigital::write_value(unsigned long newValue){
	this->BAC_APP_DATA_VALUE.type.Enumerated = newValue;
	this->do_write = true;
}

void BacNetVariableDigital::setValue(BACNET_APPLICATION_DATA_VALUE &val) {
	this->BAC_APP_DATA_VALUE.type.Enumerated = val.type.Enumerated;
}

void BacNetVariableDigital::toString() {
	BacNetVariable::toString();
}

void BacNetVariableDigital::printValue(void) {
	printf("\nDigital: %u\n", this->BAC_APP_DATA_VALUE.type.Enumerated);
}

