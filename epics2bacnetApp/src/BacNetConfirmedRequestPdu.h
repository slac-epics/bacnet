/*
 * BacNetConfirmedRequestPdu.h
 *
 *  Created on: Aug 26, 2011
 *      Author: 8w4
 */

#ifndef BACNETCONFIRMEDREQUESTPDU_H_
#define BACNETCONFIRMEDREQUESTPDU_H_
#include <stdio.h>
#include <stdint.h>
#include "bacenum.h"
#include "BacNetResponse.h"
#include "BacNetApdu.h"
#include "BacNetBuffer.h"
#include "BacNetBVLC.h"
#include "BacNetNpdu.h"
#include "BacNetDevice.h"

class BacNetConfirmedRequestPdu : public BacNetResponse{
private:
	const uint8_t pdu_type;
	bool segmented_message;
	bool more_follows;
	bool segmented_response_accepted;

	uint8_t max_segments_accepted;
	uint8_t max_apdu_length_accepted;
	uint8_t invoke_id;
	uint8_t sequence_number;
	uint8_t proposed_window_size;
	uint8_t service_choice;

protected:


public:
	BacNetConfirmedRequestPdu(uint8_t svc_choice, uint8_t invoke_id);
	BacNetConfirmedRequestPdu();
	virtual ~BacNetConfirmedRequestPdu();

	BacNetBuffer *encode_confirmed_service_req(BacNetBuffer *buffer);

	BacNetBuffer *decode_confirmed_request(class BacNetBuffer *buffer);

    uint8_t getPdu_type() const;

    bool getSegmented_message() const;

    void setSegmented_message(bool segmented_message);

    bool getMore_follows() const;

    void setMore_follows(bool more_follows);

    bool getSegmented_response_accepted() const;

    void setSegmented_response_accepted(bool segmented_response_accepted);

    uint8_t getMax_segments_accepted() const;

    void setMax_segments_accepted(uint8_t max_segments_accepted);

    uint8_t getMax_apdu_length_accepted() const;

    void setMax_apdu_length_accepted(uint8_t max_apdu_length_accepted);

    uint8_t getInvoke_id() const;

    void setInvoke_id(uint8_t invoke_id);

    uint8_t getSequence_number() const;;

    void setSequence_number(uint8_t sequence_number);

    uint8_t getProposed_window_size() const;

    void setProposed_window_size(uint8_t proposed_window_size);

    uint8_t getService_choice() const;

};

#endif /* BACNETCONFIRMEDREQUESTPDU_H_ */
