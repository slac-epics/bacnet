/*
 * BacNetApplicationValue.h
 *
 *  Created on: Sep 26, 2011
 *      Author: 8w4
 */

#ifndef BACNETAPPLICATIONVALUE_H_
#define BACNETAPPLICATIONVALUE_H_

#include <stdint.h>
#include "bacstructures.h"

class BacNetApplicationValue {
private:
	BACNET_APPLICATION_DATA_VALUE value;
public:
	BacNetApplicationValue();
	virtual ~BacNetApplicationValue();
};

#endif /* BACNETAPPLICATIONVALUE_H_ */
