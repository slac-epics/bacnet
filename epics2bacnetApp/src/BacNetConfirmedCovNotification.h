/*
 * BacNetConfirmedCovNotification.h
 *
 *  Created on: Dec 30, 2011
 *      Author: 8w4
 */
#include <stdio.h>
#include <stdint.h>
#include "bacenum.h"
#include "bacstructures.h"
#include "BacNetPropertyValue.h"
#include "BacNetAbstractSyntaxAndType.h"
#include "BacNetBuffer.h"

#ifndef BACNETCONFIRMEDCOVNOTIFICATION_H_
#define BACNETCONFIRMEDCOVNOTIFICATION_H_

class BacNetConfirmedCovNotification {
private:
	uint32_t subscriber_process_identifier;
	BACNET_OBJECT_ID initiating_device_identifier;
	BACNET_OBJECT_ID monitored_object_identifier;
	unsigned int time_remaining;
	class BacNetPropertyValue *list_of_values;

public:
	BacNetConfirmedCovNotification();
	virtual ~BacNetConfirmedCovNotification();
	void add_value_to_list(class BacNetPropertyValue *propertyValue);
	class BacNetBuffer *decode_confirmed_cov_notification(class BacNetBuffer *buffer);
	void report_cov(void);

    uint32_t getSubscriber_process_identifier() const;
    void setSubscriber_process_identifier(uint32_t subscriber_process_identifier);
    BACNET_OBJECT_ID *getInitiating_device_identifier() const;
    void setInitiating_device_identifier(BACNET_OBJECT_ID *initiating_device_identifier);
    BACNET_OBJECT_ID *getMonitored_object_identifier() const;
    void setMonitored_object_identifier(BACNET_OBJECT_ID *monitored_object_identifier);
    unsigned int getTime_remaining() const;
    void setTime_remaining(unsigned int time_remaining);
    class BacNetPropertyValue *getList_of_values() const;
    void setList_of_values(class BacNetPropertyValue *list_of_values);

};

#endif /* BACNETCONFIRMEDCOVNOTIFICATION_H_ */
