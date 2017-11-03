/*
 * BacNetIp.cpp
 *
 *  Created on: Aug 25, 2011
 *      Author: 8w4
 */

#include "BacNetIp.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>

static int get_local_address(const char *ifname, struct in_addr *addr, int req)
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

BacNetIp::BacNetIp() {

	ifName = "eth0";
		int rv = -1;

		/** SIOCGIFADDR, SIOCGIFBRDADDR*/
		rv = get_local_address(ifName, &my_addr, SIOCGIFADDR);
		if(rv < 0)
		{
			fprintf(stderr, "Could not get local address using: %s see bipSocket.c\n", ifName);
			//return -1;
		}
		else
			printf("My local addr is: %s\n", inet_ntoa(my_addr));

		rv = get_local_address(ifName, &my_addr_broadcast, SIOCGIFBRDADDR);
		if(rv < 0)
		{
			fprintf(stderr, "Could not get local broadcast address using: %s see bipSocket.c\n", ifName);
			//return -1;
		}
		else
			printf("My Broadcast addr is: %s\n", inet_ntoa(my_addr_broadcast));


}

bool BacNetIp::connect() {

	/* Create the UDP socket */
	//bip_Socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	socket = epicsSocketCreate(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (socket < 0) {
		return -1;
	}

	if(port < 1)
	{
		port = htons(0xBAC0);
	}

	unsigned int sAddrLen;
	struct sockaddr_in sAddr;

	memset(&sAddr, 0, sizeof(sAddr));       /* Clear struct */
	sAddr.sin_family = AF_INET;                  /* Internet/IP */
	sAddr.sin_addr.s_addr = htonl(INADDR_ANY);  /* IP address */
	sAddr.sin_port = port;       /* server port */

	/* Bind the socket */
	sAddrLen = sizeof(sAddr);
	if (bind(socket, (struct sockaddr *) &sAddr, sAddrLen) < 0)
	{
		printf("Failed to bind server socket\n");
	}

	return socket;
}

BacNetIp::~BacNetIp() {
	// TODO Auto-generated destructor stub
}












