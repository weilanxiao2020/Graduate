/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial API and implementation and/or initial documentation
 *    Sergio R. Caprile - "commonalization" from prior samples and/or documentation extension
 *******************************************************************************/


#if !defined(SOCKET_ERROR)
	/** error in socket operation */
	#define SOCKET_ERROR -1
#endif

#define INVALID_SOCKET SOCKET_ERROR
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
This simple low-level implementation assumes a single connection for a single thread. Thus, a static
variable is used for that connection.
On other scenarios, the user must solve this by taking into account that the current implementation of
MQTTPacket_read() has a function pointer for a function call to get the data to a buffer, but no provisions
to know the caller or other indicator (the socket id): int (*getfn)(unsigned char*, int)
*/
// static int mysock = INVALID_SOCKET;

// /**
//   * 发送缓冲区数据 
//   * @param sock socket编号
//   * @param buf 发送缓冲区
//   * @param buflen 缓冲区长度
//   */
// int transport_sendPacketBuffer(int sock, unsigned char* buf, int buflen)
// {
// 	int rc = 0;
// 	rc = write(sock, buf, buflen);
// 	return rc;
// }

// /**
//   * 接收数据到缓冲区 
//   * @param buf 接收缓冲区
//   * @param count 接收缓冲区长度
//   * @return 实际接收的长度
//   */
// int transport_getdata(unsigned char* buf, int count)
// {
// 	int rc = recv(mysock, buf, count, 0);
// 	//printf("received %d bytes count %d\n", rc, (int)count);
// 	return rc;
// }

// int transport_getdatanb(void *sck, unsigned char* buf, int count)
// {
// 	int sock = *((int *)sck); 	/* sck: pointer to whatever the system may use to identify the transport */
// 	/* this call will return after the timeout set on initialization if no bytes;
// 	   in your system you will use whatever you use to get whichever outstanding
// 	   bytes your socket equivalent has ready to be extracted right now, if any,
// 	   or return immediately */
// 	int rc = recv(sock, buf, count, 0);	
// 	if (rc == -1) {
// 		/* check error conditions from your system here, and return -1 */
// 		return 0;
// 	}
// 	return rc;
// }

// /**
// return >=0 for a socket descriptor, <0 for an error code
// @todo Basically moved from the sample without changes, should accomodate same usage for 'sock' for clarity,
// removing indirections
// */
// int transport_open(char* addr, int port)
// {
// 	return mysock;
// }

// /**
//   * 关闭socket
//   * @param sock socket标识符
//   */
// int transport_close(int sock)
// {
// 	int rc;

// 	rc = shutdown(sock, 0);
// 	rc = recv(sock, NULL, (size_t)0, 0);
// 	rc = close(sock);

// 	return rc;
// }
