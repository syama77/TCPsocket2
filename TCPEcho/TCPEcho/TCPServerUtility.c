#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include "Practical.h"

static const int MAXPENDING = 5; // Maximum outstanding connection requests

int SetupTCPServerSocket(const char *service) {
	// Construct the server address structure
	struct addrinfo addrCriteria;                   // Criteria for address match
	memset(&addrCriteria, 0, sizeof(addrCriteria)); // Zero out structure
	addrCriteria.ai_family = AF_UNSPEC;             // Any address family
	addrCriteria.ai_flags = AI_PASSIVE;             // Accept on any address/port
	addrCriteria.ai_socktype = SOCK_STREAM;         // Only stream sockets
	addrCriteria.ai_protocol = IPPROTO_TCP;         // Only TCP protocol

	struct addrinfo *servAddr; // List of server addresses
	int rtnVal = getaddrinfo(NULL, service, &addrCriteria, &servAddr);
	if (rtnVal != 0)
		DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtnVal));

	int servSock = -1;
	for (struct addrinfo *addr = servAddr; addr != NULL; addr = addr->ai_next) {
		// Create a TCP socket
		servSock = socket(addr->ai_family, addr->ai_socktype,
			addr->ai_protocol);
		if (servSock < 0)
			continue;       // Socket creation failed; try next address

							// Bind to the local address and set socket to listen
		if ((bind(servSock, addr->ai_addr, addr->ai_addrlen) == 0) &&
			(listen(servSock, MAXPENDING) == 0)) {
			// Print local address of socket
			struct sockaddr_storage localAddr;
			socklen_t addrSize = sizeof(localAddr);
			if (getsockname(servSock, (struct sockaddr *) &localAddr, &addrSize) < 0)
				DieWithSystemMessage("getsockname() failed");
			fputs("Binding to ", stdout);
			PrintSocketAddress((struct sockaddr *) &localAddr, stdout);
			fputc('\n', stdout);
			break;       // Bind and listen successful
		}

		close(servSock);  // Close and try again
		servSock = -1;
	}

	// Free address list allocated by getaddrinfo()
	freeaddrinfo(servAddr);

	return servSock;
}

int AcceptTCPConnection(int servSock) {
	struct sockaddr_storage clntAddr; // Client address
									  // Set length of client address structure (in-out parameter)
	socklen_t clntAddrLen = sizeof(clntAddr);

	// Wait for a client to connect
	int clntSock = accept(servSock, (struct sockaddr *) &clntAddr, &clntAddrLen);
	if (clntSock < 0)
		DieWithSystemMessage("accept() failed");

	// clntSock is connected to a client!

	fputs("Handling client ", stdout);
	PrintSocketAddress((struct sockaddr *) &clntAddr, stdout);
	fputc('\n', stdout);

	return clntSock;
}


//void HandleTCPClient(int clntSocket) {
//	char uffer[BUFSIZE]; // 에코 문자열을 위한 버퍼
//	
//	// 클라이언트로부터 메시지 수신
//	ssize_t numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
//	if (numBytesRcvd < 0)
//		DieWithsystemMessage("recv() failed");
//
//	// 수신한 문자열을 전송하고 여분의 데이터를 스트림이 끝날 때까지 수신
//	while (numBytesRcvd > 0) { // 0일 경우 스트림의 종료를 의미
//		// 클라이언트를 에코 메시지를 돌려보냄
//		ssize_t numBytesSent = send(clntSocket, buffer, numBytesRcvd, 0);
//		if (numBytesSent < 0)
//			DieWithSystemMessage("send() failed");
//		else if (numBytesSent != numBytesRcvd)
//			DieWithUserMessage("send()", "sent unexpected number of bytes");
//
//		// 받을 수 있는 데이터가 더 남아있는지 확인
//		numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
//		if (numBytesRcvd < 0)
//			DieWithSystemMessage("recv() failed");
//	}
//	close(clntSocket); // 클라이언트 소켓 종료 
//}
void HandleTCPClient(int clntSocket) {
	char buffer[BUFSIZE]; // Buffer for echo string

						  // Receive message from client
	ssize_t numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
	if (numBytesRcvd < 0)
		DieWithSystemMessage("recv() failed");

	// Send received string and receive again until end of stream
	while (numBytesRcvd > 0) { // 0 indicates end of stream
							   // Echo message back to client
		ssize_t numBytesSent = send(clntSocket, buffer, numBytesRcvd, 0);
		if (numBytesSent < 0)
			DieWithSystemMessage("send() failed");
		else if (numBytesSent != numBytesRcvd)
			DieWithUserMessage("send()", "sent unexpected number of bytes");

		// See if there is more data to receive
		numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
		if (numBytesRcvd < 0)
			DieWithSystemMessage("recv() failed");
	}

	close(clntSocket); // Close client socket
}