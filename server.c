/*
 * SERVER STEPS:
 *
 * create()	- create tcp socket
 *
 * bind()	- bind the socket to a server address
 *
 * listen()	- put the server socket in a passive mode where it waits for
 * 		  the client to approach the server to make a connection
 *
 * accept()	- connection established between client and server making them
 * 		  ready to transfer data
 */

#include	<stdio.h>
#include	<netdb.h>		// network database operations
#include	<netinet/in.h>		// internet address family
#include	<stdlib.h>
#include	<string.h>
#include	<sys/socket.h>		// main sockets header
#include	<sys/types.h>
#include	<unistd.h>		// for read(), write(), close()

#define MAX 80
#define PORT 8080
#define SA struct sockaddr

//function designed for chat between client and server:
void func(int connfd)
{
	char buff[MAX];	// buffer to store a string
	int n;

	//infinite loop for chat
	for (;;) {
		bzero(buff, MAX);

		//read the message from the client and copy it into the buffer
		read(connfd, buff, sizeof(buff));
		//print buffer which contains the client contents
		printf("From client: %s\t To client : ", buff);
		bzero(buff, MAX);
		n = 0;
		//copy server message from client and copy it into the buffer
		while ((buff[n++] = getchar()) != '\n');
			;

		// send the buffer to the client
		write(connfd, buff, sizeof(buff));

		// if message contains "exit" then server exit and chat ends
		if (strncmp("exit", buff, 4) == 0) {
		       printf("Server exit...\n");
		       break;
		}
	}
}

// Driver function
int main(void)
{
	int sockfd, connfd, len;

	// structure that contains the family, port and IP address
	struct sockaddr_in servaddr, cli;

	//socket create and verification

	/* 
	 * socket(int domain, int type, int protocol)
	 *
	 * socket() creates an endpoint for communication and returns a file
	 * descriptor that refers to that endpoint. The file descriptor
	 * returned by a successful call will be the lowest-numbered file
	 * decriptor not currently open for the process.
	 *
	 * domain	(communication domain), selects the protocol family to
	 * 		be used for communication.
	 *
	 * 		AF_INET (IPv4)
	 * 		AF_INET6 (IPv6)
	 *
	 * type		Specifies the communication semantics.
	 *
	 * 		SOCK_STREAM	provides sequenced, reliable, 2-way
	 * 				byte streams.
	 *
	 * protocol	specifies a particular protocol to be used with the
	 * 		socket. Usually only one protocol exists with a
	 * 		particular family, therefore usually 0.
	 *
	 * RETURN VALUE:
	 * 
	 * on error:	-1 and errno is set appropriately
	 * on success:	file descriptor is returned 
	 * 	
	 */

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	//for error in socket creation:
	if (sockfd == -1) {
		printf("Socket creation failed.Exiting....\n");
		exit(0);
	} else
		printf("Socket successfully created.\n");


	/*
	 * bzero(void *s, size_t n)
	 *
	 * erases the data in the n bytes of the memory starting at the location
	 * pointed  to by s, by writing zeros (bytes containing \0).
	 *
	 */
	bzero(&servaddr, sizeof(servaddr));

	// assign an IP and PORT
	servaddr.sin_family = AF_INET; //family

	/* 
	 * htonl() - converts unsigned int hostlong from host byte order to
	 * 		network byte order.
	 *
	 * htons() - converts unsigned short integer hostshort from host byte
	 * 		to network byt order.
	 */

	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); //server address
	servaddr.sin_port = htons(PORT); //server port
	
	// Binding newly created socket to the given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("Socket bind failed. Exiting...\n");
		exit(0);
	} else 
		printf("Socket successfully binded.\n");

	// Server ready to listen
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed. Exiting...\n");
		exit(0);
	} else
		printf("Server listening at %d on %d...\n", servaddr.sin_addr.s_addr, servaddr.sin_port);
	len = sizeof(cli);

	//accept data packet from client and verification
	
	connfd = accept(sockfd, (SA*)&cli, &len);

	if (connfd < 0) {
		printf("Server accept failed. Exiting...\n");
		exit(0);
	} else
		printf("Server accept client.\n");

	//function for chatting between the client and server
	func(connfd);

	//after chatting close the socket
	close(sockfd);
}
