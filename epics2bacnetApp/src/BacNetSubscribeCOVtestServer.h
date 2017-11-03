/*
 * BacNetSubscribeCOVtestServer.h
 *
 *  Created on: Dec 15, 2011
 *      Author: 8w4
 */

#ifndef BACNETSUBSCRIBECOVTESTSERVER_H_
#define BACNETSUBSCRIBECOVTESTSERVER_H_

//System
#include <stdint.h>

// EPICS
#include "epicsThread.h"

//Local
#include "BacNetConnection.h"
#include "BacNetBuffer.h"
#include "BacNetDevice.h"

class BacNetSubscribeCOVtestServer : public epicsThreadRunable{
private:

	class BacNetConnection *con;
	class BacNetDevice *bacnet_device_list;

public:
	BacNetSubscribeCOVtestServer(class BacNetConnection *con, int arg, const char *name, class BacNetDevice *dev_list);
	class BacNetDevice *get_bacnet_device_list();
	virtual ~BacNetSubscribeCOVtestServer();
	virtual void run();
	epicsThread thread;
};

#endif /* BACNETSUBSCRIBECOVTESTSERVER_H_ */
