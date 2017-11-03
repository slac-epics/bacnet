/*
 * drvbacnet.cpp
 *
 *  Created on: Aug 17, 2011
 *      Author: 8w4
 */

// System
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


// EPICS
#include "epicsThread.h"
#include "iocsh.h"
#include "epicsExport.h"

#include "BacNetDevice.h"

class bacnetServer{
private:
	/** Server name, e.g. "iLon1" */
	char *name;

	/** IP address, e.g. "160.91.232.90" */
	int instance;

	/** Port number, e.g. "47808" */
	int port;

	/** Link to next server in the list of known servers */
	class bacnetServer *next;

public:
	bacnetServer(const char *name, const int instance, const int port, bacnetServer *next);
	virtual ~bacnetServer();

};


class bacnetDriver
{
private:
	/** List of all the iLon servers known to the driver */
	static bacnetServer *servers;

	static double period;

	/** The 'main' routine of the driver thread */
	static void thread_main(void *parm);

public:
	/** Add a server definition to the driver */
	static void addServer(const char *name, const int instance, const int port);

	/** Locate a server by name
	 *  @return Server or 0 if not found
	 */
	static bacnetServer *findServer(const char *name);

	/** Start the driver thread */
	static void start(double period);

	/** Report driver information */
	static void report(int level);

	/** Set Server Verbosity*/
	static void setServerVerbosity(const char *name, const int level);

};

bacnetServer *bacnetDriver::servers = 0;

void bacnetDriver::addServer(const char *name, const int instance, const int port)
{
	printf("\tName:%s Instance:%d Port: %d\n", name, instance, port);
}


/**Shell Variable Stuff *************************************************************************************
*******************************************************************************************************/

// Export commands to the EPICS IOC shell
#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */


// void iLonDefineServer(const char *name, const char *ip)
static const iocshArg bacnetDefineServerArg0 = {"name", iocshArgString };
static const iocshArg bacnetDefineServerArg1 = {"instance", iocshArgInt };
static const iocshArg bacnetDefineServerArg2 = {"port", iocshArgInt };
static const iocshArg *const bacnetDefineServerArgs[3] = { &bacnetDefineServerArg0, &bacnetDefineServerArg1, &bacnetDefineServerArg2 };
static const iocshFuncDef bacnetDefineServerDef = {"bacnetDefineServer", 3, bacnetDefineServerArgs };
static void bacnetDefineServerCall(const iocshArgBuf * args)
{
	BacNetDevice d(args[0].sval, args[1].ival, args[2].ival);
	bacnetDriver::addServer(args[0].sval, args[1].ival, args[2].ival);
}

void drvbacnetRegister() {
	iocshRegister(&bacnetDefineServerDef, bacnetDefineServerCall);
}

#ifdef __cplusplus
}
#endif	/* __cplusplus */

epicsExportRegistrar(drvbacnetRegister);
