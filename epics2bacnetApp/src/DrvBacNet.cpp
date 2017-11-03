/*
 * DrvBacNet.cpp
 *
 *  Created on: Aug 18, 2011
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

//Local
#include "DrvBacNet.h"
#include "BacNetDevice.h"
#include "BacNetIp.h"
#include "BacNetServer.h"
#include "common.h"


/** Default interface name; incase one is not loaded...*/
char DrvBacNet::ifname[] = "eth0";

/** Default BACnet port number; incase one is not loaded...*/
uint16_t DrvBacNet::port = 0xBAC0;

struct in_addr DrvBacNet::iocAddr;
struct in_addr DrvBacNet::iocBroadcastAddr;
BacNetDevice *DrvBacNet::bacnet_device = 0;
double DrvBacNet::period = 1.0;
int verbosity_level = 0;


void DrvBacNet::addBacNetDevice(const char *name, int instance, const int port, const unsigned int poll_delay)
{
	/** Check to see if there is a duplicate...*/
	if(isDuplicate(name, instance, port))
	{
		printf("\tCould NOT Create device \"%s.\"\n", name);
	}
	else
	{
		printf("\tCreated device \"%s\" successfully...\n", name);
		// List<BacNetdevice> devices;
		// devices.add(new BND())
		bacnet_device = new BacNetDevice(name, instance, port, poll_delay, bacnet_device);
	}

}

bool DrvBacNet::isDuplicate(const char *name, uint32_t instance, const int port)
{
	BacNetDevice *ptr = bacnet_device;

	if(ptr)
		; /** Carry on... We have a valid pointer...*/
	else
	{
		/** Must be start-up; of course it is NULL on start-up...*/
		return false;
	}


	while(ptr)
	{
		if(strcmp(name, ptr->getName()) == 0 || (instance == ptr->getInstance()))
		{
			printf("\tFound a duplicate: %s, %s\n", name, ptr->getName());
			printf("\tFound a duplicate: %u, %u\n", instance, ptr->getInstance());

			return true;
		}
		ptr = ptr->getNext();
	}

	return false;
}

BacNetDevice *DrvBacNet::findBacNetDeviceByName(const char *name) {
	BacNetDevice *dev = bacnet_device;
	while(dev) {
		if(strcmp(dev->getName(), name) == 0)
			return dev;
		dev = dev->getNext();
	}
	return 0;
}

void DrvBacNet::listBacNetDeviceInfo(int level) {

	BacNetDevice *dev = bacnet_device;
	BacNetEnumToString eStr;

	while(dev) {
		printf("\nBACnet device: \"%s\" has %u variables {\n", dev->getName(), dev->get_number_of_variables());
		BacNetVariable *v = dev->getBacNetvariableList();
		int n = 0;
		while(v) {
			printf("\t(Variable-%d)", ++n);
			v->toString();
			v = v->getNext();
		}
		printf("\n\t}\n");
		dev = dev->getNext();
	}

}

void DrvBacNet::start(double period) {

	DrvBacNet::period = period;
	epicsThreadCreate("BacNetDrv",
		epicsThreadPriorityHigh,
		epicsThreadGetStackSize(epicsThreadStackMedium),
		&thread_main, 0);
}

void DrvBacNet::thread_main(void *parm)
{
	timeval driverStart;
	timeval driverNow;

	/** Instantiate a new connection...*/
	BacNetConnection *con = new BacNetConnection(port, DrvBacNet::ifname);
	/** Now create socket and bind...*/
	con->connect();

	/** BacNetServer listens all the time for incoming BACnet messages...*/
	BacNetServer *srvr = new BacNetServer(con, 1, "BACSERVER", DrvBacNet::getBacNetDevices());

	printf("Bacnet Driver Thread started\n");
	while (true)
	{
		BacNetDevice *dev = bacnet_device;
		gettimeofday(&driverStart, NULL);
		while(dev) {
			dev->lock();
			dev->scanBacNetVariables(con);
			dev->unlock();
			dev = dev->getNext();
		}

		// Delay until next scan
		epicsThreadSleep(period);
		gettimeofday(&driverNow, NULL);
		double t1 = driverStart.tv_sec+(driverStart.tv_usec/1000000.0);
		double t2 = driverNow.tv_sec+(driverNow.tv_usec/1000000.0);
		double tdiff = t2-t1;
		//printf("\nDriver Time: %.6lf\n", tdiff);
	}
}

const char *DrvBacNet::getIfname()
{
    return DrvBacNet::ifname;
}

void DrvBacNet::setIfname(const char *ifname)
{
	size_t len = strlen(ifname);
	if(len > MAX_IFNAME)
		printf("\nMaximum network-interface name length is: %u characters,"
				"So... \"%s\" is too long...\n", MAX_IFNAME, ifname);
	strncpy(DrvBacNet::ifname, ifname, MAX_IFNAME);
	DrvBacNet::ifname[49] = '\0';
}

struct in_addr DrvBacNet::getIocAddr() {
	return iocAddr;
}

struct in_addr DrvBacNet::getIocBroadcastAddr() {
	return iocBroadcastAddr;
}

void DrvBacNet::setPort(uint16_t newPort) {
	DrvBacNet::port = newPort;
}

uint16_t DrvBacNet::getPort() {
	return DrvBacNet::port;
}

BacNetDevice *DrvBacNet::getBacNetDevices(void){
	return bacnet_device;
}

void DrvBacNet::set_BACnet_verbosity(int level) {
	printf("\nSetting verbosity_level: %d, level: %d\n", verbosity_level, level);
	verbosity_level = level;
}

void DrvBacNet::get_BACnet_version(void) {
	printf("\nBACnet version for EPICS is: %u.%u\n", BACNET_VERSION_MAJOR, BACNET_VERSION_MINOR);
}

/**Shell Variable Stuff *************************************************************************************
*******************************************************************************************************/

// Export commands to the EPICS IOC shell
#ifdef __cplusplus
extern "C" {
#endif	/* __cplusplus */


// void bacnetDefineServer(const char *name, int instance, const int port)
static const iocshArg bacnetDefineServerArg0 = {"name", iocshArgString };
static const iocshArg bacnetDefineServerArg1 = {"instance", iocshArgInt };
static const iocshArg bacnetDefineServerArg2 = {"port", iocshArgInt };
static const iocshArg bacnetDefineServerArg3 = {"poll_delay", iocshArgInt };
static const iocshArg *const bacnetDefineServerArgs[4] = { &bacnetDefineServerArg0, &bacnetDefineServerArg1,
		&bacnetDefineServerArg2, &bacnetDefineServerArg3 };
static const iocshFuncDef bacnetDefineServerDef = {"bacnetDefineServer", 4, bacnetDefineServerArgs };
static void bacnetDefineServerCall(const iocshArgBuf * args)
{
	DrvBacNet::addBacNetDevice(args[0].sval, args[1].ival, args[2].ival, args[3].ival);
}

//
static const iocshArg bacnetListServerInfoArg0 = {"level", iocshArgInt };
static const iocshArg *const bacnetListServerInfoArgs[1] = { &bacnetListServerInfoArg0 };
static const iocshFuncDef bacnetListServerInfoDef = {"bacnetListServerInfo", 1, bacnetListServerInfoArgs };
static void bacnetListServerInfoCall(const iocshArgBuf * args)
{
	DrvBacNet::listBacNetDeviceInfo(args[0].ival);
}

//
static const iocshArg bacnetDriverInitInterfaceArg0 = {"ifName", iocshArgString };
static const iocshArg *const bacnetDriverInitInterfaceArgs[1] = { &bacnetDriverInitInterfaceArg0 };
static const iocshFuncDef bacnetDriverInitInterfaceDef = {"bacnetDriverInterfaceName", 1, bacnetDriverInitInterfaceArgs };
static void bacnetDriverInitInterfaceCall(const iocshArgBuf * args)
{
	printf("\nOld interface name: \"%s\"\n", DrvBacNet::getIfname());
	DrvBacNet::setIfname(args[0].sval);
	printf("\nNew interface name: \"%s\"\n", DrvBacNet::getIfname());
}

//
static const iocshArg bacnetDriverStartArg0 = {"period", iocshArgDouble };
static const iocshArg *const bacnetDriverStartArgs[1] = { &bacnetDriverStartArg0 };
static const iocshFuncDef bacnetDriverStartDef = {"bacnetDriverStart", 1, bacnetDriverStartArgs };
static void bacnetDriverStartCall(const iocshArgBuf * args)
{
	DrvBacNet::start(args[0].dval);
}

//
static const iocshArg bacnetDriverPortNumberArg0 = {"port", iocshArgInt };
static const iocshArg *const bacnetDriverPortNumberArgs[1] = { &bacnetDriverPortNumberArg0 };
static const iocshFuncDef bacnetDriverPortNumberDef = {"bacnetDriverPortNumber", 1, bacnetDriverPortNumberArgs };
static void bacnetDriverPortNumberCall(const iocshArgBuf * args)
{
	printf("\nOld port: \"%u\"\n", DrvBacNet::getPort());
	DrvBacNet::setPort(args[0].ival);
	printf("\nNew port: \"%u\"\n", DrvBacNet::getPort());
}

//
static const iocshArg bacnetDriverVerbosityArg0 = {"verb_level", iocshArgInt };
static const iocshArg *const bacnetDriverVerbosityArgs[1] = { &bacnetDriverVerbosityArg0 };
static const iocshFuncDef bacnetDriverVerbosityDef = {"bacnetDriverVerbosity", 1, bacnetDriverVerbosityArgs };
static void bacnetDriverVerbosityCall(const iocshArgBuf * args)
{
	DrvBacNet::set_BACnet_verbosity(args[0].ival);
}

//
static const iocshFuncDef bacnetDriverVersionDef = {"bacnetDriverVersion"};
static void bacnetDriverVersionCall(const iocshArgBuf * args)
{
	DrvBacNet::get_BACnet_version();
}

void drvbacnetRegister() {
	iocshRegister(&bacnetDefineServerDef, bacnetDefineServerCall);
	iocshRegister(&bacnetListServerInfoDef, bacnetListServerInfoCall);
	iocshRegister(&bacnetDriverInitInterfaceDef, bacnetDriverInitInterfaceCall);
	iocshRegister(&bacnetDriverStartDef, bacnetDriverStartCall);
	iocshRegister(&bacnetDriverPortNumberDef, bacnetDriverPortNumberCall);
	iocshRegister(&bacnetDriverVerbosityDef, bacnetDriverVerbosityCall);
	iocshRegister(&bacnetDriverVersionDef, bacnetDriverVersionCall);
}

#ifdef __cplusplus
}
#endif	/* __cplusplus */

epicsExportRegistrar(drvbacnetRegister);
