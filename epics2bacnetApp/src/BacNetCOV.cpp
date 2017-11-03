/*
 * BacNetCOV.cpp
 *
 *  Created on: Dec 21, 2011
 *      Author: 8w4
 */

#include "BacNetCOV.h"

BacNetCOV::BacNetCOV(const uint32_t lifeTime)
:life_time(lifeTime), cov_time(0)
{
	printf("\nCreated BacNetCOV\n");

}

BacNetCOV::~BacNetCOV() {
	// TODO Auto-generated destructor stub
}

const uint32_t BacNetCOV::getLife_time() const
{
    return life_time;
}

void BacNetCOV::printfInfo(void) {
	printf("\nthis: %p, this->life_time: %u\n", this, this->life_time);
}

uint32_t BacNetCOV::getProcess_identifier() const
{
    return process_identifier;
}

void BacNetCOV::setProcess_identifier(uint32_t process_identifier)
{
    this->process_identifier = process_identifier;
}

time_t BacNetCOV::get_cov_time(void) {
	return this->cov_time;
}

void BacNetCOV::set_cov_time(time_t t) {
	this->cov_time = t;
}


