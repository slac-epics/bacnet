/*
 * BacNetTest.cpp
 *
 *  Created on: Oct 28, 2011
 *      Author: 8w4
 */

#include <stdio.h>
#include "DrvBacNet.h"
#include "BacNetVariable.h"
#include "BacNetVariableDouble.h"
#include "BacNetDevice.h"

int main()
{
	//DrvBacNet::initIocAddr("eth0");
	DrvBacNet::setPort(47808);

	DrvBacNet::addBacNetDevice("FS", 11, 0xBAC0, 25);

	BacNetVariable *var = new BacNetVariableDouble(0, 1, 85, -1, 0, NULL);
	BacNetDevice *dev = DrvBacNet::getBacNetDevices();
	dev->addBacNetVariable(var);

	DrvBacNet::listBacNetDeviceInfo(5);

	DrvBacNet::start(1);

	while(true){
		epicsThreadSleep(0.2);
	}


	puts("Testing...");

	// Init
	// DrvBacNet::setPort(0xBAC);
	// Add "bacnet2", 11, 47808)
	// DrvBacNet::start()
	return 0;
}
