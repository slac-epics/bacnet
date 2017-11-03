/*
 * bacstructures.h
 *
 *  Created on: Sep 26, 2011
 *      Author: 8w4
 */

#ifndef BACSTRUCTURES_H_
#define BACSTRUCTURES_H_

#include <stdint.h>
#include <time.h>
#include <sys/time.h>


#include "bacenum.h"

typedef struct BACnet_Object_Id {
    uint16_t type;
    uint32_t instance;
} BACNET_OBJECT_ID;

typedef struct bacnet_application_data_value_t {
	union {
		bool Boolean;
		uint32_t Unsigned_Int;
		int32_t Signed_Int;
		float Real;
		double Double;
		char Character[255];
		uint32_t Enumerated;
		BACNET_OBJECT_ID Object_Id;
	} type;
	struct bacnet_application_data_value_t *next;
} BACNET_APPLICATION_DATA_VALUE;



typedef struct bacnet_record_t{
	enum msg_type {
		INITIAL,
		READ,
		WRITE
	};
	msg_type m_type;
	class BacNetVariable *var;
	uint8_t invoke_id;
	bool sent;
	bool received;
	timeval tx_time;
	timeval rx_time;

} BACNET_RECORD;

typedef struct bacnet_que_t {
   class BacNetVariable *var;
   uint8_t invoke_id;
   timeval tx_time;
} BACNET_QUE;


#endif /* BACSTRUCTURES_H_ */
