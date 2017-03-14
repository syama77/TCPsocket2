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
// 사용자 메시지로 오류 처리
void DieWithUserMessage(const char *msg, const char *detail);
// Handle error with system msg
// 시스템 메시지로 오류 처리
void DieWithSystemMessage(const char *msg);
// Print socket address
// 소켓 주소 출력
void PrintSocketAddress(const struct sockaddr *address, FILE *stream);
// Test socket address equality
// 소켓 주소 동기화 테스트
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

