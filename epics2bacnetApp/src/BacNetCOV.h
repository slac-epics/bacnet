/*
 * BacNetCOV.h
 *
 *  Created on: Dec 21, 2011
 *      Author: 8w4
 */

#ifndef BACNETCOV_H_
#define BACNETCOV_H_

#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "bacenum.h"
#include "bacstructures.h"

class BacNetCOV {
private:
	const uint32_t life_time;
	uint32_t process_identifier;
	time_t cov_time;
public:
	BacNetCOV(const uint32_t life_time);
	virtual ~BacNetCOV();
    const uint32_t getLife_time() const;
    void printfInfo(void);
    uint32_t getProcess_identifier() const;
    void setProcess_identifier(uint32_t process_identifier);
    time_t get_cov_time(void);
    void set_cov_time(time_t t);
};

#endif /* BACNETCOV_H_ */
