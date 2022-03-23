/*
 * BacNetUnconfirmedRequestPdu.cpp
 *
 *  Created on: Sep 2, 2011
 *      Author: 8w4
 */

#include "BacNetUnconfirmedRequestPdu.h"
#include "common.h"

BacNetUnconfirmedRequestPdu::BacNetUnconfirmedRequestPdu() {

}

BacNetUnconfirmedRequestPdu::~BacNetUnconfirmedRequestPdu() {
	// TODO Auto-generated destructor stub
}

BacNetBuffer *BacNetUnconfirmedRequestPdu::decode_unconfirmed_request(class BacNetBuffer *buffer, sockaddr_in *addr, class BacNetBVLC *bvlc,
		class BacNetNpdu *npdu, class BacNetDevice *dev) {

	BacNet_I_Am_Request iam;
	uint8_t *pdu = buffer->get_currentPosition();

	/** Should be pointing to Unconfirmed PDU type; move to Service Choice octet...*/
	pdu++;

	switch(*pdu) {
	case SERVICE_UNCONFIRMED_I_AM:
		if(verbosity_level_3(verbosity_level))
			printf("\nSERVICE_UNCONFIRMED_I_AM: (%X)\n", *pdu);
		iam.decode_iam_request(buffer);
		while(dev){
			if(dev->getInstance() == iam.get_object_instance()){
				if(npdu->get_npdu_source_specifier()){
					dev->setSnet(npdu->get_npdu_snet());
					dev->setSlen(npdu->get_npdu_slen());
					dev->setSadr(npdu->get_npdu_sadr(), npdu->get_npdu_slen());
				}
				if(verbosity_level_3(verbosity_level))
					printf("\nYay... Found a match...\n");
				dev->set_isKnown(true);
				if(bvlc->get_bvlc_function() == BVLC_FORWARDED_NPDU){
					dev->setDevMac(bvlc->get_bvlc_forward_addr(), 6);
				    if(verbosity_level_3(verbosity_level))
					    dev->printDevMac();
				}
				else{
					uint8_t tmpmac[6];
					memcpy(tmpmac, &addr->sin_addr, 4);
					memcpy(&tmpmac[4], &addr->sin_port, 2);
					if(verbosity_level_3(verbosity_level))
						buffer->hexdump(tmpmac, 6);
					dev->setDevMac(tmpmac, 6);
				}
			}
			dev = dev->getNext();
		}

		break;
	case SERVICE_UNCONFIRMED_I_HAVE:
		if(verbosity_level_3(verbosity_level))
			printf("\nSERVICE_UNCONFIRMED_I_AM: (%X)\n", *pdu);
		break;
	case SERVICE_UNCONFIRMED_COV_NOTIFICATION:
		if(verbosity_level_3(verbosity_level))
			printf("\nSERVICE_UNCONFIRMED_I_AM: (%X)\n", *pdu);
		break;
	case SERVICE_UNCONFIRMED_EVENT_NOTIFICATION:
		if(verbosity_level_3(verbosity_level))
			printf("\nSERVICE_UNCONFIRMED_I_AM: (%X)\n", *pdu);
		break;
	case SERVICE_UNCONFIRMED_PRIVATE_TRANSFER:
		if(verbosity_level_3(verbosity_level))
			printf("\nSERVICE_UNCONFIRMED_I_AM: (%X)\n", *pdu);
		break;
	case SERVICE_UNCONFIRMED_TEXT_MESSAGE:
		if(verbosity_level_3(verbosity_level))
			printf("\nSERVICE_UNCONFIRMED_I_AM: (%X)\n", *pdu);
		break;
	case SERVICE_UNCONFIRMED_TIME_SYNCHRONIZATION:
		if(verbosity_level_3(verbosity_level))
			printf("\nSERVICE_UNCONFIRMED_I_AM: (%X)\n", *pdu);
		break;
	case SERVICE_UNCONFIRMED_WHO_HAS:
		if(verbosity_level_3(verbosity_level))
			printf("\nSERVICE_UNCONFIRMED_I_AM: (%X)\n", *pdu);
		break;
	case SERVICE_UNCONFIRMED_WHO_IS:
		if(verbosity_level_3(verbosity_level))
			printf("\nSERVICE_UNCONFIRMED_I_AM: (%X)\n", *pdu);
		break;
	case SERVICE_UNCONFIRMED_UTC_TIME_SYNCHRONIZATION:
		if(verbosity_level_3(verbosity_level))
			printf("\nSERVICE_UNCONFIRMED_I_AM: (%X)\n", *pdu);
		break;
	default:
		if(verbosity_level_3(verbosity_level))
			printf("\nUnrecognised service: (%X)\n", *pdu);
		break;
	}
	return buffer;
}

