#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Practical.h"

int main(int argc, char *argv[]) {

	if (argc < 3 || argc > 4) // ��ɾ� ������ ��Ȯ�� ���� Ȯ��
		DieWithUserMessage("Parameter(s)", "<Server Address> <Echo Word> [<Server Port>]");

	char *servIP = argv[1]; 	// ù��° ���� : ���� IP �ּ�(dotted ����)
	char *echoString = argv[2]; // �ι�° ���� : �������� ���� ���ڿ�

	/* atoi(10�� ���ڿ��� ������ ��ȯ)
	 * ex 1) atoi("is 2007") == 0
	 * ex 2) atoi("2007 is") == 2007
	 */
	// ����° ����(����) : ���� ��Ʈ(��������). 7�� �߾˷��� ��Ʈ�� ���� �� 7�� ���
	in_port_t servPort = (argc == 4) ? atoi(argv[3]) : 7;

	/* scoket(AF_INET, SOCK_STREAN, IPPROTO_TCP)
	 * AF_INET(IPv4 ȯ��), SOCK_STREAN(��Ʈ�� ��� ����), IPPROTO_TCP(��)
	 * IPv4 ȯ�濡�� IPPROTO_TCP�� �Ҹ��� ��Ʈ�� ��� ���� ȣ��
	 * ������ ȣ�� = ������ �ĺ��� or �ڵ�(Handle) ��ȯ
	 * ���е� ȣ�� = -1
	 */
	// TCP�� ����Ͽ� ������ ��Ʈ�� ���� ����
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(sock < 0) // ���� socketȣ���� ���� �� DieWithSystemMessage ȣ��
		DieWithSystemMessage("socket() failed");

	// ���� �ּ� ����ü ����
	struct sockaddr_in servAddr;	// ���� �ּ�
	memset (&servAddr, 0, sizeof(servAddr));	// 0���� ����ü �ʱ�ȭ
	servAddr.sin_family = AF_INET;	// IPv4 �ּ� �йи�
	// �ּ� ��ȯ
	int rtnVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
	if (rtnVal == 0)
		DieWithUserMessage("inet_pton() failed", "invalid address string");
	else if(rtnVal < 0)
		DieWithSystemMessage("inet_pton() failed");
	servAddr.sin_port = htons(servPort); //  ���� ��Ʈ

	// ���� ������ ���� ����
	if(connect(sock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0 )
		DieWithSystemMessage("connect() failed");

	size_t echoStringLen = strlen(echoString);	// �Է¹��� ���ڿ��� ���̸� Ȯ��

	// ������ ���� ���ڿ� ����
	ssize_t numBytes = send(sock, echoString, echoStringLen, 0);
	if(numBytes < 0)
		DieWithSystemMessage("Send() failed");
	else if(numBytes != echoStringLen)
		DieWithUserMessage("send()", "sent unexpected number of bytes");

	// �����κ��� ������ ���ڿ� ����
	unsigned int totalBytesRcvd = 0; // ������ ���� ����
	fputs("Received: ", stdout); // �������� ���� ���ڿ� ����� ���� ����
	while(totalBytesRcvd < echoStringLen) {
		char buffer[BUFSIZE]; // ��/��� ����
		/* ���� ũ��(byte)��ŭ ������ ���� ����
		 * (�� ���ڸ� ���� 1byte ���ܳ���)  */
		numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
		if(numBytes < 0)
			DieWithSystemMessage("recv() failed");
		else if (numBytes == 0)
			DieWithUserMessage("recv()", "connection closed prematurely");
		totalBytesRcvd += numBytes; // �ѹ��� ũ�⸦ ���
		buffer[numBytes] = '\0'; // �ι��ڸ� �߰��Ͽ� ���ڿ� �ϼ�
		fputs(buffer, stdout);	// ���� ���۸� ���
	}
	fputc('\n', stdout); // ���������� ���� ���� ���
	close(sock);
	exit(0);
}
