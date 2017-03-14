/*
 * Practical.h
 *
 *  Created on: 2017. 3. 4.
 *      Author: Administrator
 */

#ifndef PRACTICAL_H_
#define PRACTICAL_H_

#include <stdbool.h>
#include <stdio.h>
#include <sys/socket.h>

// Handle error with user msg
// ����� �޽����� ���� ó��
void DieWithUserMessage(const char *msg, const char *detail);
// Handle error with system msg
// �ý��� �޽����� ���� ó��
void DieWithSystemMessage(const char *msg);
// Print socket address
// ���� �ּ� ���
void PrintSocketAddress(const struct sockaddr *address, FILE *stream);
// Test socket address equality
// ���� �ּ� ����ȭ �׽�Ʈ
bool SockAddressEqual(const struct sockaddr *addr1, const struct sockaddr *addr2);
// Create, bind, and listen a new TCP server socket
int SetupTCPServerSocket(const char *service);
// Accept a new TCP connection on a server socket
int AcceptTCPConnection(int servSock);
// Handle new TCP client
void HandleTCPClient(int clntSocket);
// Create and connect a new TCP client socket
int SetupTCPClientSocket(const char *server, const char *service);

enum sizeConstants {
	MAXSTRINGLENGTH = 128, BUFSIZE = 512,
};

#endif // PRACTICAL_H_

