/*
 * BacNetVariableString.cpp
 *
 *  Created on: Nov 21, 2011
 *      Author: 8w4
 */

#include "BacNetVariableString.h"
#include "BacNetReadPropertyRequest.h"
#include "BacNetWritePropertyRequest.h"
#include "BacNetNpdu.h"
#include "BacNetBVLC.h"


BacNetVariableString::BacNetVariableString()
:BacNetVariable(0,0,0,0,0, BACNET_APPLICATION_TAG_CHARACTER_STRING, NULL)
{
	// TODO Auto-generated constructor stub

}

BacNetVariableString::BacNetVariableString(uint16_t object_type, uint32_t object_instance, uint32_t property_id, int obj_index, uint8_t write_priority)
: BacNetVariable(object_type, object_instance, property_id, obj_index, write_priority, BACNET_APPLICATION_TAG_CHARACTER_STRING, NULL)
{
	this->BAC_APP_DATA_VALUE.type.Character[0] = '\0';
}

BacNetVariableString::~BacNetVariableString() {
	// TODO Auto-generated destructor stub
}

const char *BacNetVariableString::getValue() const{
	return this->BAC_APP_DATA_VALUE.type.Character;
}

void BacNetVariableString::setValue(char *newValue){
	unsigned int l = strnlen(newValue, 255);
	strncpy(this->BAC_APP_DATA_VALUE.type.Character, newValue, l);
	this->BAC_APP_DATA_VALUE.type.Character[l] = '\0';
}

void BacNetVariableString::write_value(char *newValue){
	unsigned int l = strnlen(newValue, 255);
	strncpy(this->BAC_APP_DATA_VALUE.type.Character, newValue, l);
	this->BAC_APP_DATA_VALUE.type.Character[l] = '\0';
	this->do_write = true;
}

void BacNetVariableString::setValue(BACNET_APPLICATION_DATA_VALUE &val) {
	unsigned int l = strnlen(val.type.Character, 255);
	strncpy(this->BAC_APP_DATA_VALUE.type.Character, val.type.Character, l);
	this->BAC_APP_DATA_VALUE.type.Character[l] = '\0';
}

void BacNetVariableString::toString() {

	BacNetVariable::toString();
}

void BacNetVariableString::printValue(void) {
	printf("\nString: %s\n", this->BAC_APP_DATA_VALUE.type.Character);
}


