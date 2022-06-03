#include <stdio.h>		//printf
#include <string.h>		//strlen
#include <sys/socket.h> //socket
#include <arpa/inet.h>	//inet_addr
#include <poll.h>		//timer for fget
#include <unistd.h>

int main(int argc, char *argv[])
{
	int sock;
	struct sockaddr_in server;
	struct pollfd mypoll = {STDIN_FILENO, POLLIN | POLLPRI};
	char message[1000], server_reply[2000];
	char protocol_code[2000] = "0";
	const char ACCEPTED[2000] = "1";
	const char IN_QUEUE[2000] = "2";
	const char NOT_ACCEPTED[2000] = "3";

	// Create socket
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);

	// Connect to remote server
	if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
	// receive protocol code from server
	puts("Connected");
	if (recv(sock, protocol_code, 2000, 0) < 0)
	{
		puts("recv failed");
		return 0;
	}
	if (!strcmp(protocol_code, NOT_ACCEPTED))
	{
		puts("Server is busy, Connection terminated");
		return 0;
	}

	if (send(sock, protocol_code, strlen(protocol_code), 0) < 0)
	{
		puts("Send failed");
		return 1;
	}

	if (!strcmp(protocol_code, IN_QUEUE))
	{
		puts("You are in queue, Please Wait");
		if (recv(sock, protocol_code, 2000, 0) < 0)
		{
			puts("recv failed");
			return 0;
		}
	}
	puts("You are in echo chat, you have 10 seconds.");
	puts("Send 'q' as a message to quit");
	// keep communicating with server
	time_t now = time(NULL);
	time_t later = time(NULL);
	while (1)
	{
		printf("Enter message: ");
		fflush(stdout);
		if (poll(&mypoll, 1, (10 - (later - now)) * 1000))
		{
			fgets(message, 1000, stdin);
		}
		else
		{
			puts("\nYour time is up. You won't receive any more messages from server.");
			close(sock);
			return 0;
		}
		if (!strcmp(message, "q\n"))
		{
			puts("Leaving server...");
			close(sock);
			return 0;
		}
		// Send some data
		if (send(sock, message, strlen(message), 0) < 0)
		{
			puts("Send failed");
			return 1;
		}

		// Receive a reply from the server
		if (recv(sock, server_reply, 2000, 0) < 0)
		{
			puts("recv failed");
			break;
		}

		printf("Server Reply: %s\n", server_reply);
		memset(server_reply, 0, sizeof(server_reply));
		later = time(NULL);
	}

	close(sock);
	return 0;
}