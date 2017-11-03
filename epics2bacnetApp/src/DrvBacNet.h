/*
 * DrvBacNet.h
 *
 *  Created on: Aug 18, 2011
 *      Author: 8w4
 */

#ifndef DRVBACNET_H_
#define DRVBACNET_H_

#include <stdint.h>

// EPICS
#include "epicsThread.h"


#define BACNET_VERSION_MAJOR 0
#define BACNET_VERSION_MINOR 8

#define MAX_IFNAME 50

class DrvBacNet {
private:

	static class BacNetDevice *bacnet_device;		/** A device in the field...*/
	class BacNetServer *bacnet_server;				/** Server thread that listens all the time...*/
	class BacNetConnection *bacnet_connection;		/** Class that creates a socket...*/
	static struct in_addr iocAddr;					/** Local IP Addr...*/
	static struct in_addr iocBroadcastAddr;			/** Local IP Broadcast Addr...*/
	static double period;							/** Execution rate...*/
	static uint16_t port;							/** Port used for communication...*/
	static char ifname[MAX_IFNAME];							/** Network interface name used...*/
	static int BACnet_verbosity;					/** Debugging...*/
	/** The 'main' routine of the driver thread */
	static void thread_main(void *parm);

public:

	/** Add a BacNetDevice definition to the driver */
	static void addBacNetDevice(const char *name, const int instance, const int port, const unsigned int poll_delay);

	static bool isDuplicate(const char *name, uint32_t instance, const int port);

	static BacNetDevice *findBacNetDeviceByName(const char *name);

	static void listBacNetDeviceInfo(int level);

	/** Start the driver thread */
	static void start(double period);

	/** Report driver information */
	static void report(int level);

	/** Set Server Verbosity*/
	static void setServerVerbosity(const char *name, const int level);

	static void setPort(uint16_t port);
	static uint16_t getPort(void);

	static struct in_addr getIocAddr();
	static struct in_addr getIocBroadcastAddr();

	static BacNetDevice *getBacNetDevices();

	static void set_BACnet_verbosity(int level);

	static void get_BACnet_version(void);

    static const char *getIfname(void);
    static void setIfname(const char *ifname);
};

#endif /* DRVBACNET_H_ */
