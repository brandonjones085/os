#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <limits.h> 

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues

int main(int argc, char *argv[])
{
	char dir[50]; 
	int numCiphers = 0; 
	int index = 1; 
	int listenSocketFD, establishedConnectionFD, portNumber, charsRead, pid, keylen, i, numChar, message, messageSent; 
	socklen_t sizeOfClientInfo;
	char buffer[128000];
	char buf2[128000]; 
	char buf3[128000]; 
	char buf4[128000]; 
	char* userBuf; 
	char* line; 
	struct sockaddr_in serverAddress, clientAddress;
	char* array[256]; 

	//validation 
	if (argc < 2) 
	{ 
		fprintf(stderr,"USAGE: %s port\n", argv[0]); 
		exit(1); 
	} 

	// Set up the address struct for this process (the server)
	memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
	serverAddress.sin_family = AF_INET; // Create a network-capable socket
	serverAddress.sin_port = htons(portNumber); // Store the port number
	serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

	// Set up the socket
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
	if (listenSocketFD < 0) 
	{
		printf("ERROR opening socket"); 
		exit(1); 
	}
	
	// Enable the socket to begin listening
	if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
	 // Connect socket to port
	 {
		 printf("Error with binding on port %d\n", portNumber); 
		 exit(2); 
	 }
	
	if (listen(listenSocketFD, 5) == -1 )// Flip the socket on - it can now receive up to 5 connections
	{
		printf("Unable to listen on port %d", portNumber); 
		exit(2); 
	}
	else
	{
		//printf("Listening on port %d\n", portNumber); 
	
	}
	
	// Accept a connection, blocking if one is not available until one connects
	sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
	

	while(1)
	{
	

	establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
	sleep(2); 
	memset(buffer, '\0', 128000);
	char type; 
	//int rec = recv(establishedConnectionFD, buffer, 128000, 0); 
	


	//error with accept function
	if (establishedConnectionFD < 0) error("ERROR on accept");

	memset(buffer, '\0', 256);
	int first = recv(establishedConnectionFD, buffer, sizeof(buffer), 0); // Read the client's message from the socket
	int firstMessageSent = write(establishedConnectionFD, "!", 1); //back to client

	//printf("RECEIVED \"%s\"\n", buffer); 


	


	pid = fork(); 

	if (pid < 0)
	{
		perror("Error with fork\n");
		fflush(stderr);  
	}

	if (pid == 0)
	{


	if (strstr(buffer, "post"))
	{
		printf("POST");

	memset(buffer, 0, 128000);
	memset(buf2, 0, 128000); 
	memset(buf4, 0, 128000); 

	charsRead = read(establishedConnectionFD, buf4, 128000); 
	
	messageSent = write(establishedConnectionFD, "!", 1); //back to client


	if (charsRead < 0) error("ERROR writing to socket");
	//messageSent = write(establishedConnectionFD, "!", 1); //back to client

	
	keylen = read(establishedConnectionFD, buf2, 128000);

	if (keylen < 0)
	{
		printf("Error with key on server\n"); 
		exit(2); 
	}


	// printf("KEY %d\n", keylen); 
	// printf("chars %d\n", charsRead); 
	if (keylen < charsRead) //checks for same length
	{
		
		printf("Error: Key and text do not match\n"); 
		exit(1); 
	}

	for (i = 0; i < charsRead; i++)
	{
		if (buffer[i] == 32)
		{
			buffer[i]=64; 
		}
		if(buf2[i] == 32)
		{
			buf2[i] = 64; 
		}

		int message = (int) buffer[i]; 
		int key = (int) buf2[i]; 

		message = message -64; 
		key = key - 64; 


		int cipher = (message + key) % 27; 

		cipher = cipher + 64; 

		buf3[i] = (char) cipher + 0; 

		if (buf3[i] == 64)
		{
			buf3[i] == 32; 
		}

	}

	
	struct dirent *fileInDir; 
	char file[100]; 
	char filePath[256]; 
	
	chdir("."); 
	strcat(file, "ciphertext1"); 
	
	FILE* fileD = fopen(file, "w");  

	fprintf(fileD, buf3); 
	fprintf(fileD, "\n"); 


	char *res = realpath("ciphertext1", buf2); 


	//sends file path back to client
	if(res)
	{
		printf("%s\n", buf2); 
		messageSent = write(establishedConnectionFD, buf2, charsRead); 
		exit(0); 
	}
	
	if (messageSent < charsRead)
	{
		printf("Error sending\n"); 
		exit(2); 
	}

	
	close(listenSocketFD);
	close(establishedConnectionFD); 
	exit(0); 


	}
	else 
	{
		printf("GET IS HERE"); 
	}
	

	}

	else
	{
		close(establishedConnectionFD); 
	}
	


	}

	return 0; 
}




