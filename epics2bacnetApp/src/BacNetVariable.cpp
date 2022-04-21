/*
 * BacNetVariable.cpp
 *
 *  Created on: Aug 19, 2011
 *      Author: 8w4
 */

#include "BacNetVariable.h"
#include "BacNetReadPropertyRequest.h"
#include "BacNetWritePropertyRequest.h"
#include "BacNetSubscribeCovRequest.h"
#include "BacNetNpdu.h"
#include "BacNetBVLC.h"
#include <time.h>


BacNetVariable::BacNetVariable(uint16_t object_type, uint32_t object_instance, uint32_t property_id, int obj_index, uint8_t write_priority,
		const BACNET_APPLICATION_TAG bacAppDataType, class BacNetCOV *covArg)
:BAC_APP_DATA_TYPE(bacAppDataType), next(0), cov(covArg)
{
	this->object_id.type = object_type;
	this->object_id.instance = object_instance;
	this->property_id = property_id;
	this->property_array_index = obj_index;
	this->write_priority = write_priority;
	this->que = NULL;
	this->is_valid = false;
	this->do_write = false;
}

BACNET_APPLICATION_TAG BacNetVariable::calcDataType (uint16_t objectType) {

	BACNET_APPLICATION_TAG tag = BACNET_APPLICATION_TAG_NULL;

	switch (objectType) {
		case OBJECT_ANALOG_INPUT:
			tag = BACNET_APPLICATION_TAG_REAL;
			break;
		case OBJECT_ANALOG_OUTPUT:
			tag = BACNET_APPLICATION_TAG_REAL;
			break;
		case OBJECT_ANALOG_VALUE:
			tag = BACNET_APPLICATION_TAG_REAL;
			break;
		case OBJECT_MULTI_STATE_INPUT:
			tag = BACNET_APPLICATION_TAG_UNSIGNED_INT;
			break;
		case OBJECT_MULTI_STATE_OUTPUT:
			tag = BACNET_APPLICATION_TAG_UNSIGNED_INT;
			break;
		case OBJECT_MULTI_STATE_VALUE:
			tag = BACNET_APPLICATION_TAG_UNSIGNED_INT;
			break;
		default:
			tag = BACNET_APPLICATION_TAG_REAL;
			break;
	}
	return tag;
}

BACNET_OBJECT_ID BacNetVariable::get_bacnet_obj_id(BACNET_OBJECT_ID *obj_id){
	if(obj_id){
		*obj_id = this->object_id;
	}
	return this->object_id;

}

const BACNET_OBJECT_ID *BacNetVariable::get_ptr_bacnet_obj_id(BACNET_OBJECT_ID *obj_id){
	if(obj_id){
		*obj_id = this->object_id;
	}
	return &this->object_id;

}

void BacNetVariable::set_bacnet_obj_id(BACNET_OBJECT_ID bac_obj_id){
	this->object_id = bac_obj_id;
}

uint16_t BacNetVariable::getObjectType() {
	return this->object_id.type;
}

const uint16_t *BacNetVariable::get_ptr_object_type() {
	return &this->object_id.type;
}

void BacNetVariable::setObjectType(uint16_t object_type) {
	this->object_id.type = object_type;
}

uint32_t BacNetVariable::getObject_instance() {
	return this->object_id.instance;
}

const uint32_t *BacNetVariable::get_ptr_object_instance() {
	return &this->object_id.instance;
}

void BacNetVariable::setObject_instance(uint32_t instance) {
	this->object_id.instance = instance;
}

uint32_t BacNetVariable::getProperty_Id() {
	return property_id;
}

const uint32_t *BacNetVariable::get_ptr_property_id() {
	return &property_id;
}

void BacNetVariable::setProperty_Id(uint32_t prop_id) {
	this->property_id = prop_id;
}

int BacNetVariable::get_property_array_index() {
	return this->property_array_index;
}

const int *BacNetVariable::get_ptr_property_array_index() {
	return &this->property_array_index;
}

void BacNetVariable::set_property_array_index(int prop_index){
	this->property_array_index = prop_index;
}

uint8_t BacNetVariable::get_priority() {
	return this->write_priority;
}

const uint8_t *BacNetVariable::get_ptr_priority() {
	return &this->write_priority;
}

void BacNetVariable::set_priority(uint8_t new_priority) {
	this->write_priority = new_priority;
}

class BacNetVariable *BacNetVariable::getNext() {
	return next;
}

void BacNetVariable::setNext(class BacNetVariable *next) {
	this->next = next;
}

bool BacNetVariable::get_is_valid(){
	return this->is_valid;
}

void BacNetVariable::setIsValid(bool val){
	lock();
	this->is_valid = val;
	unlock();
}

BACNET_RECORD *BacNetVariable::get_variable_que(void){
	return this->que;
}

void BacNetVariable::set_variable_que(BACNET_RECORD *new_rec){
	lock();
	this->que = new_rec;
	unlock();
}

void BacNetVariable::delete_variable_que(void){

	lock();
	this->que = NULL;
	unlock();
}

/** Lock the variable for concurrent access */
void BacNetVariable::lock() {
	this->mutex.lock();
}

/** Unock the variable for concurrent access */
void BacNetVariable::unlock() {
	this->mutex.unlock();
}

bool BacNetVariable::get_do_write(void) {
	return this->do_write;
}

void BacNetVariable::set_do_write(bool newValue) {
	lock();
	this->do_write = newValue;
	unlock();
}

class BacNetCOV *BacNetVariable::get_variable_cov(void) {
	return this->cov;
}

BacNetVariable::~BacNetVariable() {
	// TODO Auto-generated destructor stub
}

void BacNetVariable::toString() {

	BacNetEnumToString eStr;

	printf("\tObject-Type: (%u)%s,\tInstance: %u,\tProperty-Id: (%u)%s,\tArray-Index: %d,\tWrite-Priority: %u\n",
			this->getObjectType(), eStr.bactext_object_type_name(this->getObjectType()),
			this->getObject_instance(),
			this->getProperty_Id(), eStr.bactext_property_name(this->getProperty_Id()),
			this->get_property_array_index(),
			this->get_priority());
}

bool BacNetVariable::process(class BacNetConnection *con, class BacNetDevice *dev) {
	BacNetBuffer *buffer = new BacNetBuffer();
	uint8_t id = 0;

	/** See if it is pointing to a que. If so, then we may be still waiting
	 * on a response...*/
	if(!this->get_variable_que()) {
		/** Is it a COV type message?*/
		if(this->cov) {
			/** Has COV time expired?*/
			if(time(NULL) >= (this->cov->get_cov_time() + this->cov->getLife_time())) {
				id = dev->register_var_to_invoke_id(this, BACNET_RECORD::READ);
				if(id > 0) {
					BacNetRequest *req = new BacNetSubscribeCovRequest(this->get_variable_cov()->getProcess_identifier(),
							this->object_id, true, this->cov->getLife_time(), id);

					req->encode_request(buffer);

					this->cov->set_cov_time(time(NULL));

					delete req;
				}
				else {
					/** Could not register variable; table must be full; we only have 254 at a time ya know...*/
					//TODO we need to mark this place in scan-list somehow...
					//So it can pick up where it left off...
					printf("\nCould not register variable...\n");
					return false;
				}
			}
			else {
				/** COV subscription has not expired, keep waiting...*/
			}

		}
		else {
			if(!this->do_write) {
				/** See if we can register it to an invoke-id...*/
				id = dev->register_var_to_invoke_id(this, BACNET_RECORD::READ);
				//printf("\n>>%u<<\n", id);
				if(verbosity_level_2(verbosity_level))
					printf("\nRegistering this InvokeId: %u to var: %p\n", id, this);

				if(id > 0) {
					BacNetRequest *req = new BacNetReadPropertyRequest(this->get_ptr_bacnet_obj_id(NULL),
							this->get_ptr_property_id(), this->get_ptr_property_array_index(), id);

					req->encode_request(buffer);

					delete req;
				}
				else {
					/** Could not register variable; table must be full; we only have 254 at a time ya know...*/
					//TODO we need to mark this place in scan-list somehow...
					//So it can pick up where it left off...
					printf("\nCould not register variable...\n");
					return false;
				}
			}
			else {
				/** This is a "write!"
				 * Write something to device...*/
				/** See if we can register it to an invoke-id...*/
				id = dev->register_var_to_invoke_id(this, BACNET_RECORD::WRITE);
				if(verbosity_level_3(verbosity_level))
					printf("\nRegistering this InvokeId: %u to var: %p, time: %.6lf\n", id, this, (double)(this->get_variable_que()->tx_time.tv_sec + (this->get_variable_que()->tx_time.tv_usec / 1000000.0)));
				if(id > 0) {

					BacNetRequest *req = new BacNetWritePropertyRequest(this->get_ptr_bacnet_obj_id(NULL),
							this->get_ptr_property_id(), this->get_ptr_property_array_index(), id, this->get_ptr_priority(),
							this->BAC_APP_DATA_TYPE, &this->BAC_APP_DATA_VALUE);

					req->encode_request(buffer);

					delete req;
					this->set_do_write(false);
				}
				else {
					/** Could not register variable; table must be full; we only have 254 at a time ya know...*/
					//TODO we need to mark this place in scan-list somehow...
					//So it can pick up where it left off...
					return false;
				}
			}

		}
		if(id > 0) {
			/** Build NPDU...*/
			BacNetNpdu n;
			n.set_npdu_expecting_reply(true);

			if(dev->getSlen()>0){
				n.set_npdu_destination_specifier(true);
				n.set_npdu_dlen(dev->getSlen());
				n.set_npdu_dnet(dev->getSnet());
				n.set_npdu_dadr(dev->getSadr());
				//n->toString();
			}
			n.encode_npdu(buffer);

			if(verbosity_level_3(verbosity_level))
				buffer->hexdump(buffer->getPointerToBuffer(), buffer->getLength());


			//TODO build BVLC...
			BacNetBVLC bvlc;
			bvlc.encode_bvlc_originalUnicastNpdu(buffer);
			if(verbosity_level_3(verbosity_level))
				buffer->hexdump(buffer->getPointerToBuffer(), buffer->getLength());

			int rv = con->sendTo(dev, buffer->getPointerToBuffer(), buffer->getLength(), UNICAST);
			if (verbosity_level_2(verbosity_level)) {
			    printf("rv=%d", rv);
            }
		}
	}
	else {
		/** How long have we been waiting???*/
		timeval tx = this->get_variable_que()->tx_time;
		timeval timeNow;
		gettimeofday(&timeNow, NULL);
		double t1 = tx.tv_sec+(tx.tv_usec/1000000.0);
		double t2 = timeNow.tv_sec+(timeNow.tv_usec/1000000.0);
		double tdiff = t2-t1;
		if(verbosity_level_2(verbosity_level))
			printf("\n%.6lf seconds elapsed; sent: %.6lf, timeNow: %.6lf\n", tdiff, (double)(this->get_variable_que()->tx_time.tv_sec + (this->get_variable_que()->tx_time.tv_usec / 1000000.0)), t2);

		if(tdiff >= (double)(dev->get_apdu_timeout()/1000.0)){
			if(dev->get_retry_count() > dev->get_max_number_apdu_retries()) {
				/** So, that's it huh??? One second you're here, next second you're not!*/
				/** Release the variable to invoke_id...*/
				dev->release_invoke_id_to_var(this);
				/** Clear the retry count...*/
				dev->clear_retry_count();
				/** Maybe device has moved or something? See if he is somewhere...*/
				dev->set_isKnown(false);
				this->setIsValid(false);
				// TODO Increment some sort of network statistics counter...
                if(verbosity_level_2(verbosity_level)) {
				    printf("\nVariable timeout...(%s, %u, %u)\n", dev->getName(), this->getObjectType(), this->getObject_instance());
                }
			}
			else {
				/** Re-send the message...*/
				if(verbosity_level_1(verbosity_level)) {
					printf("\nRETRY, RETRY, RETRY...\n");
					printf("\n%.6lf seconds elapsed; Retry Count %u; id %u; (%u, %u)\n", tdiff, dev->get_retry_count(), this->get_variable_que()->invoke_id,
							this->getObjectType(), this->getObject_instance());
				}
				this->retry(buffer, con, dev);
				dev->increment_retry_count();
			}
		}
		else {
			if(verbosity_level_2(verbosity_level))
				printf("\nHello... device(%s), Variable(%u, %u) is late... Can't wait forever...\n", dev->getName(), this->getObjectType(), this->getObject_instance());
		}
	}

	delete buffer;
	return true;
}

void BacNetVariable::retry(class BacNetBuffer *buffer, class BacNetConnection *con, class BacNetDevice *dev) {

	BacNetRequest *req;

	if(verbosity_level_2(verbosity_level))
		printf("\nRetrying (%u) to send...\n", dev->increment_retry_count());
	gettimeofday(&this->get_variable_que()->tx_time, NULL);

	switch(this->get_variable_que()->m_type) {
	case BACNET_RECORD::READ:
		if(this->cov) {
			req = new BacNetSubscribeCovRequest(this->get_variable_cov()->getProcess_identifier(),
					this->object_id, true, this->cov->getLife_time(), this->get_variable_que()->invoke_id);
			this->cov->set_cov_time(time(NULL));
		}
		else {
		req = new BacNetReadPropertyRequest(this->get_ptr_bacnet_obj_id(NULL),
			this->get_ptr_property_id(), this->get_ptr_property_array_index(), this->get_variable_que()->invoke_id);
		}

		req->encode_request(buffer);

		delete req;
		break;
	case BACNET_RECORD::WRITE:
		req = new BacNetWritePropertyRequest(this->get_ptr_bacnet_obj_id(NULL),
			this->get_ptr_property_id(), this->get_ptr_property_array_index(), this->get_variable_que()->invoke_id,
			this->get_ptr_priority(), this->BAC_APP_DATA_TYPE, &this->BAC_APP_DATA_VALUE);

		req->encode_request(buffer);

		delete req;
		break;
	default:
		break;
	}

	/** Build NPDU...*/
	BacNetNpdu n;
	n.set_npdu_expecting_reply(true);

	if(dev->getSlen()>0){
		n.set_npdu_destination_specifier(true);
		n.set_npdu_dlen(dev->getSlen());
		n.set_npdu_dnet(dev->getSnet());
		n.set_npdu_dadr(dev->getSadr());
	}
	n.encode_npdu(buffer);
	if(verbosity_level_3(verbosity_level)) {
		buffer->hexdump(buffer->getPointerToBuffer(), buffer->getLength());
	}

	//TODO build BVLC...
	BacNetBVLC bvlc;
	bvlc.encode_bvlc_originalUnicastNpdu(buffer);

	if(verbosity_level_3(verbosity_level)) {
		buffer->hexdump(buffer->getPointerToBuffer(), buffer->getLength());
	}

	// mdunning 10/19/17 comment out hexdump below
	//buffer->hexdump(buffer->getPointerToBuffer(), buffer->getLength());
	con->sendTo(dev, buffer->getPointerToBuffer(), buffer->getLength(), UNICAST);
}
