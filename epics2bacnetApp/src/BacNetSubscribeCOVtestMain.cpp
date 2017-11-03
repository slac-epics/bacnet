/*
 * BacNetSubscribeCOVtestMain.cpp
 *
 *  Created on: Dec 15, 2011
 *      Author: Brad Webb
 */

/** System*/
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

/** Local*/
#include "bacenum.h"
#include "BacNetConnection.h"
#include "BacNetDevice.h"
#include  "BacNetVariableDouble.h"
#include "BacNetSubscribeCOVtestServer.h"

int main(int argc, char *argv[]) {
	BacNetConnection *con = new BacNetConnection(0xBAC0, "eth0");
	con->connect();

	BacNetDevice *dev = new BacNetDevice("FieldServer", 11, 0xBAC0, 25, dev);

	//uint8_t devMac[6] = {0xA0, 0x5B, 0xEC, 0x7A, 0xBA, 0xC0};
	//dev->setDevMac(devMac, 6);

	/** BacNetServer listens all the time for incoming BACnet messages...*/
	BacNetSubscribeCOVtestServer *srvr = new BacNetSubscribeCOVtestServer(con, 1, "BACSERVER", dev);

	BacNetVariable *var = new BacNetVariableDouble(OBJECT_ANALOG_INPUT, 1, PROP_PRESENT_VALUE, -1, 0, NULL);
	dev->addBacNetVariable(var);

	dev->scanBacNetVariables(con);

	/** Build a raw COV packet...*/

	uint8_t pdu[21];
	pdu[0] = 0x81;	/** BVLC*/
	pdu[1] = 0x0A;
	pdu[2] = 0x00;
	pdu[3] = 0x15;

	pdu[4] = 0x01;	/** NPDU*/
	pdu[5] = 0x04;

	pdu[6] = 0x00;	/** APDU*/
	pdu[7] = 0x05;
	pdu[8] = 0x01;
	pdu[9] = 0x05;

	pdu[10] = 0x09;
	pdu[11] = 0xC8;
	pdu[12] = 0x1C;
	pdu[13] = 0x00;	/** Object-ID*/
	pdu[14] = 0x00;
	pdu[15] = 0x00;
	pdu[16] = 0x01;
	pdu[17] = 0x29;
	pdu[18] = 0x01;	/** Confirmed notifications please...*/
	pdu[19] = 0x39;
	pdu[20] = 0x3C;	//60 sec...
/*	pdu[21] = 0x4E;
	pdu[22] = 0x09;
	pdu[23] = 0x55;
	pdu[24] = 0x4F;
	pdu[25] = 0x5C;

	pdu[26] = 0x3F;
	pdu[27] = 0x80;
	pdu[28] = 0x00;
	pdu[29] = 0x00;
*/
	size_t s = sizeof(pdu);
	con->sendTo(dev, &pdu[0], s, UNICAST);

	while(true){
		epicsThreadSleep(1);
	}

	return 0;
}
