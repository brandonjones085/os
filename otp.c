#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fcntl.h>

void error(const char *msg) { perror(msg); exit(0); } // Error function used for reporting issues

int main(int argc, char *argv[])
{
	int socketFD, portNumber, charsWritten, charsRead, file, text, i, key, len, cipher;
	struct sockaddr_in serverAddress;
	struct hostent* serverHostInfo;
	char buffer[128000];
	char buf2[128000]; 
	char buf4[128000]; 
	char buf3[1]; 
	char type; 
	char sendInfo[128000];
    
	
	// Set up the server address struct
	memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
	

	
   //user argv[2]; 
	//key argv[4]
	//otp post brandon plaintext key port



	if (strcmp(argv[1],"post") == 0)///is a post
	{

	portNumber = atoi(argv[5]); // Get the port number, convert to an integer from a string


	if(portNumber < 0)
	{
		printf("Error with port %d\n", portNumber); 
	}
	else
	{
		//printf("Connected on port %d\n", portNumber); 
	}

		if (argc < 6) { fprintf(stderr,"USAGE: %s post user plaintext key port\n", argv[0]); exit(0); } // Check usage & args

		memset(buffer, 0, 128000); 
		strcpy(sendInfo, "post "); 
		strcat(sendInfo, "NAME:"); 
		strcat(sendInfo, argv[2]); 


		serverAddress.sin_family = AF_INET; // Create a network-capable socket
		serverAddress.sin_port = htons(portNumber); // Store the port number
		serverHostInfo = gethostbyname("localhost"); 
		if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
		memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

		// Set up the socket
		socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
		if (socketFD < 0) error("CLIENT: ERROR opening socket");
		
		// Connect to server
		if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
			error("CLIENT: ERROR connecting");

		file = open(argv[3], O_RDONLY); //opens text file

		if(file < 0)
		{
			printf("Error opening text file\n"); 
			exit(1); 
		}

		
		text = read(file, buf4, 128000); //reads file stores in text
		//printf("Sending \"%s\"\n", buf4); 

		close(file); 

		key = open(argv[4], O_RDONLY);  //key file
		len = read(key, buf2, 128000);  //keys key stores in len

		if(key < 0)
		{
			printf("Error opening key\n"); 
			exit(1); 
		}


		close(key); 

		if (len < text)  //makes sure they match in length
		{
			printf("Key provided is too short\n"); 
			exit(1); 
		}


	int info = send(socketFD, sendInfo, sizeof(sendInfo), 0);
	
	

	memset(buf3, 0, sizeof(buf3)); // Clear out the buffer again for reuse
	int firstCharsRead = read(socketFD, buf3, 1); // Read data from the socket, leaving \0 at end
	
	
	if (firstCharsRead < 0) error("CLIENT: ERROR reading from socket");
	//printf("CLIENT: I received this from the server: \"%s\"\n", buffer);


	// Send message to server

	charsWritten = write(socketFD, buf4, text); // Write to the server
	//printf("Sending \"%s\"\n", buf4); 
	// if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
	// if (charsWritten < strlen(buffer)) printf("CLIENT: WARNING: Not all data written to socket!\n");

	// Get return message from server
	memset(buf3, 0, sizeof(buf3)); // Clear out the buffer again for reuse
	charsRead = read(socketFD, buf3, 1); // Read data from the socket, leaving \0 at end
	
	
	if (charsRead < 0) error("CLIENT: ERROR reading from socket");
	//printf("CLIENT: I received this from the server: \"%s\"\n", buffer);

	charsWritten = write(socketFD, buf2, len ); //sends to socket
	memset(buf3, 0, sizeof(buf3)); 
	int charsRec = read(socketFD, buf3, 1); 

	memset(buffer, 0, 128000); 

	do 
	{
		charsRead = read(socketFD, buffer, text); 
	}while( charsRead > 0); 

	for (i = 0; i < text; i++)
	{
		printf("%c", buffer[i]); 
	}
	printf("\n"); 

	close(socketFD); // Close the socket

	exit(0); 
		
	}



	//otp get user key port
	
	if (strcmp(argv[1],"get") == 0)///is a get
	{
		//printf("GET"); 
		portNumber = atoi(argv[4]); // Get the port number, convert to an integer from a string


		if(portNumber < 0)
		{
			printf("Error with port %d\n", portNumber); 
		}
		else
		{
			//printf("Connected on port %d\n", portNumber); 
		}
	// 	printf("GET"); 
		if (argc < 5) { fprintf(stderr,"USAGE: %s get user key port\n", argv[0]); exit(0); } // Check usage & args
	
		memset(buffer, 0, 128000); 
		strcpy(sendInfo, "get "); 
		strcat(sendInfo, "NAME:"); 
		strcat(sendInfo, argv[2]); 


		serverAddress.sin_family = AF_INET; // Create a network-capable socket
		serverAddress.sin_port = htons(portNumber); // Store the port number
		serverHostInfo = gethostbyname("localhost"); 
		if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(0); }
		memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

		// Set up the socket
		socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
		if (socketFD < 0) error("CLIENT: ERROR opening socket");
		
		// Connect to server
		if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to address
			error("CLIENT: ERROR connecting");

		key = open(argv[3], O_RDONLY); //opens key

		if(key < 0)
		{
			printf("Error opening key\n"); 
			exit(1); 
		}

		len = read(key, buf2, 128000);
		
		if(key < 0)
		{
			printf("Error opening key\n"); 
			exit(1); 
		}

		close(key); 

		int info = send(socketFD, sendInfo, sizeof(sendInfo), 0);
	
	

		memset(buf3, 0, sizeof(buf3)); // Clear out the buffer again for reuse
		int charsRead = read(socketFD, buf3, 1); // Read data from the socket, leaving \0 at end
	
	
		if (charsRead < 0) error("CLIENT: ERROR reading from socket");

		charsWritten = write(socketFD, buf2, len-1); //sends key to socket
		memset(buf3, 0, sizeof(buf3)); 
		int charsRec = read(socketFD, buf3, 1); //response

		memset(buffer, 0, 128000); 

		do
		{
			charsRead = read(socketFD, buffer, len-1); //decripted text
		} while (charsRead > 0);
		
		for(i = 0; i < len -1; i++)
		{
			printf("%c", buffer[i]); 
		}
		printf("\n"); 
		close(socketFD);

	}

	return 0;
}



