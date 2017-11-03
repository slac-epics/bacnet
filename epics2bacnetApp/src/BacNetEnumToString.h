/*
 * BacNetEnumToString.h
 *
 *  Created on: Nov 30, 2011
 *      Author: 8w4
 */

#ifndef BACNETENUMTOSTRING_H_
#define BACNETENUMTOSTRING_H_

#include <stdlib.h>
#include <stdio.h>

#include "bacenum.h"

/* index and text pairs */
//typedef struct {
//    unsigned index;     /* index number that matches the text */
//    const char *pString;        /* text pair - use NULL to end the list */
//} INDTEXT_DATA;


class BacNetEnumToString {
private:

public:
	BacNetEnumToString();
	virtual ~BacNetEnumToString();

    const char *bactext_confirmed_service_name(unsigned index);
    const char *bactext_unconfirmed_service_name(unsigned index);
    const char *bactext_application_tag_name(unsigned index);
    const char *bactext_object_type_name(unsigned index);
    bool bactext_object_type_index(const char *search_name,unsigned *found_index);
    const char *bactext_property_name(unsigned index);
    bool bactext_property_index(const char *search_name,unsigned *found_index);
    const char *bactext_engineering_unit_name(unsigned index);
    bool bactext_engineering_unit_index(const char *search_name,unsigned *found_index);
    const char *bactext_reject_reason_name(unsigned index);
    const char *bactext_abort_reason_name(unsigned index);
    const char *bactext_error_class_name(unsigned index);
    const char *bactext_error_code_name(unsigned index);
    unsigned bactext_property_id(const char *name);
    const char *bactext_month_name(unsigned index);
    const char *bactext_week_of_month_name(unsigned index);
    const char *bactext_day_of_week_name(unsigned index);
    const char *bactext_event_state_name(unsigned index);
    const char *bactext_binary_present_value_name(unsigned index);
    const char *bactext_binary_polarity_name(unsigned index);
    bool bactext_binary_present_value_index(const char *search_name,unsigned *found_index);
    const char *bactext_reliability_name(unsigned index);
    const char *bactext_device_status_name(unsigned index);
    const char *bactext_segmentation_name(unsigned index);
    const char *bactext_node_type_name(unsigned index);

    const char *bactext_event_transition_name(unsigned index);
    bool bactext_event_transition_index(const char *search_name,unsigned *found_index);

    const char *bactext_days_of_week_name(unsigned index);
    bool bactext_days_of_week_index(const char *search_name,unsigned *found_index);

    const char *bactext_network_layer_msg_name(unsigned index);
};

#endif /* BACNETENUMTOSTRING_H_ */
