/*
 * BacNetServer.h
 *
 *  Created on: Sep 2, 2011
 *      Author: 8w4
 */

#ifndef BACNETSERVER_H_
#define BACNETSERVER_H_

//System
#include <stdint.h>

// EPICS
#include "epicsThread.h"

//Local
#include "BacNetConnection.h"
#include "BacNetBuffer.h"
#include "BacNetDevice.h"

class BacNetServer : public epicsThreadRunable{
private:

	class BacNetConnection *con;
	class BacNetDevice *bacnet_device_list;
	epicsThread thread;

public:
	BacNetServer(class BacNetConnection *con, int arg, const char *name, class BacNetDevice *dev_list);
	virtual ~BacNetServer();
	virtual void run();
};

#endif /* BACNETSERVER_H_ */
