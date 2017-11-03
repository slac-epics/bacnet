/*
 * BacNetEnumToStringTestMain.cpp
 *
 *  Created on: Nov 30, 2011
 *      Author: 8w4
 */

#include <stdio.h>

#include "BacNetEnumToString.h"

int main(int argc, char *argv[]) {

	printf("\nBacNetEnumToStringTestMain.cpp\n");

	BacNetEnumToString estr;

	const char *myChar = estr.bactext_property_name(85);

	printf("\nestr->bactext_confirmed_service_name(12) = %s\n", myChar);

	return 0;
}
