/*
 * BacNetIp.h
 *
 *  Created on: Aug 25, 2011
 *      Author: 8w4
 */

#ifndef BACNETIP_H_
#define BACNETIP_H_
/*
#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// Get ioctl...
#include <sys/ioctl.h>
#include <net/if.h>
*/
// EPICS Base
#include <osiSock.h>

#define BVLC_TYPE 0x81

class BacNetIp {
private:
	SOCKET socket;
	const char *ifName;
	in_addr my_addr;
	in_addr my_addr_broadcast;
	int port;

public:
	BacNetIp();
	bool connect();
	void disconnect();
	virtual ~BacNetIp();
};

#endif /* BACNETIP_H_ */
