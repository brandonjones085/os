//#################################################################
//#Name: Smallsh
//#Purpose: Program creates a mini shell
//#Created By: Brandon Jones
//#Description: 
//Allows for redirection
//exit, cd, status, and commenting
//Run processes in background or foreground
//##################################################################


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h> 
#include <stdbool.h> 


bool foreground = false; 

//#################################################################
//#Name: catchSIGINT
//#Purpose: moves program from background to foreground
//#Input: Takes in signal
//#Output: Outputs message to screen 
//#Description: 
//Checks if the foreground bool is true or false, 
//changes bool, and prints message to screen. 
//##################################################################
void catchSIGINT(int signo)
{
	char* message; 
	
	if (foreground == false)
	{
		foreground=true;	
		message = "\nEntering foreground-only mode (& is now ignored)\n: ";
		write(STDOUT_FILENO, message, 52);
		fflush(stdout);	
	}
	else
	{
		foreground=false;
		message = "\nExiting foreground-only mode\n: ";
		write(STDOUT_FILENO, message, 32);
		fflush(stdout);
		
	}
}



//#################################################################
//#Name: Main
//#Purpose: Runs the majority of the program
//#Input: NA
//#Output: The number of rows and columns
//#Description: Takes in a matrix and returns the number of col and rows
//##################################################################

int main()
{

	//local variables
	char* buf1; 
	char* line = malloc(2048); 
	char line2[2048]; 
	char* inFile = NULL; 
	char* outFile = NULL; 
	char* buf3; 
	char* buf4; 
	int numChar; 
	int index = 0; 
	int file; 
	int sig = 0; 
	int run = 0; 
	int num = 0; 
	int p = -5; 
	bool back = false; 
	pid_t pid = -5; 
	pid_t pid2[512];  
	char* array[512]; 
	char pi[255];
	char buf5[2048];

	//Ctrl C signal 
	struct sigaction action={0}; 
	action.sa_handler = SIG_IGN; 
	sigaction(SIGINT, &action, NULL); 

	//Ctrl Z signal 
	struct sigaction SIGINT_action={0}; 
	SIGINT_action.sa_handler=&catchSIGINT; 
	sigfillset(&SIGINT_action.sa_mask); 
	sigaction(SIGTSTP, &SIGINT_action, NULL); 

	while(!run) //runs until exit 
	{
	back = false; //background initally false
	printf(": ");
	fflush(stdout); 
	ssize_t size = 0; //holds size of line
	numChar = getline(&line, &size, stdin);  //userinput line

	index = 0; 
	buf1 = strtok(line, " \n"); //filters user input

	while(buf1 != NULL)   //until end of input
	{
		if (strcmp(buf1, "<") == 0)  //looks for < 
		{
			buf1=(strtok(NULL, " \n"));  //used for name
			inFile = strdup(buf1);    //file into input file
			buf1=strtok(NULL, " \n"); //after < 
		}
		else if (strcmp(buf1, ">") == 0) //looks for > 
		{
			buf1=(strtok(NULL, " \n")); // used for name
			outFile = strdup(buf1);     // file into output file
			buf1 = strtok(NULL, " \n"); // after > 
		}
		else
		{
			//expanding pid
			if(strstr(buf1, "$$") != NULL) //finds the $$
			{
				
				p = getpid(); 
				strcpy(line2, buf1); //copies buf1 to line2
				
				buf3 = line2; //sets line2 to buf
				buf4 = buf3; //sets buf3 to buf4

				//used in concat
				sprintf(pi, "%d", p); //converts to string
				while((buf4 = strstr(buf4,"$$")))
				{
					//copies to buf5 and concats pid 
					strncpy(buf5, buf3, buf4-buf3); //buf up to $$
					buf5[buf4-buf3]='\0';  //terminates end
					strcat(buf5, pi); //
					strcat(buf5, buf4 + 2); //buf4 + $$
					strcpy(buf3, buf5);
					buf4+=1;  //increments
				}

				buf1=line2; 
			}
			array[index]=strdup(buf1);  //stores arguments into array
			buf1 = strtok(NULL, " \n"); //moves to next 
			index+=1; 
		}
	}

	//checks for & argument as last argument
	//if found sets bool to true
	if (strcmp(array[index-1], "&") == 0)
	{
		//printf("BACKGROUND\n"); 
		array[index] = "\0"; 
		if (foreground ==false)
		{
			back = true; 
		}
	}


	array[index] = NULL; //sets last index to NULL


	if(array[0] == NULL) //empty array do nothing
	{
		
	}
	else if (strcmp(array[0], "#") == 0) //comment out do nothing
	{
		
	}
	else if (strcmp(array[0], "exit") == 0) //argument is exit, set sig to 0
	{
		sig = 0; 
		int i = 0; 
		//printf("EXITING\n"); 

		for (i = 0; i < num; i++)
		{
			kill(pid2[i], SIGTERM); //cleans up background processes
		}
		run = 1; //for exit
		exit(sig); 
	}
	else if (strcmp(array[0], "status") == 0) //argument is status
	{
		if (WIFEXITED(sig))
		{
			printf("Exit Status: %d\n", WEXITSTATUS(sig)); //printing status
		}

		fflush(stdout); 
	}
	else if (strcmp(array[0], "cd") == 0)  //argument is cd
	{
		//printf("CHANGING DIRECTORY"); 
		if (array[1] == NULL) //no argument
		{
			chdir(getenv("HOME")); 
		}
		else
		{
			chdir(array[1]); //with argurment
		}

	}

	else
	{

	
	pid = fork();  //forks child 
	if (pid == -1)  //error, prints message, and exit
	{
		int i = 0; 
		perror("Hull Breach"); 
		for (i = 0; i < num; i++)
		{
			kill(pid2[i], SIGTERM);  //cleans up background proccess
		}
		fflush(stdout); //flushes 
		sig = 1; 
		exit(sig); //exits with 1

	}
	else if (pid == 0) //no error
	{
		if(back == false) //forground
		{
			action.sa_handler = SIG_DFL; //resets defaults
			sigaction(SIGINT, &action, NULL); 
		}
		
		if(inFile != NULL) //if inFile exists
		{
			file = open(inFile, O_RDONLY); //opens for read only
			if (file == -1)// File does not exist, and exit
			{
				fprintf(stderr, "Error with file one %s\n", inFile);
				fflush(stdout); 
				exit(1); 
			}
			if(dup2(file, 0) == -1) //error with redirecting 
			{
				fprintf(stderr, "Error with dup2\n"); 
				fflush(stdout); 
				exit(1); 
			}
			close(file); 
		}

		if(back == true) //background process
		{
			if (inFile==NULL)
			{
				file = open("/dev/null", O_RDONLY);  //opens /dev/null as read only
			if (file == -1) //if error, prints error message
			{
				fprintf(stderr, "Error with background /dev/null\n"); 
				fflush(stdout); //flush 
				exit(1); 
			}
			if (dup2(file, 0) == -1) //if error with dup2, print error
			{
				fprintf(stderr, "Error with background dup2"); 
				fflush(stdout); 
				exit(1); 
			}
			close(file); 
			}
			
			//very similar to infile
			if (outFile==NULL)
			{
				file = open("/dev/null", O_WRONLY); //write only
			if (file == -1) //if error opening, print message
			{
				fprintf(stderr, "Error with background /dev/null\n"); 
				fflush(stdout); 
				exit(1); 
			}
			if (dup2(file, 0) == -1)  //error with dup2, print message
			{
				fprintf(stderr, "Error with background dup2"); 
				fflush(stdout); 
				exit(1); 
			}
			close(file); 
			}

			
		}
		else //foreground
		{
			
		if(outFile != NULL) //ouput file is not null
		{
			file = (open(outFile, O_WRONLY|O_CREAT|O_TRUNC, 0644)); //creates, writes, appends

			if (file == -1) //file does not exist and exit
			{
				fprintf(stderr, "Error with file two %s\n", outFile); 
				fflush(stdout); 
				exit(1); 
			}
			if (dup2(file, 1) == -1) //error with redirecting
			{
				fprintf(stderr, "Error with dup2\n"); 
				fflush(stdout); 
				exit(1); 
			}
			close(file); 
		}
		}

		if (execvp(array[0], array)) //executes arguments
		{
			fprintf(stderr, "Error with command: %s\n", array[0]); 
			fflush(stdout); 
			exit(1); 
		}
	}
	else
	{
		if (back == false)
		{
			do
			{
				waitpid(pid, &sig, 0);  //waits for child to complete 
			} while (!WIFEXITED(sig));
		}

	

		if(back == true)
		{
			waitpid(pid, &sig, WNOHANG); 
			pid2[num] = pid; 
			printf("Process %d in background\n", pid); 
			fflush(stdout); 
			num+=1; 

		}
		
	}

	
	}

	int i=0; 
	for (i = 0; i < index; i++) //sets array to null for next
	{
		array[i] = NULL; 
	}

	inFile = NULL; //resets to NULL
	outFile = NULL; 

	//checks for background proccesses
	pid = waitpid(-1, &sig, WNOHANG);

	//loops through pids
	while (pid > 0)
	{
		printf("Background Process %d complete\n", pid); 
		if (WIFEXITED(sig))
		{
			printf("Exited with %d\n", WEXITSTATUS(sig)); 
			fflush(stdout); 
		}
		else
		{
			printf("Terminated with %d\n", sig); 
			fflush(stdout); 
		}
		pid = waitpid(-1, &sig, WNOHANG);  //checks next
		
	}

	}

	return 0;
}
