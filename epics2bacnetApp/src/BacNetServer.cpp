/*
 * BacNetServer.cpp
 *
 *  Created on: Sep 2, 2011
 *      Author: 8w4
 */

#include "BacNetServer.h"
#include "BacNetConnection.h"
#include "DrvBacNet.h"
#include "BacNetDevice.h"
#include "BacNetBuffer.h"
#include "BacNetBVLC.h"
#include "BacNetNpdu.h"
#include "Response.h"
#include "BacNetResponse.h"
#include "BacNetConfirmedRequestPdu.h"
#include "BacNetUnconfirmedRequestPdu.h"
#include "BacNetConfirmedCovNotification.h"
#include "BacNet_I_Am_Request.h"
#include "BacNetComplexACK.h"
#include "BacNetSimpleACK.h"
#include "BacNetReadPropertyACK.h"
#include "BacNetError.h"
#include "bacenum.h"
#include "common.h"

#include <stdio.h>
#include <string.h>

BacNetServer::BacNetServer(class BacNetConnection *con, int arg, const char *name, class BacNetDevice *dev_list)
:bacnet_device_list(dev_list), thread(*this,name,epicsThreadGetStackSize(epicsThreadStackMedium),50+arg)
{

	this->con = con;
	printf("\tStarting the Server...%s\n", name);
	thread.start();
}

BacNetServer::~BacNetServer() {
	// TODO Auto-generated destructor stu
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

void BacNetServer::run() {

	timeval serverStart;
	timeval serverNow;

	BacNetBuffer *buffer = new BacNetBuffer();

	struct sockaddr_in addr;
	socklen_t addrLen = sizeof(addr);
	int received = -1;

	BacNetResponse *r_pdu;


	/** Let this always point to the beginning of the list...*/
	BacNetDevice * const dev_list = this->bacnet_device_list;
	BacNetDevice *dev;

	if(con->getSocket() > 0)
	{
		while(true) {
			gettimeofday(&serverStart, NULL);
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

						switch(*pdu){
						case PDU_TYPE_CONFIRMED_SERVICE_REQUEST:
							if(verbosity_level_3(verbosity_level))
								printf("\nPDU_TYPE_CONFIRMED_SERVICE_REQUEST (%X)\n", *pdu);
							r_pdu = new BacNetConfirmedRequestPdu;

							//TODO remove cast and make polymorphic...
							((BacNetConfirmedRequestPdu *)r_pdu)->decode_confirmed_request(buffer);

							/** Right now, I am only supporting ConfirmedCOVNotification...*/
							if(((BacNetConfirmedRequestPdu *)r_pdu)->getService_choice() == SERVICE_CONFIRMED_COV_NOTIFICATION) {

								/** Decode COV notification...*/
								BacNetConfirmedCovNotification *ccn = new BacNetConfirmedCovNotification();
								buffer = ccn->decode_confirmed_cov_notification(buffer);
								if(!buffer) {
									/** Something happened; could not decode properly...*/
									if(r_pdu)
										delete r_pdu;
									if(ccn)
										delete ccn;
									break;
								}

								/** Try to find the correct device and variable...*/
								dev = findDevice(addr, bvlc, npdu, dev_list);
								if(!dev) {
									if(verbosity_level_1(verbosity_level))
										printf("\nPDU_TYPE_CONFIRMED_SERVICE_REQUEST. Could not find device in linked-list\n");
									if(r_pdu)
										delete r_pdu;
									if(ccn)
										delete ccn;
									break;
								}

								BacNetVariable *var;
								var = dev->get_bacnet_variable_by_process_id(ccn->getSubscriber_process_identifier());
								if(!var){
									if(verbosity_level_1(verbosity_level))
										printf("\nPDU_TYPE_CONFIRMED_SERVICE_REQUEST. Could not find variable in linked-list\n");
									if(r_pdu)
										delete r_pdu;
									if(ccn)
										delete ccn;
									break;
								}

								BacNetPropertyValue *pv = ccn->getList_of_values();
								while(pv) {
									if(pv->getProperty_identifier() == PROP_PRESENT_VALUE) {
										var->setIsValid(true);
										var->setValue(*pv->getValue()->get_abstract_value());

										/** Now build a simple ack and send it back...*/
										BacNetSimpleACK *smplACK = new BacNetSimpleACK(((BacNetConfirmedRequestPdu *)r_pdu)->getInvoke_id(),
												SERVICE_CONFIRMED_COV_NOTIFICATION);
										smplACK->encode_simple_ack(buffer, dev);

										con->sendTo(dev, buffer->getPointerToBuffer(), buffer->getLength(), UNICAST);
										if(smplACK)
											delete smplACK;
										break;
									}
									pv = pv->get_next();
								}
								if(!pv) {
								var->setIsValid(false);
								}
								if(ccn)
									delete ccn;
							}
							else {
								/** Do nothing, it is an unsupported service...*/
							}

							if(r_pdu)
								delete r_pdu;
							break;
						case PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST:
							if(verbosity_level_3(verbosity_level))
								printf("\nPDU_TYPE_UNCONFIRMED_SERVICE_REQUEST (%X)\n", *pdu);
							r_pdu = new BacNetUnconfirmedRequestPdu();

							((BacNetUnconfirmedRequestPdu *)r_pdu)->decode_unconfirmed_request(buffer, &addr, bvlc, npdu, dev_list);
							delete r_pdu;
							break;
						case PDU_TYPE_SIMPLE_ACK:
							if(verbosity_level_3(verbosity_level))
								printf("\nPDU_TYPE_SIMPLE_ACK: %X\n", *pdu);
							r_pdu = new BacNetSimpleACK;

							/** Find the device...*/
							dev = findDevice(addr, bvlc, npdu, dev_list);

							if(!dev){
								if(verbosity_level_1(verbosity_level))
									printf("\nDevice not found in linked-list!!! (%p), addr: %s\n", dev, inet_ntoa(addr.sin_addr));
								break;
							}

							/** Decode packet*/
							((BacNetSimpleACK *)r_pdu)->decode_simple_ack(buffer, &addr, bvlc, npdu, dev);
							delete r_pdu;
							break;
						case PDU_TYPE_COMPLEX_ACK:
							if(verbosity_level_3(verbosity_level))
								printf("\nPDU_TYPE_COMPLEX_ACK PDU type: %X\n", *pdu);
							r_pdu = new BacNetComplexACK;

							/** Find the device...*/
							dev = findDevice(addr, bvlc, npdu, dev_list);

							if(!dev){
								if(verbosity_level_1(verbosity_level))
									printf("\nDevice not found in linked-list!!! (%p), addr: %s\n", dev, inet_ntoa(addr.sin_addr));
								break;
							}

							/** Decode packet*/
							((BacNetComplexACK *)r_pdu)->decode_complex_ack(buffer, &addr, bvlc, npdu, dev);

							delete r_pdu;
							break;
						case PDU_TYPE_SEGMENT_ACK:
							if(verbosity_level_3(verbosity_level))
								printf("\nPDU_TYPE_SEGMENT_ACK PDU type: %X\n", *pdu);
							break;
						case PDU_TYPE_ERROR:
							if(verbosity_level_3(verbosity_level))
								printf("\nPDU_TYPE_ERROR PDU type: %X\n", *pdu);
							r_pdu = new BacNetError;

							/** Find the device...*/
							dev = findDevice(addr, bvlc, npdu, dev_list);

							if(!dev){
								if(verbosity_level_1(verbosity_level))
									printf("\nDevice not found in linked-list!!! (%p), addr: %s\n", dev, inet_ntoa(addr.sin_addr));
								break;
							}

							/** Decode packet*/
							((BacNetError *)r_pdu)->decode_bacnet_error(buffer, &addr, bvlc, npdu, dev);

							delete r_pdu;
							break;
						case PDU_TYPE_REJECT:
							if(verbosity_level_3(verbosity_level))
								printf("\nPDU_TYPE_REJECT PDU type: %X\n", *pdu);
							break;
						case PDU_TYPE_ABORT:
							if(verbosity_level_3(verbosity_level))
								printf("\nPDU_TYPE_ABORT PDU type: %X\n", *pdu);
							break;
						default:
							if(verbosity_level_3(verbosity_level))
								printf("\nUnsuported PDU type: %X\n", *pdu);
							break;
						}
						delete bvlc;
						delete npdu;
					}
					else
					{
						if(verbosity_level_3(verbosity_level))
							printf("\"This\" message is NOT a BACnet message...\n");
					}
				}

			}
			else
			{
				if(verbosity_level_3(verbosity_level))
					printf("\tNothing yet: %d\n", received);
			}
			epicsThreadSleep(0.0025);
			gettimeofday(&serverNow, NULL);

			/** check time difference...*/
			//double t1 = serverStart.tv_sec+(serverStart.tv_usec/1000000.0);
			//double t2 = serverNow.tv_sec+(serverNow.tv_usec/1000000.0);
			//double tdiff = t2-t1;
			//printf("\nServer Time: %.6lf\n", tdiff);
			}
	}
	else
	{

	}
	delete buffer;
}

