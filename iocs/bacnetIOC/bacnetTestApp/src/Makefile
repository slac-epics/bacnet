TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#=====================================================
# Path to "NON EPICS" External PACKAGES: USER INCLUDES
#======================================================

#======================================================
# PATH TO "NON EPICS" EXTERNAL PACKAGES: USER LIBRARIES
#======================================================

# ======================================================
# LINK "NON EPICS" EXTERNAL PACKAGE LIBRARIES STATICALLY
#=======================================================

#=============================
# Build the IOC application
PROD_NAME = epics2bacnet
PROD_IOC = $(PROD_NAME)

DBD += $(PROD_NAME).dbd

# $(PROD_NAME).dbd will be made up from these files:
$(PROD_NAME)_DBD += base.dbd

# Include dbd files from all support modules:
$(PROD_NAME)_DBD += bacnetSupport.dbd

# Add all the support libraries needed by this IOC
$(PROD_NAME)_LIBS += bacnetlib

# $(PROD_NAME)_registerRecordDeviceDriver.cpp derives from $(PROD_NAME).dbd
PROD_SRCS += $(PROD_NAME)_registerRecordDeviceDriver.cpp $(PROD_NAME)Main.cpp

# Finally link to the EPICS Base libraries
$(PROD_NAME)_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

