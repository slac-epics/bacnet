/*
 * BacNetConnection.h
 *
 *  Created on: Sep 1, 2011
 *      Author: 8w4
 */

#ifndef BACNETCONNECTION_H_
#define BACNETCONNECTION_H_

//System
#include <stdint.h>

// EPICS Base
#include <osiSock.h>

// Local
#include "BacNetDevice.h"

enum msg_option{
	UNICAST = 0,
	BROADCAST = 1
};

class BacNetConnection {
private:
	SOCKET conSocket;
	uint16_t conPort;
	struct in_addr iocAddr;
	struct in_addr iocBroadcastAddr;
	void initIocAddr(const char *ifname);

public:
	BacNetConnection(uint16_t port);
	BacNetConnection(uint16_t port, const char *ifname);
	virtual ~BacNetConnection();
	bool connect();
	void disconnect();

	int sendTo(class BacNetDevice *dev, uint8_t *pdu, int pdu_len, enum msg_option m);
	int recvFrom(struct sockaddr_in *rcv_addr, socklen_t lengthOfAddr, uint8_t *pdu, int pdu_len);
	int getSocket();

	struct in_addr *get_iocAddr(void);
	struct in_addr *get_iocBroadcastAddr(void);

	uint16_t getPort();
	void setPort(uint16_t newPort);
};

#endif /* BACNETCONNECTION_H_ */
