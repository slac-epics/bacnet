/*
 * BacNetSubscribeCOVtestServer.cpp
 *
 *  Created on: Dec 15, 2011
 *      Author: 8w4
 */

#include "BacNetSubscribeCOVtestServer.h"
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

BacNetSubscribeCOVtestServer::BacNetSubscribeCOVtestServer(class BacNetConnection *con, int arg, const char *name, class BacNetDevice *dev_list)
:bacnet_device_list(dev_list), thread(*this,name,epicsThreadGetStackSize(epicsThreadStackMedium),50+arg)
{

	this->con = con;
	printf("\tStarting the Server...%s\n", name);
	thread.start();
}


BacNetSubscribeCOVtestServer::~BacNetSubscribeCOVtestServer() {
	// TODO Auto-generated destructor stub
}

BacNetDevice *findDevice(sockaddr_in addr, BacNetBVLC *bvlc, BacNetNpdu *npdu, BacNetDevice *dev){

	uint8_t tempMac[6];
	memcpy(&tempMac[0], &addr.sin_addr, 4);
	memcpy(&tempMac[4], &addr.sin_port, 2);

	if(bvlc->get_bvlc_function() == BVLC_FORWARDED_NPDU){
		while(dev){
			if(npdu->get_npdu_source_specifier()){
				if(dev->getSnet() == npdu->get_npdu_snet())
					if(memcmp(dev->getSadr(), npdu->get_npdu_sadr(), dev->getSlen()) == 0){
						if(verbosity_level_3(verbosity_level)) {
							printf("\nMATCH(addr behind router): %s\n", dev->getName());
						}
						return dev;
					}
			}
			else{
				if(memcmp(bvlc->get_bvlc_forward_addr(),dev->getDevMac(), 6) == 0){
					if(verbosity_level_3(verbosity_level)) {
						printf("\nMATCH (IP): %s\n", dev->getName());
					}
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
						if(verbosity_level_3(verbosity_level)) {
							printf("\nMATCH(addr behind router): %s\n", dev->getName());
						}
						return dev;
					}
			}
			else{
				if(memcmp(&tempMac,dev->getDevMac(), 6) == 0){
					if(verbosity_level_3(verbosity_level)) {
						printf("\nMATCH (IP): %s\n", dev->getName());
					}
					return dev;
				}
			}
			dev = dev->getNext();
		}
	}

	return NULL;
}

void BacNetSubscribeCOVtestServer::run() {

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

						printf("\nMessage received from: %s\n", inet_ntoa(addr.sin_addr));
						printf("\nMessage start in \"RAW_HEX:\">>>\n");
						buffer->hexdump(pdu, received);
						printf("\n<<<Message end.\n");

						BacNetBVLC *bvlc = new BacNetBVLC();
						bvlc->decode_bvlc(buffer);

						BacNetNpdu *npdu = new BacNetNpdu();
						npdu->decode_npdu(buffer);

						pdu = buffer->get_currentPosition();


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

class BacNetDevice *BacNetSubscribeCOVtestServer::get_bacnet_device_list() {
	return this->bacnet_device_list;
}

