/*
 * BacNetVariableStringTestMain.cpp
 *
 *  Created on: Nov 21, 2011
 *      Author: 8w4
 */

/** System*/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

/** Local*/
#include "bacenum.h"
#include "bacstructures.h"
//#include "BacNetVariable.h"
#include "BacNetVariableString.h"

int main(int argc, char *argv[]) {

	printf("\nLet's create a string variable: new BacNetVariableString(OBJECT_ANALOG_OUTPUT, 1, PROP_PRESENT_VALUE, -1, 0)\n");
	BacNetVariableString *strVar = new BacNetVariableString(OBJECT_ANALOG_OUTPUT, 1, PROP_PRESENT_VALUE, -1, 0);

	assert(strVar != NULL);

	printf("\nCreated Successfully...(%u(OBJECT_ANALOG_OUTPUT), %u(INSTANCE), %u(PROP_PRESENT_VALUE), %d(INDEX), %u(WRITE_PRIORITY))\n",
			strVar->getObjectType(), strVar->getObject_instance(), strVar->getProperty_Id(), strVar->get_property_array_index(), strVar->get_priority());

	printf("\nValue is: %s; set it to this \"Object-Analog\"\n", strVar->getValue());
	strVar->setValue("Object-Analog");

	assert(strcmp(strVar->getValue(), "Object-Analog") == 0);


	printf("\nString value: %s\n", strVar->getValue());


	return 0;
}
