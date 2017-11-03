/*
 * BacNetConnection.cpp
 *
 *  Created on: Sep 1, 2011
 *      Author: 8w4
 */

#include "BacNetConnection.h"
#include "DrvBacNet.h"
#include "common.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

int get_local_address(const char *ifname, struct in_addr *addr, int req)
{
	struct ifreq ifr = { {{0}} };
	struct sockaddr_in *tcpip_address;
	int rv;     /* return value */

	strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name));
	int fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if (fd < 0) {
		rv = fd;
	} else {
		rv = ioctl(fd, req, &ifr);
		close(fd);
	}

	if(rv >= 0)
	{
		tcpip_address = (struct sockaddr_in *) &ifr.ifr_addr;
		memcpy(addr, &tcpip_address->sin_addr, sizeof(struct in_addr));
	}

	return rv;
}

void BacNetConnection::initIocAddr(const char *ifname) {
	int rv = -1;

	/** SIOCGIFADDR, SIOCGIFBRDADDR*/
	rv = get_local_address(ifname, &this->iocAddr, SIOCGIFADDR);
	if(rv < 0)
	{
		fprintf(stderr, "Could not get local address using: %s see bipSocket.c\n", ifname);
	}
	else
		printf("My local addr is: %s\n", inet_ntoa(this->iocAddr));

	rv = get_local_address(ifname, &this->iocBroadcastAddr, SIOCGIFBRDADDR);
	if(rv < 0)
	{
		fprintf(stderr, "Could not get local broadcast address using: %s see bipSocket.c\n", ifname);
	}
	else
		printf("My Broadcast addr is: %s\n", inet_ntoa(this->iocBroadcastAddr));
}

BacNetConnection::BacNetConnection(uint16_t port) {

	initIocAddr("eth0");
	this->conSocket = 0;
	this->conPort = port;
}

BacNetConnection::BacNetConnection(uint16_t port, const char *ifname) {
	initIocAddr(ifname);
	this->conSocket = 0;
	this->conPort = port;
}

struct in_addr *BacNetConnection::get_iocAddr(void) {
	return &this->iocAddr;
}
struct in_addr *BacNetConnection::get_iocBroadcastAddr(void) {
	return &this->iocBroadcastAddr;
}

bool BacNetConnection::connect() {
	/* Create the UDP socket */
	conSocket = epicsSocketCreate(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (conSocket < 0) {
		if(verbosity_level_1(verbosity_level))
			printf("Failed to create socket\n");
		return false;
	}

	/** Not sure why, but you have to set this "reuseaddr" option...*/
	int sockopt = 1;
	int rv;
	rv = setsockopt(conSocket, SOL_SOCKET, SO_REUSEADDR, (void *) &sockopt, sizeof(sockopt));
	if (rv < 0) {
		if(verbosity_level_1(verbosity_level))
			printf("Failed to SET socket option\n");
		return false;
	}

	struct sockaddr_in srcAddr;
	unsigned int srcAddrLen;

	memset(&srcAddr, 0, sizeof(srcAddr));       /* Clear struct */
	srcAddr.sin_family = AF_INET;                  /* Internet/IP */
	srcAddr.sin_addr.s_addr = htonl(INADDR_ANY);  /* IP address */
	srcAddr.sin_port = htons(conPort);       /* server port */


	/* Bind the socket */
	srcAddrLen = sizeof(srcAddr);

	if (bind(conSocket, (struct sockaddr *) &srcAddr, srcAddrLen) < 0)
	{
		if(verbosity_level_1(verbosity_level))
			printf("Failed to bind! Socket: %d, IP: %s, Port: %d\n", conSocket, inet_ntoa(srcAddr.sin_addr), srcAddr.sin_port);
		return false;
	}
	else
	{
		if(verbosity_level_1(verbosity_level))
			printf("Bound to Socket: %d, IP: %s, Port: %d\n", conSocket, inet_ntoa(srcAddr.sin_addr), srcAddr.sin_port);
	}

	return true;

}

void BacNetConnection::disconnect()
{
	if (conSocket != 0)
	{
		if(verbosity_level_1(verbosity_level))
			printf("Deleting Socket: %d, Port: %d\n", conSocket, conPort);
		epicsSocketDestroy(conSocket);
		conSocket = 0;
	}
}

uint16_t BacNetConnection::getPort() {
	return conPort;
}

void BacNetConnection::setPort(uint16_t newPort) {
	this->conPort = newPort;
}

BacNetConnection::~BacNetConnection() {
	disconnect();
}

int BacNetConnection::getSocket() {
	return conSocket;
}


int BacNetConnection::sendTo(class BacNetDevice *dev, uint8_t *pdu, int pdu_len, enum msg_option m) {
	int bytes_sent = 0;

	struct sockaddr_in dest;
	int rv;
	int sockopt = 0;

	dest.sin_family = AF_INET;
	dest.sin_port = htons(this->conPort);

	if(m == BROADCAST){
		dest.sin_addr = this->iocBroadcastAddr;
		sockopt = 1;
		rv = setsockopt(conSocket, SOL_SOCKET, SO_BROADCAST, (void *) &sockopt, sizeof(sockopt));
	}
	else{
		dev->getDeviceIP(&dest);
		sockopt = 0;
		rv = setsockopt(conSocket, SOL_SOCKET, SO_BROADCAST, (void *) &sockopt, sizeof(sockopt));
	}

	if(verbosity_level_3(verbosity_level))
		printf("Socket: %d, pdu_len: %d, addr: %s\n", conSocket, pdu_len, inet_ntoa(dest.sin_addr));

	bytes_sent = sendto(conSocket, pdu, pdu_len, 0, (struct sockaddr *) &dest, sizeof(dest));
	if(verbosity_level_3(verbosity_level))
		printf("Bytes sent: %d\n", bytes_sent);

	return bytes_sent;
}

int BacNetConnection::recvFrom(struct sockaddr_in *addr, socklen_t addrLen, uint8_t *pdu, int pdu_len) {

	/* Wait for responses */

	int received = -1;

	const unsigned time_out = 100;
	int max_mpdu = 5000;

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(this->conSocket, &fds);
	struct timeval select_timeout;
	/* we could just use a non-blocking socket, but that consumes all
	   the CPU time.  We can use a timeout; it is only supported as
	   a select. */
	if (time_out >= 1000) {
		select_timeout.tv_sec = time_out / 1000;
		select_timeout.tv_usec = 1000 * (time_out - select_timeout.tv_sec * 1000);
	} else {
		select_timeout.tv_sec = 0;
		select_timeout.tv_usec = 1000 * time_out;
	}
	if (select(this->conSocket+1, &fds, NULL, NULL, &select_timeout) > 0)
	{
		received = recvfrom(this->conSocket, &pdu[0], max_mpdu, 0, (struct sockaddr *) addr, &addrLen);
	}
	else
		return -1;

	/** Did we get anything???*/
	if(received <= 0)
		return -1;


	return received;
}

