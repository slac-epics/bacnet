/*
 * BacNetDevice.cpp
 *
 *  Created on: Aug 18, 2011
 *      Author: 8w4
 */

#include "BacNetDevice.h"
#include "common.h"
#include "BacNetConnection.h"
#include "BacNetWhoIsRequest.h"
#include "BacNetReadPropertyRequest.h"
#include "BacNetNpdu.h"
#include "BacNetBVLC.h"
#include "BacNetBuffer.h"
#include "BacNetRequest.h"

// EPICS
#include "epicsThread.h"

unsigned int BacNetDevice::number_of_devices = 0;
unsigned int BacNetDevice::cov_process_id = 1;

const unsigned int POLL_DELAY_MIN = 15;	/** Milliseconds...*/
const unsigned int POLL_DELAY_DEFAULT = 25;	/** Milliseconds...*/

BacNetDevice::BacNetDevice(const char *name, uint32_t instance, uint16_t port, unsigned int pollDelay, BacNetDevice *next)
:segmentation_supported(false), vendor_ID(0), hop_count(0), poll_delay(pollDelay), var_last_scan(0), apdu_retry_count(1), last_who_is(0), number_of_variables(0)
{

	this->setName(name);
	strncpy(this->name, name, 255);
	this->instance = instance;
	this->max_apdu_length_accepted = 1476;
	this->apdu_timeout = 10000;
	this->max_number_of_apdu_retries = 3;
	memset(this->mac, 0, 6);
	this->snet = 0x0000;
	this->slen = 0x00;
	memset(this->sadr, 0, 6);
	this->next = next;
	this->bacNetVariable = 0;
	this->isKnown = false;
	this->rm = new BacNetRecordManager();

	if(pollDelay <= 0) {this->poll_delay = POLL_DELAY_DEFAULT;}

	number_of_devices++;
}

char *BacNetDevice::getName() {
	return name;
}

char *BacNetDevice::setName(const char *new_name) {
	strncpy(this->name, new_name, MAX_DEVICE_NAME_LENGTH);
	this->name[254] = '\0';
	return this->name;
}

uint32_t BacNetDevice::getInstance() {
	return instance;
}

BacNetDevice *BacNetDevice::getNext() {
	return next;
}

BacNetVariable *BacNetDevice::getBacNetvariableList() {
	return bacNetVariable;
}

/** Returns NULL if it can't find variable...*/
BacNetVariable *BacNetDevice::get_bacnet_variable_by_process_id(unsigned int p_id) {
	BacNetVariable *var = this->getBacNetvariableList();

	while(var) {
		if(var->get_variable_cov())
			if(var->get_variable_cov()->getProcess_identifier() == p_id)
				return var;
		var = var->getNext();
	}
	return NULL;
}

uint32_t BacNetDevice::get_cov_process_id(void) {
	return cov_process_id++;
}

void BacNetDevice::addBacNetVariable(class BacNetVariable *var) {
	var->setNext(bacNetVariable);
	bacNetVariable = var;
	increment_number_of_variables();
	if(var->get_variable_cov())
		var->get_variable_cov()->setProcess_identifier(this->get_cov_process_id());
}

void BacNetDevice::set_isKnown(bool value){
	lock();
	this->isKnown = value;
	unlock();
}

bool BacNetDevice::get_isKnown(){
	return this->isKnown;
}

uint16_t BacNetDevice::getSnet(){
	return this->snet;
}

void BacNetDevice::setSnet(uint16_t val){
	lock();
	this->snet = val;
	unlock();
}

uint8_t BacNetDevice::getSlen(){
	return this->slen;
}

void BacNetDevice::setSlen(uint8_t val){
	lock();
	this->slen = val;
	unlock();
}

uint8_t *BacNetDevice::getSadr(){
	return this->sadr;
}

void BacNetDevice::setSadr(uint8_t *ptr, uint8_t len){
	lock();
	memcpy(this->sadr, ptr, len);
	unlock();
}

void BacNetDevice::scanBacNetVariables(class BacNetConnection *con) {

	BacNetVariable *var = this->bacNetVariable;
	BacNetBuffer *buffer = new BacNetBuffer();

	/** If the device is not known then send a BACnet who-is to find out network address...*/
	if(!this->isKnown)
	{
		time_t timeNow = time(NULL);
		/** Basically, don't broadcast who-is too fast...
		 * Maybe ~10 seconds...*/
		if((timeNow-this->last_who_is) > 9){
			if(verbosity_level_2(verbosity_level))
				printf("\tThis device \"%s\" is unknown; do a \"who-is\"...\n", name);
			BacNetRequest *req = new BacNetWhoIsRequest(this->getInstance(), this->getInstance());
			req->encode_request(buffer);

			if(verbosity_level_3(verbosity_level))
				buffer->hexdump(buffer->getPointerToBuffer(), buffer->getLength());

			con->sendTo(this, buffer->getPointerToBuffer(), buffer->getLength(), BROADCAST);
			this->last_who_is = time(NULL);
			if(verbosity_level_2(verbosity_level))
				printf("\tWho-is? \"%s\"... \"%s\"... \"%s\"...\n", name, name, name);
			/** delete request object...*/
			delete req;
		}
	}
	else
	{
		if(verbosity_level_2(verbosity_level)) {
			printf("\tThis device is known: %s ", name);
			this->printDevMac();
		}

		// Send out requests
		if(this->var_last_scan) {
			var = this->var_last_scan;
			this->var_last_scan = NULL;
		}
		else {
			/** Go to the beginning of the linked-list...*/
			var = this->bacNetVariable;
		}

		while(var) {
			if(verbosity_level_2(verbosity_level))
				printf("\tI (%s) should be scanning this %u, %u, %u right now.\n", this->name, var->getObjectType(),
						var->getObject_instance(), var->getProperty_Id());

			/** If something went wrong during scan then mark the variable and and restart from it...*/
			if(!var->process(con, this)) {
				delete buffer;
				this->var_last_scan = var;
				return;
			}

			/** Poll delay? After testing, Ethernet devices did not like getting hit with
			 * multiple request at the same time... I noticed better results using a poll delay.
			 * BTL Implemention Guidlines 7.4 states: "Do not poll as fast as network allows..."
			 * */
			epicsThreadSleep((double)this->poll_delay / 1000.0);
			var = var->getNext();
		}
	}
	delete buffer;
}

sockaddr_in *BacNetDevice::getDeviceIP(sockaddr_in *addr){
	lock();
	memcpy(&addr->sin_addr, this->mac, 4);
	unlock();
	return addr;
}

void BacNetDevice::setDevMac(uint8_t *new_mac, int length) {
	lock();
	memcpy(this->mac, new_mac, length);
	unlock();
}

uint8_t *BacNetDevice::getDevMac(){
	return this->mac;
}

void BacNetDevice::printDevMac(){
	int i;
	for(i=0; i < 6; i++){
		if(i<5)
			printf("%02X:", this->mac[i]);
		else
			printf("%02X", this->mac[i]);
	}
	printf("\n");
}

void BacNetDevice::print_device_info(void) {
	//TODO Print something useful...
	sockaddr_in addr;
	printf("\nDevice Instance: %u {\n", this->instance);
	printf("  Name:\t\"%s\"\n", this->name);
	printf("  IP:\t%s\n", inet_ntoa(this->getDeviceIP(&addr)->sin_addr));
	printf("  Instance:\t%u\n", this->instance);
	printf("  MAC:\t");
	this->printDevMac();
	printf("  SNET:\t%u\n", this->snet);
	printf("  SADR:\t%u\n", *this->getSadr());
	printf("  APDU:\t%u\n", this->max_apdu_length_accepted);
	printf("}\n");
}

BacNetVariable *BacNetDevice::getBacNetvariableByInvokeId(uint8_t invoke_id){
	return this->rm->get_ptr_var(invoke_id);
}

uint8_t BacNetDevice::get_max_number_apdu_retries(void) const {
	return this->max_number_of_apdu_retries;
}

uint32_t BacNetDevice::get_apdu_timeout() const {
	/** In mili seconds...*/
	return this->apdu_timeout;
}

uint8_t BacNetDevice::get_retry_count(void) const {
	return this->apdu_retry_count;
}

uint8_t BacNetDevice::increment_retry_count(void) {
	lock();
	++this->apdu_retry_count;
	unlock();
	return this->apdu_retry_count;
}

uint8_t BacNetDevice::clear_retry_count(void) {
	lock();
	this->apdu_retry_count = 1;
	unlock();
	return this->apdu_retry_count = 1;
}

void BacNetDevice::lock(){
	this->mutex.lock();
}

void BacNetDevice::unlock(){
	this->mutex.unlock();
}

uint8_t BacNetDevice::register_var_to_invoke_id(class BacNetVariable *var, enum BACNET_RECORD::msg_type msgT)
{
	return this->rm->register_variable(var, msgT);
}

void BacNetDevice::release_invoke_id_to_var(class BacNetVariable *var){
	this->rm->release_variable(var);
}

unsigned int BacNetDevice::get_number_of_variables(void) {
	return this->number_of_variables;
}

void BacNetDevice::increment_number_of_variables(void) {
	this->number_of_variables++;
}

unsigned int BacNetDevice::get_number_of_devices() {
	return number_of_devices;
}

BacNetDevice::~BacNetDevice() {
	// TODO Auto-generated destructor stub
}
