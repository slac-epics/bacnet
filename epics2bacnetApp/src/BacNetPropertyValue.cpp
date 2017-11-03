/*
 * BacNetPropertyValue.cpp
 *
 *  Created on: Dec 30, 2011
 *      Author: 8w4
 */

#include "BacNetPropertyValue.h"

BacNetPropertyValue::BacNetPropertyValue() {
	this->property_identifier = (BACNET_PROPERTY_ID) 0;
	this->property_array_index = -1;
	this->value = NULL;
	this->priority = -1;

}

BacNetPropertyValue::~BacNetPropertyValue() {
	// TODO Auto-generated destructor stub
}

BacNetPropertyValue::BacNetPropertyValue(uint32_t property_id, int prop_array_index, BacNetAbstractSyntaxAndType *value_list, int priority, class BacNetPropertyValue *next) {
	this->property_identifier = (BACNET_PROPERTY_ID)property_id;
	this->property_array_index = prop_array_index;
	this->value = value_list;
	this->priority = priority;
	this->next = next;
}

BACNET_PROPERTY_ID BacNetPropertyValue::getProperty_identifier() const
{
    return property_identifier;
}

void BacNetPropertyValue::setProperty_identifier(BACNET_PROPERTY_ID property_identifier)
{
    this->property_identifier = property_identifier;
}

int BacNetPropertyValue::getProperty_array_index() const
{
    return property_array_index;
}

void BacNetPropertyValue::setProperty_array_index(int property_array_index)
{
    this->property_array_index = property_array_index;
}

BacNetAbstractSyntaxAndType *BacNetPropertyValue::getValue()
{
    return this->value;
}

void BacNetPropertyValue::setValue(BacNetAbstractSyntaxAndType *value)
{
    this->value = value;
}

int BacNetPropertyValue::getPriority() const
{
    return priority;
}

void BacNetPropertyValue::setPriority(int priority)
{
    this->priority = priority;
}

void BacNetPropertyValue::set_next(class BacNetPropertyValue *next) {
	this->next = next;
}

class BacNetPropertyValue *BacNetPropertyValue::get_next(void) {
	return this->next;
}





