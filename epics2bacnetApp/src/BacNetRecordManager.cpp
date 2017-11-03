/*
 * BacNetRecordManager.cpp
 *
 *  Created on: Oct 17, 2011
 *      Author: 8w4
 */

#include "BacNetRecordManager.h"
unsigned int BacNetRecordManager::pass = 0;

BacNetRecordManager::BacNetRecordManager() {

	this->counter = 1;
	for(int i=0; i < MAX_INVOKE_ID; i++){
		this->record[i].var = NULL;
		this->record[i].invoke_id = 0;
		this->record[i].sent = false;
		this->record[i].received = false;
		this->record[i].tx_time.tv_sec = 0;
		this->record[i].tx_time.tv_usec = 0;
		this->record[i].rx_time.tv_sec = 0;
		this->record[i].rx_time.tv_usec = 0;
		this->record[i].m_type = BACNET_RECORD::INITIAL;
	}

}

BacNetRecordManager::~BacNetRecordManager() {

}

uint8_t BacNetRecordManager::register_variable(class BacNetVariable *var, enum BACNET_RECORD::msg_type msgT){

    uint8_t rval = 0;

    lock();

    for(int i = 0; i < MAX_INVOKE_ID; i++){
            uint8_t id = this->get_id();

            /** If this slot is still in use then try the next...*/
            if(this->record[id-1].var) {
				increment_id();
				//printf("\nPass: %u; id: %u\n", pass++, id);
            }
            /** This slot is empty (available) so, use it...*/
            else {
            	var->set_variable_que(&this->record[id-1]);
            	this->record[id-1].m_type = msgT;
				this->record[id-1].var = var;
				this->record[id-1].invoke_id = id;
				gettimeofday(&this->record[id-1].tx_time, NULL);
				rval = id;
				increment_id();
				//printf("\nPass: %u; id: %u\n", pass++, id);
				break;
            }
    }

    unlock();

    return rval;
}

void BacNetRecordManager::release_variable(class BacNetVariable *var) {
	BACNET_RECORD *ptr = this->record;
	timeval zeroTimeval = {0, 0};

	lock();

    while(ptr->var != var){
		ptr++;
    }

    var->delete_variable_que();

    ptr->m_type = BACNET_RECORD::INITIAL;
    ptr->invoke_id = 0;
    ptr->received = false;
    ptr->rx_time = zeroTimeval;
    ptr->sent = false;
    ptr->tx_time = zeroTimeval;
    ptr->var = NULL;

    unlock();
}

uint8_t BacNetRecordManager::get_id_byIndex(uint8_t val) const
{
	return this->record[val].invoke_id;
}

void BacNetRecordManager::report(void)
{
	BACNET_RECORD *ptr = this->record;
	BACNET_RECORD *end = &this->record[255];

	lock();

    for(; ptr < end; ptr++){
    	if(ptr->var){
		printf("\nInvoke_Id: %d, var(Object: %u, Instance: %u, Property: %u)\n", ptr->invoke_id, ptr->var->getObjectType(),
				ptr->var->getObject_instance(), ptr->var->getProperty_Id());
    	}
    }

    unlock();
}

uint8_t BacNetRecordManager::get_id(void) const
{
	return this->counter;
}

void BacNetRecordManager::increment_id(void)
{
	lock();
	uint8_t id = get_id();
	++id;
	if(id > MAX_INVOKE_ID || id < 1){
		id = 1;
	}
	this->counter = id;

	unlock();
}

BacNetVariable *BacNetRecordManager::get_ptr_var(uint8_t invoke_id) {
	return this->record[invoke_id -1].var;
}

void BacNetRecordManager::lock() {
	this->mutex.lock();
}

void BacNetRecordManager::unlock() {
	this->mutex.unlock();
}
