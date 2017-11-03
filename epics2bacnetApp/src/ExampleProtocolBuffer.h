/*
 * ProtocolBuffer.h
 *
 *  Created on: Aug 29, 2011
 *      Author: 8w4
 */

#ifndef PROTOCOLBUFFER_H_
#define PROTOCOLBUFFER_H_

/*
char buffer[800];

 char *encode_32(char * p, long value)


 char *p = buffer;
 p = encode_32(p, 0x123);
 p = encode_16(p, 0x123);
 p = encode_apdu(p, 1, 2, 3, 4);
*/

class ProtocolBuffer
{
private:
	char buffer[2048];
	char *p;

public:
	void reset()
	{
		p = buffer;
	}

	void encode_unsigned16(uint16_t value)
	{
		*(p++) = (uint8_t) ((value & 0xff00) >> 8);
		*(p++) = (uint8_t) (value & 0x00ff);
	}

	uint16_t dcode_unsigned16()
	{
		*(p++) = (uint8_t) ((value & 0xff00) >> 8);
		*(p++) = (uint8_t) (value & 0x00ff);

		return ...;
	}
};

/*
 * ProtocolBuffer buf;
 *
 * buf.reset();
 * buf.encode(0x1234);
 * buf.encode(0x3244);
 *
 *
 * buf.fillFromNetwork(...)
 * BacNetConfirmedRequestPdu.decodeFromBuffer(buf)
 */

#endif /* PROTOCOLBUFFER_H_ */
