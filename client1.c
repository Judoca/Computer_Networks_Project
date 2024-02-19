/*
 * CLIENT STEPS:
 *
 * - create a TCP socket
 * - connect a newly created client socket to server.
 */

#include	<arpa/inet.h>	// inet_addr()
#include	<netdb.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<strings.h>	// bzero
#include	<sys/socket.h>
#include	<unistd.h>	// read(), write(), close()

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

void func(int sockfd)
{
	char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		printf("Enter the string...\n");
		n = 0;
		while ((buff[n++] = getchar()) != '\n')
			;
		write (sockfd, buff, sizeof(buff));
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("From Server: %s", buff);
		if ((strncmp(buff, "exit", 4)) == 0) {
			printf("Client exit...\n");
			break;
		}
	}
}

int main(void)
{
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed.Exiting...\n");
		exit(0);
	} else
		printf("Socket successfully created.\n");

	bzero(&servaddr, sizeof(servaddr));

	// assign IP and port
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("10.1.19.187");
	servaddr.sin_port = htons(PORT);

	// connect the cliet socket to the server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		printf("Connection with server failed. Exiting...\n");
		exit(0);
	} else
		printf("Connected to the server.\n");

	//function for chat
	func(sockfd);

	//close the socket
	close(sockfd);

}
