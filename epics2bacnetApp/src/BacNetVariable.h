/*
 * BacNetVariable.h
 *
 *  Created on: Aug 19, 2011
 *      Author: 8w4
 */

#ifndef BACNETVARIABLE_H_
#define BACNETVARIABLE_H_

//System
#include <stdio.h>
#include <stdint.h>

//EPICS
#include <epicsMutex.h>

//Local
#include "bacstructures.h"
#include "BacNetConnection.h"
#include "BacNetDevice.h"
#include "common.h"
#include "BacNetBuffer.h"
#include "BacNetCOV.h"

class BacNetVariable {
private:

	BACNET_OBJECT_ID object_id;
	uint32_t property_id;
	int property_array_index;
	uint8_t write_priority;

	const BACNET_APPLICATION_TAG BAC_APP_DATA_TYPE;

	class BacNetVariable *next;

	BACNET_RECORD *que;

	epicsMutex mutex;

	class BacNetCOV *cov;

	void retry(class BacNetBuffer *buffer, class BacNetConnection *con, class BacNetDevice *dev);

protected:
	bool is_valid;
	bool do_write;
	BACNET_APPLICATION_DATA_VALUE BAC_APP_DATA_VALUE;

public:
	BacNetVariable(uint16_t object_type, uint32_t object_instance, uint32_t property_id, int obj_index, uint8_t write_priority,
			const BACNET_APPLICATION_TAG bacAppDataType, class BacNetCOV *cov);

	virtual bool process(class BacNetConnection *con, class BacNetDevice *dev);
	virtual void setValue(BACNET_APPLICATION_DATA_VALUE &val) = 0;
	virtual void printValue(void) = 0;

	uint16_t getObjectType();
	const uint16_t *get_ptr_object_type();
	void setObjectType(uint16_t object_type);

	BACNET_OBJECT_ID get_bacnet_obj_id(BACNET_OBJECT_ID *obj_id);
	const BACNET_OBJECT_ID *get_ptr_bacnet_obj_id(BACNET_OBJECT_ID *obj_id);
	void set_bacnet_obj_id(BACNET_OBJECT_ID bac_obj_id);

	uint32_t getObject_instance();
	const uint32_t *get_ptr_object_instance();
	void setObject_instance(uint32_t instance);

	uint32_t getProperty_Id();
	const uint32_t *get_ptr_property_id();
	void setProperty_Id(uint32_t prop_id);

	int get_property_array_index();
	const int *get_ptr_property_array_index();
	void set_property_array_index(int prop_index);

	uint8_t get_priority();
	const uint8_t *get_ptr_priority();
	void set_priority(uint8_t new_priority);

	class BacNetVariable *getNext();
	void setNext(class BacNetVariable *next);

	BACNET_RECORD *get_variable_que(void);
	void set_variable_que(BACNET_RECORD *new_rec);
	void delete_variable_que(void);

	bool get_is_valid();
	void setIsValid(bool val);

	bool get_do_write(void);
	void set_do_write(bool newValue);

	/** Lock the variable for concurrent access */
	void lock();

	/** Unock the variable for concurrent access */
	void unlock();

	class BacNetCOV *get_variable_cov(void);

	static BACNET_APPLICATION_TAG calcDataType(uint16_t objecType);

	virtual void toString(void);

	virtual ~BacNetVariable();
};

#endif /* BACNETVARIABLE_H_ */
