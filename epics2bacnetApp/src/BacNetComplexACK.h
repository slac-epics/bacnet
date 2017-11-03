/*
 * BacNetComplexACK.h
 *
 *  Created on: Sep 26, 2011
 *      Author: 8w4
 */

#ifndef BACNETCOMPLEXACK_H_
#define BACNETCOMPLEXACK_H_

#include <stdint.h>
#include "BacNetResponse.h"
#include "BacNetConfirmedService.h"
#include "bacenum.h"
#include "BacNetBuffer.h"
#include "BacNetBVLC.h"
#include "BacNetNpdu.h"
#include "BacNetDevice.h"
#include "BacNetVariable.h"
#include "BacNetReadPropertyACK.h"

class BacNetComplexACK : public BacNetResponse{
private:
	uint8_t pdu_type;
	bool segmented_msg;
	bool more_follows;
	uint8_t invoke_id;
	uint8_t sequence_number;
	uint8_t prop_window_sized;
	BACNET_CONFIRMED_SERVICE svc;
public:
	BacNetComplexACK();
	virtual ~BacNetComplexACK();
	BacNetBuffer *decode_complex_ack(class BacNetBuffer *buffer);
	BacNetBuffer *decode_complex_ack(class BacNetBuffer *buffer, sockaddr_in *addr, class BacNetBVLC *bvlc,
			class BacNetNpdu *npdu, class BacNetDevice *dev);
	uint8_t get_pdu_type();
	bool is_segmented();
	bool get_more_follows();
	uint8_t get_invoke_id();
	uint8_t get_sequence_number();
	uint8_t get_prop_window_size();
	BACNET_CONFIRMED_SERVICE get_service_type();
};

#endif /* BACNETCOMPLEXACK_H_ */
