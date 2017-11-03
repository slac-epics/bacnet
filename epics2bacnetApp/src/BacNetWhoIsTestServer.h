/*
 * BacNetServer.h
 *
 *  Created on: Sep 2, 2011
 *      Author: 8w4
 */

#ifndef BACNETWHOISTESTSERVER_H_
#define BACNETWHOISTESTSERVER_H_

//System
#include <stdint.h>

// EPICS
#include "epicsThread.h"

//Local
#include "BacNetConnection.h"
#include "BacNetBuffer.h"
#include "BacNetDevice.h"

class BacNetWhoIsTestServer : public epicsThreadRunable{
private:

	class BacNetConnection *con;
	class BacNetDevice *bacnet_device_list;
	epicsThread thread;
public:
	BacNetWhoIsTestServer(class BacNetConnection *con, int arg, const char *name, class BacNetDevice *dev_list);
	class BacNetDevice *get_bacnet_device_list();
	virtual ~BacNetWhoIsTestServer();
	virtual void run();
};

#endif /* BACNETWHOISTESTSERVER_H_ */
