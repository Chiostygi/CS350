#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h> //strlen
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <time.h>

#include <pthread.h> //for threading , link with lpthread

void *connection_handler(void *);

int client_counter = 0;
const char ACCEPTED[2000] = "1";
const char IN_QUEUE[2000] = "2";
const char NOT_ACCEPTED[2000] = "3";

int main(int argc, char *argv[])
{
    int socket_desc, new_socket, c, *new_sock;
    struct sockaddr_in server, client;
    char *message;

    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("bind failed");
        return 1;
    }
    puts("bind done");

    // Listen
    listen(socket_desc, 3);

    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while ((new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t *)&c)))
    {
        if (client_counter > 2)
        {
            puts("Server is busy, Connection terminated");
            send(new_socket, NOT_ACCEPTED, sizeof(NOT_ACCEPTED), 0);
            // close(new_socket);
        }
        else
        {
            puts("Connection accepted");
            if(client_counter < 2)
                send(new_socket, ACCEPTED, sizeof(ACCEPTED), 0);
            else
                send(new_socket, IN_QUEUE, sizeof(IN_QUEUE), 0);
            client_counter++;
            pthread_t sniffer_thread;
            new_sock = malloc(1);
            *new_sock = new_socket;
            if (pthread_create(&sniffer_thread, NULL, connection_handler, (void *)new_sock) < 0)
            {
                perror("could not create thread");
                return 1;
            }
            puts("Handler assigned");
        }
    }

    if (new_socket < 0)
    {
        perror("accept failed");
        return 1;
    }

    return 0;
}

void *connection_handler(void *socket_desc)
{
    // Get the socket descriptor
    int sock = *(int *)socket_desc;
    int read_size;
    char *message, client_message[2000];
    char client_protocol_code[2000] = "0";
    const char time_is_up[2000] = "Time is up";
    if (recv(sock, client_protocol_code, 2000, 0) < 0)
	{
		puts("recv failed");
		return 0;
	}
    while(strcmp(client_protocol_code,ACCEPTED)){
        if(client_counter <= 2){
            strcpy(client_protocol_code,ACCEPTED);
            send(sock,client_protocol_code,2000,0);
            break;
        }
    }
    // Receive a message from client
    time_t now = time(NULL);
    time_t later = time(NULL);
    while ((read_size = recv(sock, client_message, 2000, 0)) > 0)
    {
        // Send the message back to client
        write(sock, client_message, strlen(client_message));
        memset(client_message, 0, sizeof(client_message));
    }
    client_counter--;
/*     if(now - time(NULL) > 10) {
        write(sock, time_is_up, strlen(time_is_up));
    } */
    if (read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if (read_size == -1)
    {
        perror("recv failed");
    }

    // Free the socket pointer
    free(socket_desc);

    return 0;
}