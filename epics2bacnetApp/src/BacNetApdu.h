/*
 * BacNetApdu.h
 *
 *  Created on: Aug 26, 2011
 *      Author: 8w4
 */

#ifndef BACNETAPDU_H_
#define BACNETAPDU_H_

#include "bacenum.h"
#include "BacNetBuffer.h"

class BacNetApdu {
private:
	BACNET_PDU_TYPE pdu_type;

protected:

public:
	BacNetApdu();
	virtual ~BacNetApdu();
	BACNET_PDU_TYPE get_pdu_type();
	void set_pdu_type(BACNET_PDU_TYPE type);
	BacNetBuffer *decode_apdu(BacNetBuffer *buffer);

};

#endif /* BACNETAPDU_H_ */
