/*
 * BacNetConfirmedCovNotification.cpp
 *
 *  Created on: Dec 30, 2011
 *      Author: 8w4
 */

#include "BacNetConfirmedCovNotification.h"
#include "common.h"

BacNetConfirmedCovNotification::BacNetConfirmedCovNotification() {
	this->subscriber_process_identifier = 0;
	this->initiating_device_identifier.type = 0;
	this->initiating_device_identifier.instance = 0;
	this->monitored_object_identifier.type = 0;
	this->monitored_object_identifier.instance = 0;
	this->time_remaining = 0;
	this->list_of_values = NULL;

}

BacNetConfirmedCovNotification::~BacNetConfirmedCovNotification() {
	// TODO Auto-generated destructor stub
}

void BacNetConfirmedCovNotification::add_value_to_list(class BacNetPropertyValue *propertyValue) {
	this->list_of_values->set_next(propertyValue);
}

class BacNetBuffer *BacNetConfirmedCovNotification::decode_confirmed_cov_notification(class BacNetBuffer *buffer) {

	/** ConfirmedCOVNotification-Request ::= SEQUENCE {
	 * subscriberProcessIdentifier	[0]
	 * initiatingDeviceIdentifier	[1]
	 * monitoredObjectIdentifier	[2]
	 * timeRemaining				[3]
	 * listOfValues					[4]
	 * }*/

	uint8_t *pdu = buffer->get_currentPosition();
	//buffer->hexdump(pdu, 10);

	uint8_t tagNumber = 0;
	uint8_t tagClass = 0;
	uint32_t tagLVT = 0;

	/** [0] Decode Process Identifier...*/
	pdu = buffer->decode_bacnet_tag(pdu, &tagNumber, &tagClass, &tagLVT);
	//printf("\ntagNumber: %u, tagClass: %u, tagLVT: %u\n", tagNumber, tagClass, tagLVT);
	if(tagNumber != 0 || tagClass != 1){
		if(verbosity_level_1(verbosity_level))
			printf("\nBad packet! Should be BacNetConfirmedCovNotification Process-ID: %u\n", tagLVT);
		return NULL;
	}
	pdu = buffer->decode_bacnet_unsigned_int(pdu, &this->subscriber_process_identifier, tagLVT);

	/** [1] Decode Initiating Device Identifier...*/
	pdu = buffer->decode_bacnet_tag(pdu, &tagNumber, &tagClass, &tagLVT);
	//printf("\ntagNumber: %u, tagClass: %u, tagLVT: %u\n", tagNumber, tagClass, tagLVT);
	if(tagNumber != 1 || tagClass != 1 || tagLVT != 4){
		if(verbosity_level_2(verbosity_level))
			printf("\nBad packet! Should be BacNetConfirmedCovNotification Initiating Device Identifier: %u\n", tagLVT);
		return NULL;
	}
	pdu = buffer->decode_bacnet_object_id(pdu, &this->initiating_device_identifier.type,
			&this->initiating_device_identifier.instance);

	/** [2] Decode Monitored Device Identifier...*/
	pdu = buffer->decode_bacnet_tag(pdu, &tagNumber, &tagClass, &tagLVT);
	//printf("\ntagNumber: %u, tagClass: %u, tagLVT: %u\n", tagNumber, tagClass, tagLVT);
	if(tagNumber != 2 || tagClass != 1 || tagLVT != 4){
		if(verbosity_level_2(verbosity_level))
			printf("\nBad packet! Should be BacNetConfirmedCovNotification Monitored Device Identifier: %u\n", tagLVT);
		return NULL;
	}
	pdu = buffer->decode_bacnet_object_id(pdu, &this->monitored_object_identifier.type,
			&this->monitored_object_identifier.instance);

	/** [3] Decode Time Remaining...*/
	pdu = buffer->decode_bacnet_tag(pdu, &tagNumber, &tagClass, &tagLVT);
	//printf("\ntagNumber: %u, tagClass: %u, tagLVT: %u\n", tagNumber, tagClass, tagLVT);
	if(tagNumber != 3 || tagClass != 1){
		if(verbosity_level_2(verbosity_level))
			printf("\nBad packet! Should be BacNetConfirmedCovNotification Process-ID: %u\n", tagLVT);
		return NULL;
	}
	pdu = buffer->decode_bacnet_unsigned_int(pdu, &this->time_remaining, tagLVT);

	//printf("\nTime Remaining: %u, %X\n", this->time_remaining, *pdu);

	/** [4] This will be a list of objects and their values...*/

	/** We should be sitting at the listOfValues opening tag...
	 * BACnetPropertyValue ::=SEQUENCE {
	 * propertyIdentifier	[0]
	 * propertyArrayIndex	[1]
	 * value				[2]
	 * priority				[3]
	 * }*/

	/** 0x4E should be the start of an opening tag*/
	if(*pdu == 0x4E) {
		/** Ok, great, we are; now advance to next octet...*/
		pdu++;

		/** Because this is a list... And, we don't know how long...
		 * Maybe loop until we come across the closing-tag octet (i.e.'0x4F')...*/
		while(*pdu != 0x4F) {

			/** [0] propertyIdentifier...*/
			pdu = buffer->decode_bacnet_tag(pdu, &tagNumber, &tagClass, &tagLVT);
			if(tagNumber != 0 || tagClass != 1 || tagLVT <= 0){
				if(verbosity_level_1(verbosity_level))
					printf("\nBad packet! Should be BACNET_PROPERTY_ID: %u\n", tagLVT);
				return NULL;
			}
			uint32_t tempPropId = 0;
			pdu = buffer->decode_bacnet_unsigned_int(pdu, &tempPropId, tagLVT);

			/** Decode the Tag to see what we have...*/
			pdu = buffer->decode_bacnet_tag(pdu, &tagNumber, &tagClass, &tagLVT);

			/** [1] propertyArrayIndex (Optional)...*/

			/** See if has array-index...*/
			int tempArrayIndex = -1;
			if(tagNumber == 1) {
				/** There is an array present...*/
				if(tagNumber != 1 || tagClass != 1 || tagLVT <= 0){
					if(verbosity_level_1(verbosity_level))
						printf("\nBad packet! Should be BACNET_ARRAY_INDEX: %u\n", tagLVT);
					return NULL;
				}
				pdu = buffer->decode_bacnet_unsigned_int(pdu, (uint32_t *)&tempArrayIndex, tagLVT);
				pdu = buffer->decode_bacnet_tag(pdu, &tagNumber, &tagClass, &tagLVT);
			}

			/** [2] value*/

			/** Better be a value present though...*/
			if(tagNumber != 2 || tagClass != 1 || tagLVT != 0x06){
				if(verbosity_level_1(verbosity_level))
					printf("\nBad packet! Should be BACNET_PROPERTY_VALUE: %u\n", tagLVT);
				return NULL;
			}

			BacNetAbstractSyntaxAndType *appVal;
			while(tagNumber == 0x02 && tagLVT!= 0x07) {
				//printf("\nDecode value...\n");
				/** Decode the value portion...*/
				BACNET_APPLICATION_DATA_VALUE val;
				BACNET_APPLICATION_TAG tag;
				pdu = buffer->decode_bacnet_abstract_syntax_type(pdu, &val, &tag);
				appVal = new BacNetAbstractSyntaxAndType(&tag, &val, appVal);
				pdu = buffer->decode_bacnet_tag(pdu, &tagNumber, &tagClass, &tagLVT);
			}

			/** [3] priority...*/

			/** Another optional tag is Priority... Check to see if it is present...*/
			int tempPriority = -1;
			if(tagNumber == 3) {
				/** There is a priority present...*/
				if(tagNumber != 3 || tagClass != 1 || tagLVT <= 0){
					if(verbosity_level_1(verbosity_level))
						printf("\nBad packet! Should be BACNET_PRIORITY_VALUE: %u\n", tagLVT);
					return NULL;
				}
				pdu = buffer->decode_bacnet_unsigned_int(pdu, (uint32_t *)&tempPriority, tagLVT);

			}
			else {
				/** There is NO priority present...*/
			}
			this->list_of_values = new BacNetPropertyValue(tempPropId, tempArrayIndex, appVal, tempPriority, this->list_of_values);
		}
		//printf("\nFinished decoding listOfValues...\n");
		//report_cov();
	}
	else {
		/** If octet is not equal to 4E then something is wrong...*/
		return NULL;
	}

	return buffer;
}

void BacNetConfirmedCovNotification::report_cov(void) {

	BacNetPropertyValue *list = this->list_of_values;
	while(list) {
		printf("\nPropertyIdentifier: %u\n", list->getProperty_identifier());
		printf("\nPropertyArrayIndex: %d\n", list->getProperty_array_index());
		printf("\nPropertyValue: (%u) %lf\n", list->getValue()->get_abstract_type(),
				list->getValue()->get_abstract_type() == 4 ? list->getValue()->get_abstract_value()->type.Real : list->getValue()->get_abstract_value()->type.Unsigned_Int);
		printf("\nPropertyPriority: %d\n", list->getPriority());
		list = list->get_next();
	}
}

uint32_t BacNetConfirmedCovNotification::getSubscriber_process_identifier() const
{
    return subscriber_process_identifier;
}

void BacNetConfirmedCovNotification::setSubscriber_process_identifier(uint32_t subscriber_process_identifier)
{
    this->subscriber_process_identifier = subscriber_process_identifier;
}

BACNET_OBJECT_ID *BacNetConfirmedCovNotification::getInitiating_device_identifier() const
{
    return (BACNET_OBJECT_ID *) &initiating_device_identifier;
}

void BacNetConfirmedCovNotification::setInitiating_device_identifier(BACNET_OBJECT_ID *initiating_device_identifier)
{
    this->initiating_device_identifier = *initiating_device_identifier;
}

BACNET_OBJECT_ID *BacNetConfirmedCovNotification::getMonitored_object_identifier() const
{
    return (BACNET_OBJECT_ID *) &monitored_object_identifier;
}

void BacNetConfirmedCovNotification::setMonitored_object_identifier(BACNET_OBJECT_ID *monitored_object_identifier)
{
    this->monitored_object_identifier = *monitored_object_identifier;
}

unsigned int BacNetConfirmedCovNotification::getTime_remaining() const
{
    return time_remaining;
}

void BacNetConfirmedCovNotification::setTime_remaining(unsigned int time_remaining)
{
    this->time_remaining = time_remaining;
}

class BacNetPropertyValue *BacNetConfirmedCovNotification::getList_of_values() const
{
    return list_of_values;
}

void BacNetConfirmedCovNotification::setList_of_values(class BacNetPropertyValue *list_of_values)
{
    this->list_of_values = list_of_values;
}

