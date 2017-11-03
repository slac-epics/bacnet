/*
 * BacNetDevSupport.cpp
 *
 *  Created on: Aug 22, 2011
 *      Author: 8w4
 */

/** EPICS device support, linking records to BacNet */

// System
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

// EPICS Base
#include "alarm.h"
#include "cvtTable.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "recSup.h"
#include "devSup.h"
#include "link.h"
#include "aiRecord.h"
#include "biRecord.h"
#include "stringinRecord.h"
#include "stringoutRecord.h"
#include "mbbiRecord.h"
#include "mbboRecord.h"
#include "boRecord.h"
#include "aoRecord.h"
#include "epicsExport.h"

// Local
#include "BacNetDevSupport.h"
#include "DrvBacNet.h"
#include "BacNetCOV.h"
#include "BacNetVariableDouble.h"
#include "BacNetVariableDigital.h"
#include "BacNetVariableString.h"
#include "BacNetDevice.h"
#include "common.h"

enum cov_type {
	COV,
	COV_PROPERTY
};

struct cov_t {
	cov_type type;
	uint32_t lifetime;
	double covp_increment;
};

BacNetDevSupport::BacNetDevSupport() {
	// TODO Auto-generated constructor stub

}

BacNetDevSupport::~BacNetDevSupport() {
	// TODO Auto-generated destructor stub
}


/*
class InputLinkParser
{
   class BacNetCOV *covArg;

   char *deviceName;

   InputLinkParser(char *link);
}
*/
static const char *parseInputLink(char *link, char **deviceName, uint32_t *objType,
		uint32_t *objInstance, uint32_t *propertyId, int *objectArrayIndex, uint8_t *priority, class BacNetCOV **covArg) {

	/** Find the end of the first parameter; must be device name...*/
	char *p = strchr(link, ' ');
	/** If no find(?) then complain...*/
	if (!p)
		return "devBacNet is missing device name...\n";

	/** Instantiate name...*/
	char *devName = strndup(link, p - link);

	// Skip spaces after device name...
	while (*p == ' ')
		++p;

	/** Now, next parameter must be object type...*/
	char *p2 = strchr(p, ' ');
	if(!p2)
		return "devBacnet is missing object type...\n";

	/** Instantiate object type...*/
	char *o_type = strndup(p, p2 - p);
	p = p2;

	// Skip spaces after object type...
	while (*p == ' ')
		++p;

	/** Now, next parameter must be object instance...*/
	char *p3 = strchr(p, ' ');
	if(!p3)
		return "devBacnet is missing object instance...\n";

	/** Instantiate object instance...*/
	char *o_instance = strndup(p, p3 - p);
	p = p3;

	// Skip spaces after object instance...
	while (*p == ' ')
		++p;

	/** Now, next parameter must be property id...*/
	char *p_id = 0;
	char *end = strchr(p, '\0');
	if(!end)
		return "devBacnet is missing property id...\n";


	/** Look for space separator between property id and index ref...*/
	char *HasMore = strchr(p, ' ');

	if(HasMore){
		p_id = strndup(p, HasMore - p);
	}
	else{
		p_id = strndup(p, end - p);
	}

	/** If the link being parsed is an output record then
	 * see if there is a write-priority; if not, then default is priority 16*/
	char *HasArrayIndex = strchr(p, 'a');
	char *HasPriority = strchr(p, 'p');
	char *HasCov = strchr(p, 'c');
	char *HasCovP = strchr(p, 'C');

	char *o_index = 0;

	if(HasArrayIndex) {
		p = HasArrayIndex;
		p++;
		while(*p == ' '){
			p++;
		}
		HasMore = strchr(p, ' ');
		if(HasMore) {
			o_index = strndup(p, HasMore - p);
		}
		else{
			o_index = strndup(p, end - p);
		}
	}

	char *w_priority = 0;

	if(HasPriority) {
		p = HasPriority;
		p++;
		while(*p == ' '){
			p++;
		}
		HasMore = strchr(p, ' ');
		if(HasMore) {
			w_priority = strndup(p, HasMore - p);
		}
		else{
			w_priority = strndup(p, end - p);
		}
	}

	/** 'c' is for "Change Of Value." If there, must supply a subscription interval...*/
	char *_cov = 0;

	if(HasCov) {
		p = HasCov;
		p++;
		while(*p == ' '){
			p++;
		}
		HasMore = strchr(p, ' ');
		if(HasMore) {
			_cov = strndup(p, HasMore - p);
		}
		else{
			_cov = strndup(p, end - p);
		}

		*covArg = new BacNetCOV((uint32_t)strtoul(_cov, NULL, 0));
		(*covArg)->printfInfo();
		printf("\nLifetime: %d\n", (*covArg)->getLife_time());
		//covArg->type = COV;
		//covArg->lifetime = (uint32_t)strtoul(cov, NULL, 0);
		free(_cov);
	}

	/** 'C' is for "Change Of Value Property." If there, must supply a subscription interval and COV-increment value*/
	struct {
		char *life;
		char *inc;
	}_covp;

	if(HasCovP) {
		p = HasCovP;
		p++;
		while(*p == ' '){
			p++;
		}
		HasMore = strchr(p, ' ');
		if(HasMore) {
			_covp.life = strndup(p, HasMore - p);
			p = HasMore;
			if(HasMore) {
				_covp.inc = strndup(p, HasMore - p);
			}
			else {
				_covp.inc = strndup(p, end - p);
			}
		}
		else{
			_covp.life = strndup(p, end - p);
		}

		//covArg = (struct cov_t *)malloc(sizeof(cov_t));
		//covArg->type = COV_PROPERTY;
		//covArg->lifetime = (uint32_t)strtoul(_covp.life, NULL, 0);
		//covArg->covp_increment = strtod(_covp.inc, NULL);
		free(_covp.life);
		free(_covp.inc);
	}


	*deviceName = devName;
	*objType = (uint32_t) strtoul(o_type, NULL, 0);
	*objInstance = (uint32_t) strtoul(o_instance, NULL, 0);
	*propertyId = (uint32_t) strtoul(p_id, NULL, 0);

	if(o_index)
		*objectArrayIndex = atoi(o_index);
	if(w_priority)
		*priority = strtoul(w_priority, NULL, 0);

	printf("\ndevName: %s, object_type: %s, object_instance: %s, property_id: %s, array_index: %s, write_priority: %s\n",
			devName, o_type, o_instance, p_id, o_index, w_priority);

	printf("\ndevName: %s, object_type: %u, object_instance: %u, property_id: %u, array_index: %d, write_priority: %u\n",
			*deviceName, *objType, *objInstance, *propertyId, *objectArrayIndex, *priority);

	if(o_type)
		free(o_type);
	if(o_instance)
		free(o_instance);
	if(p_id)
		free(p_id);
	if(o_index)
		free(o_index);
	if(w_priority)
		free(w_priority);

	return 0;
}

/** AI Suppoort...*/
static long init_ai(aiRecord *rec) {
	printf("\n\tInit_AI...\n");
	printf("\n\t%s\n", rec->inp.value.instio.string);

	char *deviceName = 0;
	uint32_t objType = 0;
	uint32_t objInstance = 0;
	uint32_t propertyId = 0;
	int objectArrayIndex = -1;
	uint8_t write_priority = 0;
	class BacNetCOV *cov = NULL;

	const char *error = parseInputLink(rec->inp.value.instio.string, &deviceName, &objType, &objInstance, &propertyId, &objectArrayIndex, &write_priority,
			&cov);
	printf("\n\terror: %s\n", error);

	BacNetDevice *device = DrvBacNet::findBacNetDeviceByName(deviceName);

	if(device)
		printf("\n\tYo! I found this device: %s\n", device->getName());
	else {
		fprintf (stderr, "\n\"%s\" is not found in the device-list, check st.cmd\n", deviceName);
		fprintf(stderr,"\nThis is my list:\n");
		DrvBacNet::listBacNetDeviceInfo(5);
		exit(1);
	}

	free(deviceName);

	if(cov)
		printf("\nWe Have COV: %u\n", cov->getLife_time());
	printf("\n%p\n", cov);
	BacNetVariable *var = new BacNetVariableDouble(objType, objInstance, propertyId, objectArrayIndex, 0, cov);
	device->addBacNetVariable(var);
	// Remember variable in 'device private'
	rec->dpvt = var;
	return 0;
}

static long read_ai(aiRecord *rec) {

	if (rec->tpro)
		printf("\nProcessing BACnet record '%s'\n", rec->name);
	// Get BACnetVariable from 'device private'
	BacNetVariableDouble *var = (BacNetVariableDouble *)rec->dpvt;
	if (!var)
		return 2;

	// If it has a current value, put that into record's VAL field
	var->lock();
	if (var->get_is_valid())
	{
		rec->val = var->getValue();
		rec->udf = 0;
	}
	else
	{
		recGblSetSevr(rec, READ_ALARM, INVALID_ALARM);
	}
	var->unlock();
	return 2;
}

/** BI Suppoort...*/
static long init_bi(biRecord *rec) {
	printf("\n\tInit_BI...\n");
	printf("\n\t%s\n", rec->inp.value.instio.string);

	char *deviceName = 0;
	uint32_t objType = 0;
	uint32_t objInstance = 0;
	uint32_t propertyId = 0;
	int objectArrayIndex = -1;
	uint8_t write_priority = 0;
	class BacNetCOV *cov = NULL;

	const char *error = parseInputLink(rec->inp.value.instio.string, &deviceName, &objType, &objInstance, &propertyId, &objectArrayIndex, &write_priority,
			&cov);
	printf("\n\terror: %s\n", error);

	BacNetDevice *device = DrvBacNet::findBacNetDeviceByName(deviceName);
	if(device)
		printf("\n\tYo! I found this device: %s\n", device->getName());
	else {
		fprintf (stderr, "\n\"%s\" is not found in the device-list, check st.cmd\n", deviceName);
		fprintf(stderr,"\nThis is my list:\n");
		DrvBacNet::listBacNetDeviceInfo(5);
		exit(1);
	}

	free(deviceName);

	if(cov)
		printf("\nWe Have COV: %u\n", cov->getLife_time());
	printf("\n%p\n", cov);

	BacNetVariable *var = new BacNetVariableDigital(objType, objInstance, propertyId, objectArrayIndex, 0, cov);
	device->addBacNetVariable(var);
	// Remember variable in 'device private'
	rec->dpvt = var;
	return 0;
}

static long read_bi(biRecord *rec) {

	if (rec->tpro)
		printf("\nProcessing BACnet record '%s'\n", rec->name);
	// Get BACnetVariable from 'device private'
	BacNetVariableDigital *var = (BacNetVariableDigital *)rec->dpvt;
	if (!var)
		return 2;

	// If it has a current value, put that into record's VAL field
	var->lock();
	if (var->get_is_valid())
	{
		rec->val = (var->getValue() > 0) ? 1 : 0;
		rec->udf = 0;
	}
	else
	{
		recGblSetSevr(rec, READ_ALARM, INVALID_ALARM);
	}
	var->unlock();
	return 2;
}

/** MBBI Suppoort...*/
static long init_mbbi(mbbiRecord *rec) {
	printf("\n\tInit_MBBI...\n");
	printf("\n\t%s\n", rec->inp.value.instio.string);

	char *deviceName = 0;
	uint32_t objType = 0;
	uint32_t objInstance = 0;
	uint32_t propertyId = 0;
	int objectArrayIndex = -1;
	uint8_t write_priority = 0;
	class BacNetCOV *cov = NULL;

	const char *error = parseInputLink(rec->inp.value.instio.string, &deviceName, &objType, &objInstance, &propertyId, &objectArrayIndex, &write_priority,
			&cov);
	printf("\n\terror: %s\n", error);

	BacNetDevice *device = DrvBacNet::findBacNetDeviceByName(deviceName);

	if(device)
		printf("\n\tYo! I found this device: %s\n", device->getName());
	else {
		fprintf (stderr, "\n\"%s\" is not found in the device-list, check st.cmd\n", deviceName);
		fprintf(stderr,"\nThis is my list:\n");
		DrvBacNet::listBacNetDeviceInfo(5);
		exit(1);
	}

	free(deviceName);

	BacNetVariable *var = new BacNetVariableDigital(objType, objInstance, propertyId, objectArrayIndex, 0, cov);
	device->addBacNetVariable(var);
	// Remember variable in 'device private'
	rec->dpvt = var;
	return 0;
}

static long read_mbbi(mbbiRecord *rec) {

	if (rec->tpro)
		printf("\nProcessing BACnet record '%s'\n", rec->name);
	// Get BACnetVariable from 'device private'
	BacNetVariableDigital *var = (BacNetVariableDigital *)rec->dpvt;
	if (!var)
		return 2;

	// If it has a current value, put that into record's VAL field
	var->lock();
	if (var->get_is_valid())
	{
		rec->val = var->getValue();
		rec->udf = 0;
	}
	else
	{
		recGblSetSevr(rec, READ_ALARM, INVALID_ALARM);
	}
	var->unlock();
	return 2;
}

/** AO support...*/
static long init_ao(aoRecord *rec)
{
	printf("\n\tInit_AO...\n");
	printf("\n\t%s\n", rec->out.value.instio.string);

	char *deviceName = 0;
	uint32_t objType = 0;
	uint32_t objInstance = 0;
	uint32_t propertyId = 0;
	int objectArrayIndex = -1;
	uint8_t priority = 16;
	class BacNetCOV *cov = NULL;

	const char *error = parseInputLink(rec->out.value.instio.string, &deviceName, &objType, &objInstance, &propertyId, &objectArrayIndex, &priority,
			&cov);
	printf("\n\terror: %s\n", error);

	BacNetDevice *device = DrvBacNet::findBacNetDeviceByName(deviceName);

	if(device)
		printf("\n\tYo! I found this device: %s\n", device->getName());
	else {
		fprintf (stderr, "\n\"%s\" is not found in the device-list, check st.cmd\n", deviceName);
		fprintf(stderr,"\nThis is my list:\n");
		DrvBacNet::listBacNetDeviceInfo(5);
		exit(1);
	}

	free(deviceName);
	BacNetVariable *var = new BacNetVariableDouble(objType, objInstance, propertyId, objectArrayIndex, priority, NULL);
	device->addBacNetVariable(var);
	// Remember variable in 'device private'
	rec->dpvt = var;

    return 0;
}

static long write_ao(aoRecord *rec)
{

	if (rec->tpro)
		printf("\nProcessing BACnet record '%s'\n", rec->name);
	// Get BACnetVariable from 'device private'
	BacNetVariableDouble *var = (BacNetVariableDouble *)rec->dpvt;
	if (!var)
		return 2;

	// If it has a current value, put that into record's VAL field
	var->lock();
	var->write_value(rec->val);
	var->unlock();
	return 0;
}

/** BO support...*/
static long init_bo(boRecord *rec)
{
	printf("\n\tInit_BO...\n");
	printf("\n\t%s\n", rec->out.value.instio.string);

	char *deviceName = 0;
	uint32_t objType = 0;
	uint32_t objInstance = 0;
	uint32_t propertyId = 0;
	int objectArrayIndex = -1;
	uint8_t priority = 16;
	class BacNetCOV *cov = NULL;

	const char *error = parseInputLink(rec->out.value.instio.string, &deviceName, &objType, &objInstance, &propertyId, &objectArrayIndex, &priority,
			&cov);
	printf("\n\terror: %s\n", error);

	BacNetDevice *device = DrvBacNet::findBacNetDeviceByName(deviceName);

	if(device)
		printf("\n\tYo! I found this device: %s\n", device->getName());
	else {
		fprintf (stderr, "\n\"%s\" is not found in the device-list, check st.cmd\n", deviceName);
		fprintf(stderr,"\nThis is my list:\n");
		DrvBacNet::listBacNetDeviceInfo(5);
		exit(1);
	}

	free(deviceName);
	BacNetVariable *var = new BacNetVariableDigital(objType, objInstance, propertyId, objectArrayIndex, priority, NULL);
	device->addBacNetVariable(var);
	// Remember variable in 'device private'
	rec->dpvt = var;

    return 0;
}

static long write_bo(boRecord *rec)
{

	if (rec->tpro)
		printf("\nProcessing BACnet record '%s'\n", rec->name);
	// Get BACnetVariable from 'device private'
	BacNetVariableDigital *var = (BacNetVariableDigital *)rec->dpvt;
	if (!var)
		return 2;

	// If it has a current value, put that into record's VAL field
	var->lock();
	var->write_value(rec->val);
	var->unlock();
	return 0;
}

static long init_stringin(stringinRecord *rec)
{
	printf("\n\tInit_STRING_IN...\n");
	printf("\n\t%s\n", rec->inp.value.instio.string);

	char *deviceName = 0;
	uint32_t objType = 0;
	uint32_t objInstance = 0;
	uint32_t propertyId = 0;
	int objectArrayIndex = -1;
	uint8_t priority = 16;
	class BacNetCOV *cov = NULL;

	const char *error = parseInputLink(rec->inp.value.instio.string, &deviceName, &objType, &objInstance, &propertyId, &objectArrayIndex, &priority,
			&cov);
	printf("\n\terror: %s\n", error);

	BacNetDevice *device = DrvBacNet::findBacNetDeviceByName(deviceName);

	if(device)
		printf("\n\tYo! I found this device: %s\n", device->getName());
	else {
		fprintf (stderr, "\n\"%s\" is not found in the device-list, check st.cmd\n", deviceName);
		fprintf(stderr,"\nThis is my list:\n");
		DrvBacNet::listBacNetDeviceInfo(5);
		exit(1);
	}

	free(deviceName);
	BacNetVariable *var = new BacNetVariableString(objType, objInstance, propertyId, objectArrayIndex, priority);
	device->addBacNetVariable(var);
	// Remember variable in 'device private'
	rec->dpvt = var;

    return 0;
}

static long read_stringin(stringinRecord *rec) {

	if (rec->tpro)
		printf("\nProcessing BACnet record '%s'\n", rec->name);
	// Get BACnetVariable from 'device private'
	BacNetVariableString *var = (BacNetVariableString *)rec->dpvt;
	if (!var)
		return 2;

	// If it has a current value, put that into record's VAL field
	var->lock();
	if (var->get_is_valid())
	{
		strncpy(rec->val, var->getValue(), 40);
		rec->val[39] = '\0';
		rec->udf = 0;
	}
	else
	{
		recGblSetSevr(rec, READ_ALARM, INVALID_ALARM);
	}
	var->unlock();
	return 2;
}

/* Create the dset for devAiBacNet */
struct
{
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read;
        DEVSUPFUN       special_linconv;
}  devAiBacNet =
{
        6,
        NULL,
        NULL,
        (DEVSUPFUN)init_ai,
        NULL,
        (DEVSUPFUN)read_ai,
        NULL
};

/* Create the dset for devBiBacNet */
struct
{
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read;
        DEVSUPFUN       special_linconv;
}  devBiBacNet =
{
        6,
        NULL,
        NULL,
        (DEVSUPFUN)init_bi,
        NULL,
        (DEVSUPFUN)read_bi,
        NULL
};

/* Create the dset for devStringinBacNet */
struct
{
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read;
        DEVSUPFUN       special_linconv;
}  devStringInBacNet =
{
        6,
        NULL,
        NULL,
        (DEVSUPFUN)init_stringin,
        NULL,
        (DEVSUPFUN)read_stringin,
        NULL
};

/* Create the dset for devAoBacnet */
struct
{
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       write;
        DEVSUPFUN       special_linconv;
}  devAoBacNet =
{
        6,
        NULL,
        NULL,
        (DEVSUPFUN)init_ao,
        NULL,
        (DEVSUPFUN)write_ao,
        NULL
};

/* Create the dset for devBoBacnet */
struct
{
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       write;
        DEVSUPFUN       special_linconv;
}  devBoBacNet =
{
        6,
        NULL,
        NULL,
        (DEVSUPFUN)init_bo,
        NULL,
        (DEVSUPFUN)write_bo,
        NULL
};

/* Create the dset for devMbbiBacNet */
struct
{
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read;
        DEVSUPFUN       special_linconv;
}  devMbbiBacNet =
{
        6,
        NULL,
        NULL,
        (DEVSUPFUN)init_mbbi,
        NULL,
        (DEVSUPFUN)read_mbbi,
        NULL
};


epicsExportAddress(dset, devAiBacNet);
epicsExportAddress(dset, devBiBacNet);
epicsExportAddress(dset, devMbbiBacNet);
epicsExportAddress(dset, devAoBacNet);
epicsExportAddress(dset, devBoBacNet);
epicsExportAddress(dset, devStringInBacNet);
