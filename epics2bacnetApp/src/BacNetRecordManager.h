/*
 * BacNetRecordManager.h
 *
 *  Created on: Oct 17, 2011
 *      Author: 8w4
 */

#ifndef BACNETRECORDMANAGER_H_
#define BACNETRECORDMANAGER_H_

#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <time.h>
#include <sys/time.h>

#include <epicsMutex.h>

#include "bacstructures.h"
#include "BacNetVariable.h"

#define MAX_INVOKE_ID 255
#define MIN_INVOKE_ID 1

class BacNetRecordManager {
private:
	uint8_t counter;
	BACNET_RECORD record[MAX_INVOKE_ID];
	uint8_t get_id(void) const;
	void increment_id(void);

	class epicsMutex mutex;
	static unsigned int pass;
public:
	BacNetRecordManager();
	virtual ~BacNetRecordManager();

	uint8_t register_variable(class BacNetVariable *var, enum BACNET_RECORD::msg_type msgT);
	void release_variable(class BacNetVariable *var);
	void report(void);
	uint8_t get_id_byIndex(uint8_t val) const;
	BacNetVariable *get_ptr_var(uint8_t invoke_id);

	void lock(void);
	void unlock(void);
};


#endif /* BACNETRECORDMANAGER_H_ */
