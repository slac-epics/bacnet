/*
 * BacNetWhoIsRequest.cpp
 *
 *  Created on: Aug 26, 2011
 *      Author: 8w4
 */

#include "BacNetWhoIsRequest.h"
#include "bacenum.h"
#include "BacNetBuffer.h"
#include "BacNetNpdu.h"
#include "BacNetBVLC.h"

#define CONTEXTSPECIFIC 1

uint32_t find_lvt_len(uint32_t val){
	uint32_t len;

	if (val < 0x100) {
		len = 1;
	} else if (val < 0x10000) {
		len = 2;
	} else if (val < 0x1000000) {
		len = 3;
	} else {
		len = 4;
	}

	return len;
}

BacNetWhoIsRequest::BacNetWhoIsRequest(uint32_t highRange, uint32_t lowRange){
	this->deviceInstanceRangeHighLimit = highRange;
	this->deviceInstanceRangeLowLimit = lowRange;
}

BacNetWhoIsRequest::~BacNetWhoIsRequest() {
	// TODO Auto-generated destructor stub
}

BacNetBuffer *BacNetWhoIsRequest::encode_whoIs_request(BacNetBuffer *buffer){


	uint8_t *ptr2Buff = buffer->getPointerToBuffer();
	*(ptr2Buff++) = PDU_TYPE_UNCONFIRMED_SERVICE_REQUEST;
	*(ptr2Buff++) = SERVICE_UNCONFIRMED_WHO_IS;

	if(deviceInstanceRangeHighLimit < deviceInstanceRangeLowLimit ||
			(deviceInstanceRangeHighLimit == 0 && deviceInstanceRangeLowLimit == 0)) {

		/** Leave off low and high and build npdu...*/

	}
	else {

		/** Now see if LengthValueType needs to extend or not...*/
		uint32_t len = find_lvt_len(this->deviceInstanceRangeLowLimit);
		/** Encode context-specific tag with low limit.*/
		ptr2Buff = buffer->encode_bacnet_tag(ptr2Buff, 0, CONTEXTSPECIFIC, this->deviceInstanceRangeLowLimit, len);
		//buffer->hexdump(buffer->getPointerToBuffer(), ptr2Buff-buffer->getPointerToBuffer());

		/** Encode context-specific tag with high limit.*/
		len = find_lvt_len(this->deviceInstanceRangeHighLimit);
		ptr2Buff = buffer->encode_bacnet_tag(ptr2Buff, 1, CONTEXTSPECIFIC, this->deviceInstanceRangeHighLimit, len);
		//buffer->hexdump(buffer->getPointerToBuffer(), ptr2Buff-buffer->getPointerToBuffer());
	}

	buffer->set_length(ptr2Buff);

	/** Build NPDU...*/
	BacNetNpdu *n = new BacNetNpdu();
	n->set_npdu_destination_specifier(true);
	n->set_npdu_dlen(0);

	n->encode_npdu(buffer);

	BacNetBVLC *bvlc = new BacNetBVLC();
	bvlc->encode_bvlc_originalBroadcastNpdu(buffer);

	delete n;
	delete bvlc;
	return buffer;
}

BacNetBuffer *BacNetWhoIsRequest::encode_request(BacNetBuffer *buffer){

	buffer = encode_whoIs_request(buffer);
	return buffer;
}
