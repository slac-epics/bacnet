/*
 * BacNetBuffer.h
 *
 *  Created on: Aug 29, 2011
 *      Author: 8w4
 */

#ifndef BACNETBUFFER_H_
#define BACNETBUFFER_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "bacstructures.h"

const unsigned int APPLICATION_SPECIFIC = 0;
const unsigned int CONTEXT_SPECIFIC = 1;

const unsigned int OPENING_TAG = 6;
const unsigned int CLOSING_TAG = 7;

class BacNetBuffer {
private:

	uint8_t buffer[2500];
	uint8_t *ptrBuff;
	uint8_t *currentPosition;
	size_t length;
public:
	BacNetBuffer();
	virtual ~BacNetBuffer();
	uint8_t *getPointerToBuffer();
	size_t getLength(uint8_t *pos);
	size_t getLength();
	void set_length(uint8_t *pos);
	void set_currentPosition(uint8_t *ptr);
	uint8_t *get_currentPosition();

	void hexdump(const uint8_t *buffer, size_t length);

	/** unsigned value encoding and decoding */
	uint8_t *encode_unsigned16(uint8_t *pdu, uint16_t value);

	uint8_t *decode_unsigned16(uint8_t *apdu, uint16_t *value);

	uint8_t *encode_unsigned24(uint8_t *apdu, uint32_t value);

	uint8_t *decode_unsigned24(uint8_t *apdu, uint32_t *value);

	uint8_t *encode_unsigned32(uint8_t *apdu, uint32_t value);

	uint8_t *decode_unsigned32(uint8_t *apdu, uint32_t *value);

	uint8_t *decode_var_length(uint8_t *destination, uint8_t *source, uint8_t length);

	/** signed value encoding and decoding */
	int encode_signed8(uint8_t * apdu, int8_t value);

	int decode_signed8(uint8_t * apdu, int32_t * value);

	int encode_signed16(uint8_t * apdu, int16_t value);

	int decode_signed16(uint8_t * apdu, int32_t * value);

	int encode_signed24(uint8_t * apdu, int32_t value);

	int decode_signed24(uint8_t * apdu, int32_t * value);

	int encode_signed32(uint8_t * apdu, int32_t value);

	int decode_signed32(uint8_t * apdu, int32_t * value);

	uint8_t *decode_bacnet_tag(uint8_t *pdu, uint8_t *tagNumber, uint8_t *tagClass, uint32_t *tagLenValType);

	uint8_t *decode_bacnet_object_id(uint8_t *pdu, uint16_t *type, uint32_t *instanceNumber);

	uint8_t *decode_bacnet_unsigned_int(uint8_t *pdu, uint32_t *uValue, uint32_t len);

	uint8_t *decode_bacnet_bit_string(uint8_t *pdu, uint32_t *uValue, uint32_t len);

	uint8_t *decode_bacnet_boolean(uint8_t *pdu, bool *uValue, uint32_t len);

	uint8_t *decode_bacnet_enumeration(uint8_t *pdu, uint32_t *enumValue, uint32_t len);

	uint8_t *decode_bacnet_character_string(uint8_t *apdu, char *stringValue, uint32_t len);

	uint8_t *decode_bacnet_abstract_syntax_type(uint8_t *pdu, BACNET_APPLICATION_DATA_VALUE *value, BACNET_APPLICATION_TAG *tag);

	uint8_t *decode_bacnet_real(uint8_t *apdu, float *p);

	uint8_t *encode_bacnet_real(uint8_t *apdu, BACNET_APPLICATION_DATA_VALUE *val);

	uint8_t *encode_bacnet_enumeration(uint8_t *apdu, uint32_t val);

	uint8_t *encode_bacnet_tag(uint8_t *pdu, uint8_t tagNumber, uint8_t tagClass, uint32_t tagLenValType, unsigned int lvt_len);

	uint8_t *encode_bacnet_context_tag(uint8_t *pdu, uint8_t tagNumber, uint32_t tagLenValType, unsigned int lvt_len);

	uint8_t *encode_bacnet_opening_tag(uint8_t *pdu, uint8_t tagNumber, uint32_t tagLenValType, unsigned int lvt_len);

	uint8_t *encode_bacnet_closing_tag(uint8_t *pdu, uint8_t tagNumber, uint32_t tagLenValType, unsigned int lvt_len);

	uint8_t *encode_bacnet_application_tag(uint8_t *pdu, uint8_t tagNumber, uint32_t tagLenValType);

	uint32_t encode_bacnet_objectIdentifier(uint16_t objType, uint32_t instanceNumber, uint32_t *bacObjId);

	//uint8_t *encode_bacnet_constructed_tag(uint8_t *pdu, uint8_t tagNumber, BACNET_APPLICATION_TAG tag, BACNET_APPLICATION_DATA_VALUE *val, uint32_t length);

	uint8_t *encode_bacnet_application_tag_real(uint8_t *pdu, uint8_t tagNumber, BACNET_APPLICATION_DATA_VALUE *val);

	uint8_t *encode_bacnet_application_tag_unsignedInt(uint8_t *pdu, uint8_t tagNumber, uint32_t val);

	uint8_t *encode_bacnet_application_tag_enumeration(uint8_t *pdu, uint8_t tagNumber, BACNET_APPLICATION_DATA_VALUE *val);

	int get_bacnet_unsigned_length(uint32_t some_value);
};

#endif /* BACNETBUFFER_H_ */
