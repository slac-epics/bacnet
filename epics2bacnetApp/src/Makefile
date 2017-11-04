TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================
# No optimization, we want debugging info:
HOST_OPT=NO
CROSS_OPT=NO

LIBRARY += bacnetlib

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


# Buid a test to test the whole driver whithout using EPICS.
#PROD_HOST += bacnet_test
#bacnet_test_LIBS += $(EPICS_BASE_IOC_LIBS)
#bacnet_test_SRCS += BacNetTestMain.cpp
#bacnet_test_LIBS += bacnetlib

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

#PROD_HOST += bacnet_whoIs_test
#bacnet_whoIs_test_LIBS += $(EPICS_BASE_IOC_LIBS)
#bacnet_whoIs_test_LIBS += bacnetlib
#bacnet_whoIs_test_SRCS += BacNetWhoIsTestServer.cpp
#bacnet_whoIs_test_SRCS += BacNetWhoIsTestMain.cpp

#PROD_HOST += bacnet_cov_test
#bacnet_cov_test_LIBS += $(EPICS_BASE_IOC_LIBS)
#bacnet_cov_test_LIBS += bacnetlib
#bacnet_cov_test_SRCS += BacNetSubscribeCOVtestServer.cpp
#bacnet_cov_test_SRCS += BacNetSubscribeCOVtestMain.cpp

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

