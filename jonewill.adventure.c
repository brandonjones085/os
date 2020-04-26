#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <pthread.h>
#include <unistd.h>  //pid
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> //for bools
#include <time.h>  //random


struct Room
{
    char* fileName; 
    char* name; 
    char* type; 
    int numOutbound;  
    struct Room* outboundCon[6]; 
};



char newestDirName[256];
struct Room* rooms[7]; 



void open(char* dir); 
void getRooms( char* dir); 
void roomArrayInit(); 
void readFile(char* file, int i); 
void printArray(); 
int FindRoomByName(char* name);
int getRoom(char* r); 
void connectRoom(struct Room* x, struct Room* y);


void open(char* dir)
{
    time_t newestDirTime = -1; // Modified timestamp of newest subdir examined
    time_t time = 0; 
    char targetDirPrefix[32] = "jonewill.rooms."; // Prefix we're looking for
   

    char line[256]; 
    memset(newestDirName, '\0', sizeof(newestDirName));

    DIR* dirToCheck; // Holds the directory we're starting in
    struct dirent *fileInDir; // Holds the current subdir of the starting dir
    struct stat *dirAttributes = malloc(sizeof(struct stat)); // Holds information we've gained about subdir

    dirToCheck = opendir("."); // Open up the directory this program was run in

    if (dirToCheck > 0) // Make sure the current directory could be opened
    {
        while ((fileInDir = readdir(dirToCheck)) != NULL) 
        {
            if (strstr(fileInDir->d_name, targetDirPrefix) != NULL)
            {
                stat(fileInDir->d_name, dirAttributes); 
                newestDirTime = dirAttributes->st_mtime;
                if(newestDirTime > time)
                {
                    time = newestDirTime;
                    strcpy(newestDirName, fileInDir->d_name);
                    int i = 0; 
                    if((dirToCheck=opendir(newestDirName)) != NULL)
                    {
                        while ((fileInDir = readdir(dirToCheck)) != NULL)
                        {
                            if(strlen(fileInDir->d_name) > 2)
                            {
            
                                rooms[i]->fileName=fileInDir->d_name; 
                            
                                readFile(fileInDir->d_name, i); 
                                i++; 
                            }
                          
                        }
                    }
                    
                }
            }
        }
    }

  closedir(dirToCheck); // Close the directory we opened


}


void readFile(char* file, int i)
{

    char buf1[256]; 
    char buf2[256]; 
    char token[256]; 

   
    chdir(newestDirName);


    FILE* fileToOpen; 

     // printf("FILENAMES: %s\n", rooms[i].fileName);
    fileToOpen = fopen(file, "r"); 
    if(fileToOpen == NULL)
    {
        printf("ERROR\n"); 
    }
    else
    {
        while(fgets(buf1, sizeof(buf1), fileToOpen) != NULL)
        {


            strtok(buf1, ":");
           // printf("LINE: %s\n", buf1); 
            strcpy(buf2, strtok(NULL, "")); 
    
            if (strstr(buf1, "Room Type") != NULL)
            {
               if (strstr(buf2, "End"))
               {
                   rooms[i]->type = "End Room"; 
               }
               else if (strstr(buf2, "Mid"))
               {
                   rooms[i]->type = "Mid Room"; 
               }
               else
               {
                   rooms[i]->type = "Start Room"; 
               }
               
            }

            //If I have more time, I will find a more elegant way
            //To handle this. 
            //I spent a long time on this only to just get this working
            else if (strstr(buf1, "NAME") != NULL)
            {
                if (strstr(buf2, "Living Room"))
               {
                   rooms[i]->name = "Living Room"; 
               }
               else if (strstr(buf2, "Master Bedroom"))
               {
                   rooms[i]->name= "Master Bedroom"; 
               }
               else if (strstr(buf2, "Office"))
               {
                   rooms[i]->name = "Office"; 
               }
                else  if (strstr(buf2, "Kitchen"))
               {
                   rooms[i]->name = "Kitchen"; 
               }
               else if (strstr(buf2, "Bathroom 1"))
               {
                   rooms[i]->name = "Bathroom 1"; 
               }
               else if (strstr(buf2, "Guest Bedroom"))
               {
                   rooms[i]->name = "Guest Bedroom"; 
               }
                else if (strstr(buf2, "Bathroom 2"))
               {
                   rooms[i]->name = "Bathroom 2"; 
               }
               else if (strstr(buf2, "Front Porch"))
               {
                   rooms[i]->name= "Front Porch"; 
               }
               else if (strstr(buf2, "Garage"))
               {
                   rooms[i]->name = "Garage"; 
               }
               else
               {
                   rooms[i]->name = "Basement";
               }
               
            }

            
        }    

        while(fgets(buf1, sizeof(buf1), fileToOpen) != NULL)
        {
             strtok(buf1, ":");
          
            strcpy(buf2, strtok(NULL, "")); 
            if(strstr(buf1, "Connecti") != NULL)
            {
                //find index
                 printf("LINE: %s\n", buf1); 
                int ind = getRoom(buf2); 

                //add connection
                connectRoom(rooms[i], rooms[ind]); 
            }
        }
        
    }
    fclose(fileToOpen);
    
}


void connectRoom(struct Room* x, struct Room* y)
{
    int num = x->numOutbound; //gets num

    x->outboundCon[num] = y; //creates link
    x->numOutbound += 1; //increments int by one    
}


int getRoom(char* r)
{
    	int i=0;

	//search through the rooms array to find a room that matches the given name
	for (i = 0; i<7;i++)
	{
		if(strcmp(rooms[i]->name, r) == 0)
		{
			return i;
		}
		i++;
	}
}

void printArray()
{
    int i, j; 
     for (i = 0; i < 7; i++)
     {
       // printf("FILENAME %s", rooms[i]->fileName);
        printf("ROOM %s\n", rooms[i]->name);
        for (j = 0; j < rooms[i]->numOutbound; j++)
        {
            printf("CONNECTION: %s", rooms[i]->outboundCon[j]);
        }
    }
}


void roomArrayInit()
{
	int i, j;
	for (i = 0; i < 7; i++)
	{	
        rooms[i] = malloc(sizeof(struct Room)); 
		for (j = 0; j < 6; j++)
		{
			rooms[i]->outboundCon[j] = NULL;
		}
		rooms[i]->numOutbound = 0;

	}
}



int main()
{
    char dir[50];

    roomArrayInit(); 
    open(dir); 
    printArray();


   return 0;  
}