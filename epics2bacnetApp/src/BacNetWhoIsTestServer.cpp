/*
 * BacNetServer.cpp
 *
 *  Created on: Sep 2, 2011
 *      Author: 8w4
 */

#include "BacNetWhoIsTestServer.h"
#include "BacNetConnection.h"
#include "BacNetDevice.h"
#include "BacNetBuffer.h"
#include "BacNetBVLC.h"
#include "BacNetNpdu.h"
#include "Response.h"
#include "BacNetResponse.h"
#include "BacNetUnconfirmedRequestPdu.h"
#include "BacNetComplexACK.h"
#include "BacNet_I_Am_Request.h"
#include "bacenum.h"
#include "common.h"

#include <stdio.h>
#include <string.h>

BacNetWhoIsTestServer::BacNetWhoIsTestServer(class BacNetConnection *con, int arg, const char *name, class BacNetDevice *dev_list)
:bacnet_device_list(dev_list), thread(*this,name,epicsThreadGetStackSize(epicsThreadStackMedium),50+arg)
{

	this->con = con;
	printf("\tStarting the Server...%s\n", name);
	thread.start();
}

BacNetWhoIsTestServer::~BacNetWhoIsTestServer() {
	// TODO Auto-generated destructor stub
	thread.exit();
}

BacNetDevice *findDevice(sockaddr_in addr, BacNetBVLC *bvlc, BacNetNpdu *npdu, BacNetDevice *dev){

	uint8_t tempMac[6];
	memcpy(&tempMac[0], &addr.sin_addr, 4);
	memcpy(&tempMac[4], &addr.sin_port, 2);

	printf("\nLooking for IP %s\n", inet_ntoa(addr.sin_addr));
	printf("\nLooking for BVLC %X:%X:%X:%X:%X:%X\n", bvlc->get_bvlc_forward_addr()[0], bvlc->get_bvlc_forward_addr()[1],
			bvlc->get_bvlc_forward_addr()[2], bvlc->get_bvlc_forward_addr()[3], bvlc->get_bvlc_forward_addr()[4],
			bvlc->get_bvlc_forward_addr()[5]);
	printf("\nLooking for BVLC %u\n", bvlc->get_bvlc_function());
	printf("\nLooking for NPDU %u, %u\n", npdu->get_npdu_source_specifier(), npdu->get_npdu_snet());


	if(bvlc->get_bvlc_function() == BVLC_FORWARDED_NPDU){
		while(dev){
			if(npdu->get_npdu_source_specifier()){
				if(dev->getSnet() == npdu->get_npdu_snet())
					if(memcmp(dev->getSadr(), npdu->get_npdu_sadr(), dev->getSlen()) == 0){
						//if(verbosity_level_3(verbosity_level)) {
							printf("\nMATCH(addr behind router): %s\n", dev->getName());
						//}
						return dev;
					}
			}
			else{
				if(memcmp(bvlc->get_bvlc_forward_addr(),dev->getDevMac(), 6) == 0){
					//if(verbosity_level_3(verbosity_level)) {
						printf("\nMATCH (IP): %s\n", dev->getName());
					//}
					return dev;
				}
			}
			dev = dev->getNext();
		}
	}
	else {
		while(dev){
			if(npdu->get_npdu_source_specifier()){
				if(dev->getSnet() == npdu->get_npdu_snet())
					if(memcmp(dev->getSadr(), npdu->get_npdu_sadr(), dev->getSlen()) == 0){
						//if(verbosity_level_3(verbosity_level)) {
							printf("\nMATCH(addr behind router): %s\n", dev->getName());
						//}
						return dev;
					}
			}
			else{
				if(memcmp(&tempMac,dev->getDevMac(), 6) == 0 && dev->getSnet() == 0x0000){
					//if(verbosity_level_3(verbosity_level)) {
						printf("\nMATCH (IP): %s\n", dev->getName());
					//}
					return dev;
				}
			}
			dev = dev->getNext();
		}
	}

	return NULL;
}

void BacNetWhoIsTestServer::run() {

	BacNetBuffer *buffer = new BacNetBuffer();

	struct sockaddr_in addr;
	size_t addrLen = sizeof(addr);
	int received = -1;

	BacNet_I_Am_Request *iam;

	if(con->getSocket() > 0)
	{
		while(true) {
			if(verbosity_level_3(verbosity_level))
				printf("\tI am listening for BACNET packets on port: %d using socket:%d.\n",con->getPort(), con->getSocket());
			uint8_t *pdu = buffer->getPointerToBuffer();
			received = con->recvFrom(&addr, addrLen, pdu, 0);

			/** Do we have anything???*/
			if(received > 0)
			{
				/** If so, is it our own message that we sent out? If it is
				 * our own sent message then ignore it...*/
				if((addr.sin_addr.s_addr == con->get_iocAddr()->s_addr ||
						addr.sin_addr.s_addr == con->get_iocBroadcastAddr()->s_addr))
				{
					/** Do nothing if it is our own message...*/;
				}
				else
				{
					/** We have a message! Is it a BacNet message?*/
					/** Look for BIP signature i.e. Make sure it is a BACnet message...*/
					if(*pdu == 0x81)
					{
						if(verbosity_level_3(verbosity_level)) {
							printf("\nMessage received from: %s\n", inet_ntoa(addr.sin_addr));
							printf("\nMessage start in \"RAW_HEX:\">>>\n");
							buffer->hexdump(pdu, received);
							printf("\n<<<Message end.\n");
						}

						BacNetBVLC *bvlc = new BacNetBVLC();
						bvlc->decode_bvlc(buffer);

						BacNetNpdu *npdu = new BacNetNpdu();
						npdu->decode_npdu(buffer);

						pdu = buffer->get_currentPosition();

						if(*pdu == PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST) {
							/** Move to next octet...*/
							++pdu;
							if(*pdu == SERVICE_UNCONFIRMED_I_AM) {

								iam = new BacNet_I_Am_Request;
								iam->decode_iam_request(buffer);

								char tempName[MAX_DEVICE_NAME_LENGTH];
								int l = sprintf(tempName, "Unknown_%u", bacnet_device_list->get_number_of_devices() + 1);
								if(l)
									bacnet_device_list = new BacNetDevice(tempName, iam->get_object_instance(), 0, 25, bacnet_device_list);
								else
									bacnet_device_list = new BacNetDevice("Unknown?", iam->get_object_instance(), 0, 25, bacnet_device_list);

								if(npdu->get_npdu_source_specifier()){
									bacnet_device_list->setSnet(npdu->get_npdu_snet());
									bacnet_device_list->setSlen(npdu->get_npdu_slen());
									bacnet_device_list->setSadr(npdu->get_npdu_sadr(), npdu->get_npdu_slen());
								}
								bacnet_device_list->set_isKnown(true);
								if(bvlc->get_bvlc_function() == BVLC_FORWARDED_NPDU){
									bacnet_device_list->setDevMac(bvlc->get_bvlc_forward_addr(), 6);
								}
								else{
									uint8_t tmpmac[6];
									memcpy(tmpmac, &addr.sin_addr, 4);
									memcpy(&tmpmac[4], &addr.sin_port, 2);
									if(verbosity_level_3(verbosity_level))
										buffer->hexdump(tmpmac, 6);
									bacnet_device_list->setDevMac(tmpmac, 6);
								}
							}
						}
						else if(*pdu == PDU_TYPE_COMPLEX_ACK) {
							BacNetResponse *r_pdu = new BacNetComplexACK;

							/** Find the device...*/
							BacNetDevice *dev;
							printf("\n>>>>\n\n");
							dev = findDevice(addr, bvlc, npdu, bacnet_device_list);
							printf("\ndev->getInstance() %u\n", dev->getInstance());
							printf("\n<<<<\n\n");

							if(!dev){
								printf("\nDevice not found in linked-list!!! (%p), addr: %s\n", dev, inet_ntoa(addr.sin_addr));
								break;
							}

							/** Decode packet*/
							((BacNetComplexACK *)r_pdu)->decode_complex_ack(buffer, &addr, bvlc, npdu, dev);

							delete r_pdu;
						}
						delete bvlc;
						delete npdu;
					}
					else
					{
						/** Do nothing... This is not a BACnet message...*/
					}
				}

			}

			epicsThreadSleep(0.0025);

			}
	}

	delete buffer;
}

class BacNetDevice *BacNetWhoIsTestServer::get_bacnet_device_list() {
	return this->bacnet_device_list;
}

