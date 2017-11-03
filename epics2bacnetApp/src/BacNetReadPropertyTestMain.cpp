/*
 * BacNetReadPropertyTestMain.cpp
 *
 *  Created on: Nov 17, 2011
 *      Author: 8w4
 */

/** System*/
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

/** Local*/
#include "bacenum.h"
#include "BacNetConnection.h"
#include "BacNetDevice.h"
#include "BacNetServer.h"
#include "BacNetVariableDouble.h"

struct in_addr iocAddr;
struct in_addr iocBroadcastAddr;
static BacNetDevice *dev = 0;

int main(int argc, char *argv[]) {

	BacNetConnection *con = new BacNetConnection(0xBAC0, "eth0");
	con->connect();
	/** BacNetServer listens all the time for incoming BACnet messages...*/

	dev = new BacNetDevice("FieldServer", 11, 0xBAC0, 25, dev);
	BacNetServer *srvr = new BacNetServer(con, 1, "BACSERVER", dev);

	/*
	for(int i=1; i<30; i++){
		BacNetVariable *var_a_i = new BacNetVariableDouble(OBJECT_ANALOG_INPUT, i, PROP_PRESENT_VALUE, -1, 0);
		dev->addBacNetVariable(var_a_i);
	}

	for(int i=1; i<10; i++){
		BacNetVariable *var_b_i = new BacNetVariableDouble(OBJECT_BINARY_INPUT, i, PROP_PRESENT_VALUE, -1, 0);
		dev->addBacNetVariable(var_b_i);
	}
*/
	BacNetVariable *var = new BacNetVariableDouble(OBJECT_ANALOG_INPUT, 1, PROP_PRESENT_VALUE, -1, 0, NULL);
	dev->addBacNetVariable(var);
	//BacNetVariable *var = new BacNetVariableDouble(OBJECT_ANALOG_OUTPUT, 1, PROP_OUT_OF_SERVICE, -1, 0);
	//BacNetVariable *var = new BacNetVariableDouble(OBJECT_ANALOG_OUTPUT, 1, PROP_PRIORITY_ARRAY, 16, 0);
	//BacNetVariable *var = new BacNetVariableDouble(OBJECT_BINARY_OUTPUT, 1, PROP_PRIORITY_ARRAY, 16, 0);
	//BacNetVariable *var = new BacNetVariableDouble(OBJECT_ANALOG_OUTPUT, 1, PROP_PRIORITY_ARRAY, 15, 0);
	//BacNetVariable *var = new BacNetVariableDouble(OBJECT_ANALOG_OUTPUT, 7, PROP_OBJECT_NAME, -1, 0);



	while(true){
		timeval tNow;
		gettimeofday(&tNow, NULL);
		dev->scanBacNetVariables(con);
		printf("\nSEND.tv_sec: %.6lf\n", (double)tNow.tv_sec + (tNow.tv_usec/1000000.0));
		epicsThreadSleep(1);
		var->printValue();
	}

	//var->printValue();


	return 0;
}
