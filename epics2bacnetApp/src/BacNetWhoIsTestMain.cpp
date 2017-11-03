/*
 * BacNetWhoIsTestMain.cpp
 *
 *  Created on: Dec 6, 2011
 *      Author: 8w4
 */

/** System*/
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

/** Local*/
#include "bacenum.h"
#include "BacNetConnection.h"
#include "BacNetDevice.h"
#include "BacNetWhoIsTestServer.h"
#include "BacNetBuffer.h"
#include "BacNetRequest.h"
#include "BacNetWhoIsRequest.h"
#include "BacNetVariable.h"
#include "BacNetVariableString.h"

struct in_addr iocAddr;
struct in_addr iocBroadcastAddr;

#define BACNET_MAX_INSTANCE 0x3FFFFF

typedef struct {
	int32_t Min;
	int32_t Max;
} Target_Object_Instance;


Target_Object_Instance ins = {0, BACNET_MAX_INSTANCE};
Target_Object_Instance *instance = &ins;

static int debugLevel;

char *filename_remove_path(char *filename_in)
{
    char *filename_out = (char *) filename_in;

    /* allow the device ID to be set */
    if (filename_in) {
        filename_out = strrchr(filename_in, '\\');
        if (!filename_out) {
            filename_out = strrchr(filename_in, '/');
        }
        /* go beyond the slash */
        if (filename_out) {
            filename_out++;
        } else {
            /* no slash in filename */
            filename_out = (char *) filename_in;
        }
    }

    return filename_out;
}

void debugInit(void)
{
	/** Do something to initialize...*/
	debugLevel = 0;
}

void setDebugLevel(int level)
{
	debugLevel = level;
	printf("Debug level is set to: %d\n", debugLevel);
}

int getDebugLevel(void)
{
	return debugLevel;
}

void printArgs(int argc, char *argv[])
{
	int i;
	for(i=0; i<argc; i++)
		printf("arg[%d]: %s\n", i, argv[i]);
}

static void printUsage(int type, char *name)
{
	switch(type)
	{
		case 1:
			printf("Send BACnet WhoIs service request to a device or\r\n"
				"multiple devices, and wait for responses.  Displays\r\n"
				"any devices found and their network information.\r\n"
				"\r\ndevice-instance:\r\n"
				"BACnet Device Object Instance number that you are trying to\r\n"
				"send a Who-Is service request.  The value should be in\r\n"
				"the range of 0 to 4194303.  A range of values can also be\r\n"
				"specified by using a minimum value and a maximum value.\r\n"
				"\r\nExample:\r\n" "To send a WhoIs request to Device 123\r\n"
				"use the following command:\r\n" "%s 123\r\n"
				"To send a WhoIs request to Devices from 1000 to 9000:\r\n"
				"%s 1000 9000\r\n" "To send a WhoIs request to all devices\r\n"
				"use the following command:\r\n" "%s -1\r\n",
				filename_remove_path(name), filename_remove_path(name),
				filename_remove_path(name));
			break;
		case 2:
			/** If they run command with no parameters...*/
			printf("Usage:\n"
					"\"Broadcast\" to all devices:\n"
					"\t%s -1\n\n"
					"\"Single device\" or \"Range:\"\n"
					"\t%s <device-instance-min> <device-instance-max>\n\n"
					"Try -h or --help...\n\n", filename_remove_path(name), filename_remove_path(name));
			break;
		default:
			printf("BACnet \"who-is\" service. Try -h or --help\n");
			break;

	}
}

static void parseArgs(int argc, char *argv[])
{
	int i, n;
	unsigned int counter = 0;
	char *p_arg = NULL;
	char *p_data = NULL;


	if (!argv)
		return;

	/** If they ran command without parameters...*/
	if(argc == 1) printUsage(2, argv[0]);

	/** Loop through parameters...*/
	for(i=1; i<argc; i++)
	{
		/** Start with 1, cause parameter[0] is program name...*/
		p_arg = argv[i];

		/** Are there any switches?*/
		if(p_arg[0] == '-')
		{
			switch(p_arg[1])
			{
				case '-':
					if(strcmp(p_arg, "--help") == 0)
					printf("You need help(Long)...\n");
					printUsage(1, argv[0]);
					break;
				case 'h':
					printf("You need help(Short)...\n");
					printUsage(1, argv[0]);
					break;
				case 'd':
					p_data = &p_arg[1];
					/** We're at 'd' now go over one...*/
					++p_data;
					/** Set position of for loop to next element...*/
					i+=1;
					//printf("p_data: %d\n", *p_data);
					/** If they did it right and have space between switch and value...*/
					while(*p_data == 0 || *p_data == ' '){printf("Skipping white spaces...\n"); ++p_data;}
					//printf("p_data: %s\n", ((char *)p_data));
					printf("Debugging is active...\n");

					n = strtol(p_data, NULL, 0);

					if ((n >= 0L) && (n <= 9))
						setDebugLevel(n);
					else
						printf("Invalid debug level. Using default.\n");
					break;
				case '1':
					/** This is not a switch, but because it has a '-' char in it, it just looks like one to code.*/

					printf("Broadcast \"whois\"...\n");
					//printf("para: %s\n", p_arg);
					instance->Min = 0;
					instance->Max = instance->Min;

					if(getDebugLevel() > 2)
					{
						printf("device-instance-min = %d\n"
								"device-instance-max = %d\n", instance->Min, instance->Max);
					}

					if (instance->Min > BACNET_MAX_INSTANCE)
					{
						fprintf(stderr,
								"object-instance-min=%u - it must be less than %u\r\n",
								instance->Min, BACNET_MAX_INSTANCE + 1);
						return;
					}
					break;
				default:
					printf("default? Nothing could be deduced...\n");
					printUsage(2, argv[0]);
					break;
			}

		}
		/** If there arn't any switches then must be BACnet device id's...*/
		else
		{
			//printf("p_arg as String: %s\n", p_arg);
			if(counter == 0)
			{
				instance->Min = strtol(p_arg, NULL, 0);
				counter++;
			}
			else if(counter == 1)
			{
				instance->Max = strtol(p_arg, NULL, 0);
				counter ++;
			}
		}
	}
	if(counter <= 1)
		instance->Max = instance->Min;
}

int main(int argc, char *argv[]) {

	BacNetConnection *con = new BacNetConnection(0xBAC0, "eth0");
	con->connect();
	/** BacNetServer listens all the time for incoming BACnet messages...*/

	//dev = new BacNetDevice("FieldServer", 11, 0xBAC0, 25, dev);
	BacNetDevice *dev = 0;
	BacNetWhoIsTestServer *srvr = new BacNetWhoIsTestServer(con, 1, "BACSERVER", dev);

	/** Deduce...*/
	parseArgs(argc,argv);

	BacNetRequest *req = new BacNetWhoIsRequest(instance->Max, instance->Min);
	BacNetBuffer *buffer = new BacNetBuffer;

	req->encode_request(buffer);

	printf("I am looking for this/these devices: %d %d\n", instance->Min, (instance->Max <= 0) ? 0x3FFFFF : instance->Max);

	con->sendTo(dev, buffer->getPointerToBuffer(), buffer->getLength(), BROADCAST);

	epicsThreadSleep(5);

	dev = srvr->get_bacnet_device_list();
	printf("\nReceived \"%u\" I-AM-REQUEST....\n\n", dev->get_number_of_devices());

	while(dev) {
		dev->printDevMac();
		dev = dev->getNext();
	}

	printf("\nNow see if they have names in their device properties...\n");
	dev = srvr->get_bacnet_device_list();
	while(dev) {
		BacNetVariable *var = new BacNetVariableString(OBJECT_DEVICE, dev->getInstance(), PROP_OBJECT_NAME, -1, 16);
		dev->addBacNetVariable(var);
		dev = dev->getNext();
	}

	dev = srvr->get_bacnet_device_list();
	while(dev) {
	 dev->scanBacNetVariables(con);
	 dev = dev->getNext();
	}

	epicsThreadSleep(5);

	dev = srvr->get_bacnet_device_list();
	while(dev) {
		BacNetVariable *vStr = dev->getBacNetvariableList();
		dev->setName((char *)((BacNetVariableString *)vStr)->getValue());
		dev->print_device_info();
		dev = dev->getNext();
	}

	delete con;
	//delete srvr;
	delete req;
	delete buffer;
	return 0;
}
