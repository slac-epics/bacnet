/*
 * BacNetDevice.h
 *
 *  Created on: Aug 18, 2011
 *      Author: 8w4
 */

#ifndef BACNETDEVICE_H_
#define BACNETDEVICE_H_

// System
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// EPICS
#include <epicsMutex.h>

//Local
#include "BacNetVariable.h"
#include "BacNetIp.h"
#include "BacNetConnection.h"
#include "BacNetRecordManager.h"

#define MAX_DEVICE_NAME_LENGTH 255

class BacNetDevice {
private:
	char name[MAX_DEVICE_NAME_LENGTH];
	bool isKnown;
	uint8_t mac[6];

	uint32_t instance;
	uint32_t max_apdu_length_accepted;	/** Initialize to 1476.*/
	uint32_t apdu_timeout;				/** Initialize to 3000 mili sec.*/
	uint32_t max_number_of_apdu_retries;	/** Initialize to 3.*/
	uint16_t snet;
	uint8_t slen;
	uint8_t sadr[6];
	bool segmentation_supported;
	uint32_t vendor_ID;
	uint8_t hop_count;
	unsigned int poll_delay;	/** In milliseconds...*/

	class BacNetVariable *bacNetVariable;
	class BacNetVariable *var_last_scan;
	class BacNetDevice *next;
	class BacNetRecordManager *rm;

	class epicsMutex mutex;
	uint8_t apdu_retry_count;

	time_t last_who_is;

	unsigned int number_of_variables;
	void increment_number_of_variables(void);
	uint32_t get_cov_process_id(void);

	static unsigned int number_of_devices;
	static unsigned int cov_process_id;

public:

	BacNetDevice(const char *name, uint32_t instance, uint16_t port, unsigned int pollDelay, BacNetDevice *next);

	BacNetDevice *getNext();
	char *setName(const char *new_name);
	char *getName();
	uint32_t getInstance();
	int getPort();
	sockaddr_in *getDeviceIP(sockaddr_in *addr);
	BacNetVariable *getBacNetvariableList();
	BacNetVariable *get_bacnet_variable_by_process_id(unsigned int p_id);
	void addBacNetVariable(class BacNetVariable *var);
	void scanBacNetVariables(class BacNetConnection *con);
	uint8_t *getDevMac();
	void setDevMac(uint8_t *new_mac, int length);
	void printDevMac();
	void set_isKnown(bool value);
	bool get_isKnown();
	uint16_t getSnet();
	void setSnet(uint16_t val);
	uint8_t getSlen();
	void setSlen(uint8_t val);
	uint8_t *getSadr();
	void setSadr(uint8_t *ptr, uint8_t len);

	uint8_t register_var_to_invoke_id(class BacNetVariable *var, enum BACNET_RECORD::msg_type msgT);
	BacNetVariable *getBacNetvariableByInvokeId(uint8_t invoke_id);

	uint8_t get_max_number_apdu_retries(void) const;
	uint32_t get_apdu_timeout(void) const;
	uint8_t get_retry_count(void) const;
	uint8_t increment_retry_count(void);
	uint8_t clear_retry_count(void);

	void release_invoke_id_to_var(class BacNetVariable *var);
	void lock();
	void unlock();

	unsigned int get_number_of_variables(void);

	void set_var_last_scan(class BacNetVariable *var);

	void print_device_info(void);

	static unsigned int get_number_of_devices();

	virtual ~BacNetDevice();
};

#endif /* BACNETDEVICE_H_ */
