/*
 * BacNetObjectIdentifier.h
 *
 *  Created on: Aug 26, 2011
 *      Author: 8w4
 */

#ifndef BACNETOBJECTIDENTIFIER_H_
#define BACNETOBJECTIDENTIFIER_H_
#include <stdint.h>
#include "bacenum.h"

class BacNetObjectIdentifier {
private:
	BACNET_OBJECT_TYPE objectType;
	uint32_t instanceNumber;
public:
	BacNetObjectIdentifier(BACNET_OBJECT_TYPE object_type, uint32_t instance_number);
	BACNET_OBJECT_TYPE getObjectType();
	uint32_t getInstanceNumber();
	class BacNetObjectIdentifier *getBacNetObjID();
	virtual ~BacNetObjectIdentifier();
};

#endif /* BACNETOBJECTIDENTIFIER_H_ */
