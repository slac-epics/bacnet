TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
# No optimization, we want debugging info:
HOST_OPT=NO
CROSS_OPT=NO
#=============================
# Build the IOC application

PROD_IOC = epics2bacnet
# epics2bacnet.dbd will be created and installed
DBD += epics2bacnet.dbd

# epics2bacnet.dbd will be made up from these files:
epics2bacnet_DBD += base.dbd

# Include dbd files from all support applications:
epics2bacnet_DBD += xxx.dbd

# Add all the support libraries needed by this IOC
#epics2bacnet_LIBS += xxx

LIBRARY += bacnetlib
bacnetlib_SRCS += epics2bacnet_registerRecordDeviceDriver.cpp

bacnetlib_SRCS += BacNetApplicationValue.cpp
bacnetlib_SRCS += BacNetServer.cpp
bacnetlib_SRCS += BacNetConnection.cpp
bacnetlib_SRCS += BacNetBuffer.cpp
bacnetlib_SRCS += BacNetBVLC.cpp
bacnetlib_SRCS += BacNetApdu.cpp
bacnetlib_SRCS += BacNetNpdu.cpp
bacnetlib_SRCS += BacNetComplexACK.cpp
bacnetlib_SRCS += BacNetSimpleACK.cpp
bacnetlib_SRCS += BacNetUnconfirmedRequestPdu.cpp
bacnetlib_SRCS += BacNetConfirmedRequestPdu.cpp
bacnetlib_SRCS += BacNetConfirmedService.cpp
bacnetlib_SRCS += BacNetObjectIdentifier.cpp
bacnetlib_SRCS += BacNetAbstractSyntaxAndType.cpp
bacnetlib_SRCS += BacNetPropertyValue.cpp
bacnetlib_SRCS += BacNetReadPropertyRequest.cpp
bacnetlib_SRCS += BacNetSubscribeCovRequest.cpp
bacnetlib_SRCS += BacNetCOV.cpp
bacnetlib_SRCS += BacNetConfirmedCovNotification.cpp
bacnetlib_SRCS += BacNetWritePropertyRequest.cpp
bacnetlib_SRCS += BacNetReadPropertyACK.cpp
bacnetlib_SRCS += BacNetWhoIsRequest.cpp
bacnetlib_SRCS += BacNet_I_Am_Request.cpp
bacnetlib_SRCS += BacNetIp.cpp
bacnetlib_SRCS += BacNetVariableDouble.cpp
bacnetlib_SRCS += BacNetVariable.cpp
bacnetlib_SRCS += BacNetVariableDigital.cpp
bacnetlib_SRCS += BacNetVariableString.cpp
bacnetlib_SRCS += BacNetDevice.cpp
bacnetlib_SRCS += DrvBacNet.cpp
bacnetlib_SRCS += BacNetDevSupport.cpp
bacnetlib_SRCS += BacNetRecordManager.cpp
bacnetlib_SRCS += BacNetRequest.cpp
bacnetlib_SRCS += Response.cpp
bacnetlib_SRCS += BacNetResponse.cpp
bacnetlib_SRCS += BacNetError.cpp
bacnetlib_SRCS += BacNetEnumToString.cpp
bacnetlib_SRCS += indtext.c

# epics2bacnet_registerRecordDeviceDriver.cpp derives from epics2bacnet.dbd
epics2bacnet_SRCS += epics2bacnet_registerRecordDeviceDriver.cpp


# Build the main IOC entry point on workstation OSs.
epics2bacnet_SRCS_DEFAULT += epics2bacnetMain.cpp
epics2bacnet_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#epics2bacnet_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

# Finally link to the EPICS Base libraries
epics2bacnet_LIBS += $(EPICS_BASE_IOC_LIBS)
epics2bacnet_LIBS += bacnetlib

# Buid a test to test the whole driver whithout using EPICS.
PROD_HOST += bacnet_test
bacnet_test_LIBS += $(EPICS_BASE_IOC_LIBS)
bacnet_test_LIBS += bacnetlib
bacnet_test_SRCS += BacNetTestMain.cpp

# Test all of the sources...

#PROD_HOST += bacnet_recordManager_test
#bacnet_recordManager_test_LIBS += $(EPICS_BASE_IOC_LIBS)
#bacnet_recordManager_test_LIBS += bacnetlib
#bacnet_recordManager_test_SRCS += BacNetRecordManagerTestMain.cpp

#PROD_HOST += bacnet_readProperty_test
#bacnet_readProperty_test_LIBS += $(EPICS_BASE_IOC_LIBS)
#bacnet_readProperty_test_LIBS += bacnetlib
#bacnet_readProperty_test_SRCS += BacNetReadPropertyTestMain.cpp

#PROD_HOST += bacnet_variableString_test
#bacnet_variableString_test_LIBS += $(EPICS_BASE_IOC_LIBS)
#bacnet_variableString_test_LIBS += bacnetlib
#bacnet_variableString_test_SRCS += BacNetVariableStringTestMain.cpp

#PROD_HOST += bacnet_enumToString_test
#bacnet_enumToString_test_LIBS += $(EPICS_BASE_IOC_LIBS)
#bacnet_enumToString_test_LIBS += bacnetlib
#bacnet_enumToString_test_SRCS += BacNetEnumToStringTestMain.cpp

PROD_HOST += bacnet_whoIs_test
bacnet_whoIs_test_LIBS += $(EPICS_BASE_IOC_LIBS)
bacnet_whoIs_test_LIBS += bacnetlib
bacnet_whoIs_test_SRCS += BacNetWhoIsTestServer.cpp
bacnet_whoIs_test_SRCS += BacNetWhoIsTestMain.cpp

#PROD_HOST += bacnet_cov_test
#bacnet_cov_test_LIBS += $(EPICS_BASE_IOC_LIBS)
#bacnet_cov_test_LIBS += bacnetlib
#bacnet_cov_test_SRCS += BacNetSubscribeCOVtestServer.cpp
#bacnet_cov_test_SRCS += BacNetSubscribeCOVtestMain.cpp

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

