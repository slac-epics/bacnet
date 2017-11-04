#!../../bin/rhel6-x86_64/epics2bacnet

## You may have to change epics2bacnet to something else
## everywhere it appears in this file

< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase "dbd/epics2bacnet.dbd"
epics2bacnet_registerRecordDeviceDriver pdbbase

# You have to provide a Hardware Interface Name that matches the
# name of your hardware... If you don't "eth0" is default...
bacnetDriverInterfaceName("eth0")

#Default and most common BACnet port number...
#"47808" is default...
bacnetDriverPortNumber(47808)


#Define a bacnet Server, Provide a name etc...
#bacnetDefineServer("<someName>", <BACnet instance number>, <BACnet port>, <poll_delay in milliseconds>
#bacnetDefineServer("bacnet1", 10, 47808, 25)
#bacnetDefineServer("FieldServer", 11, 47808, 25)
#bacnetDefineServer("BASremoteMstr", 12, 47808, 25)
#bacnetDefineServer("Viconics", 73010, 47808, 25)
#bacnetDefineServer("BASremoteBill", 2749, 47808, 25)
#bacnetDefineServer("bacnet2", 15, 47808, 25)
#bacnetDefineServer("bacnet5", 73010, 47808, 25)
#bacnetDefineServer("bacnet6", 12, 47809, 25)
#bacnetDefineServer("KMC", 1000, 47808, 25)
bacnetDefineServer("HT", 89, 47808, 25)

## Load record instances
#dbLoadTemplate "db/userHost.substitutions"
#dbLoadRecords "db/test.db", "user=8w4Host"
#dbLoadRecords "db/fieldServer.db", "user=8w4Host"
#dbLoadRecords "db/test_single.db", "user=8w4Host"
dbLoadRecords "db/heatTimer.db", "user=8w4Host"

## Set this to see messages from mySub
#var mySubDebug 1

## Run this to trace the stages of iocInit
#traceIocInit

cd ${TOP}/iocBoot/${IOC}
iocInit

bacnetListServerInfo(5)

bacnetDriverStart(1)

bacnetDriverVerbosity(1)

## Start any sequence programs
#seq sncExample, "user=8w4Host"
