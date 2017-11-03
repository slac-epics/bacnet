/*
 * BacNetVariableDouble.cpp
 *
 *  Created on: Aug 19, 2011
 *      Author: 8w4
 */

#include "BacNetVariableDouble.h"

BacNetVariableDouble::BacNetVariableDouble()
:BacNetVariable(0,0,0,0,0, BACNET_APPLICATION_TAG_REAL, NULL)
{
	// TODO Auto-generated destructor stub
}

BacNetVariableDouble::BacNetVariableDouble(uint16_t object_type, uint32_t object_instance, uint32_t property_id, int obj_index, uint8_t write_priority,
		class BacNetCOV *covArg)
: BacNetVariable(object_type, object_instance, property_id, obj_index, write_priority, BacNetVariable::calcDataType(object_type), covArg)
{

}

BacNetVariableDouble::~BacNetVariableDouble() {
	// TODO Auto-generated destructor stub
}

double BacNetVariableDouble::getValue(){
	return this->BAC_APP_DATA_VALUE.type.Real;
}

void BacNetVariableDouble::setValue(double newValue){
	this->BAC_APP_DATA_VALUE.type.Real = newValue;
}

void BacNetVariableDouble::write_value(double newValue){
	this->BAC_APP_DATA_VALUE.type.Real = newValue;
	this->do_write = true;
}

void BacNetVariableDouble::setValue(BACNET_APPLICATION_DATA_VALUE &val) {
	this->BAC_APP_DATA_VALUE.type.Real = val.type.Real;
}

void BacNetVariableDouble::toString() {

	BacNetVariable::toString();
}

void BacNetVariableDouble::printValue(void) {
	printf("\nDouble: %f\n", this->BAC_APP_DATA_VALUE.type.Real);
}


