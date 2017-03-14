#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "practical.h"

static const int MAXPENDING = 5;	// ���� ��û�� ����� �� �ִ� �ִ��

int main(int argc, char *argv[]) {
	if(argc != 2) // ��ɾ� ������ ���� Ȯ��
		DieWithUserMessage("Parameter(s)", "<Server Port>");
		
	in_port_t servPort = atoi(argv[1]); // ù��° ���� : ���� ��Ʈ
	
	// ���� ��û�� ó���ϴ� ���� ���� 
	int servSock; // ���� ���� �ĺ���
	if ( (servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 )
	   DieWithSystemMessage("socket() failed");
	   
 	// ���� �ּ� ����ü ����
 	struct sockaddr_in servAddr;	// ���� �ּ�
	memset(&servAddr, 0, sizeof(servAddr)); // 0���� ����ü �ʱ�ȭ
	servAddr.sin_family = AF_INET; // IPv4�ּ� �йи�
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // ȣ��Ʈ�� ��� IP�ε� ���� ��û ����
	servAddr.sin_port = htons(servPort);	// ������Ʈ
	 
	// ���� �ּҿ� ���ε�
	if (bind(servSock, (const struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
		DieWithSystemMessage("bind() failed");

	// ������ ������ ��û�� ó���� �� �ֵ��� ����
	if (listen(servSock, MAXPENDING) < 0)
		DieWithSystemMessage("listen() failed");

	for (;;) { // ���� ����
		struct sockaddr_in clntAddr; // Ŭ���̾�Ʈ �ּ�
		// Ŭ���̾�Ʈ �ּ� ����ü�� ũ�� ����
		socklen_t clntAddrLen = sizeof(clntAddr);

		// Ŭ���̾�Ʈ ������ ��ٸ�
		int clntSock = accept(servSock, (struct sockaddr*)&clntAddr, &clntAddrLen);
		if (clntSock < 0)
			DieWithSystemMessage("accept() failed");


		// clntSock�� Ŭ���̾�Ʈ�� �����!

		char clntName[INET_ADDRSTRLEN]; // Ŭ���̾�Ʈ �ּҸ� ��� ���� ���ڿ�
		if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName, sizeof(clntName)) != NULL)
			printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
		else
			puts("Unable to get client address");

		HandleTCPClient(clntSock);
	}
	// ������� ����	
} 

