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

char *fileNames[7] =
{
    "First_File", 
    "Second_File", 
    "Third_File", 
    "Fourth_File", 
    "Fifth_File", 
    "Sixth_File", 
    "Seventh_File"
};



char newestDirName[256];
struct Room* rooms[7]; 



void open(char* dir); 
void getRooms( char* dir); 
void roomArrayInit(); 
void readFile(char* file, int i); 
int printArray(char* r); 
int FindRoomByName(char* name);
int getRoom(char* r); 
void connectRoom(struct Room* x, struct Room* y);
void makeConnections(char* file, int i); 


void open(char* dir)
{
    
    int newestDirTime=-1; 
    char targetDirPrefix[32] = "jonewill.rooms."; // Prefix we're looking for
   

    char line[256]; 
    

    DIR* dirToCheck; // Holds the directory we're starting in
    struct dirent *fileInDir; // Holds the current subdir of the starting dir
    struct stat dirAttributes;

    dirToCheck = opendir("."); // Open up the directory this program was run in

    if (dirToCheck > 0) // Make sure the current directory could be opened
    {
        while ((fileInDir = readdir(dirToCheck)) != NULL) 
        {
            if (strstr(fileInDir->d_name, targetDirPrefix) != NULL)
            {
                stat(fileInDir->d_name, &dirAttributes); 
               
                if((int)dirAttributes.st_mtime > newestDirTime)
                {
                    newestDirTime = (int)dirAttributes.st_mtime;
                   // memset(newestDirName, '\0', sizeof(newestDirName)); 
                    strcpy(newestDirName, fileInDir->d_name);
                   

                    
                }
            }
        }
    }



                    int i = 0; 
                    int j = 0; 
                     int q=0; 
                    if((dirToCheck=opendir(newestDirName)) != NULL)
                    {
                        while ((fileInDir = readdir(dirToCheck)) != NULL)
                        {
                            if(strlen(fileInDir->d_name) > 2)
                            {
            
                                rooms[i]->fileName=fileInDir->d_name; 
                            
                               int m;
                                for (m = 0; m < 7; m++)
                                {
                                readFile(fileInDir->d_name, i); 
                                }
                                
                                //printf("FILES %s\n", fileInDir->d_name); 
                                //int p = getRoom(fileInDir->d_name); 
                           
     
                                i++; 
                            }


                        }
                    }





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
            if (buf1[strlen(buf1)-1] == '\n')
            {
                buf1[strlen(buf1)-1]='\0';
            }
            
            if (buf2[strlen(buf2)-1] == '\n')
            {
                buf2[strlen(buf2)-1]='\0';
            }

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
                   rooms[i]->name = "Living Room\n"; 
               }
               else if (strstr(buf2, "Master Bedroom"))
               {
                   rooms[i]->name= "Master Bedroom\n"; 
               }
               else if (strstr(buf2, "Office"))
               {
                   rooms[i]->name = "Office\n"; 
               }
                else  if (strstr(buf2, "Kitchen"))
               {
                   rooms[i]->name = "Kitchen\n"; 
               }
               else if (strstr(buf2, "Bathroom 1"))
               {
                   rooms[i]->name = "Bathroom 1\n"; 
               }
               else if (strstr(buf2, "Guest Bedroom"))
               {
                   rooms[i]->name = "Guest Bedroom\n"; 
               }
                else if (strstr(buf2, "Bathroom 2"))
               {
                   rooms[i]->name = "Bathroom 2\n"; 
               }
               else if (strstr(buf2, "Front Porch"))
               {
                   rooms[i]->name= "Front Porch\n"; 
               }
               else if (strstr(buf2, "Garage"))
               {
                   rooms[i]->name = "Garage\n"; 
               }
               else
               {
                   rooms[i]->name = "Basement\n";
               }
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
    int num=100; 
    char* p;
    for (i = 0; i < 7; i++)
   {  
      p=strstr(r, rooms[i]->name);
        if  (p)
        { 
            num = i; 
        }
    }
    return num; 
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
        rooms[i]->name='\0';

	}
}


void makeConnections(char* file, int index)
{
    char buf1[256]; 
    char buf2[256]; 
    char buf3[20];
    DIR* dirToCheck; // Holds the directory we're starting in
    struct dirent *fileInDir; // Holds the current subdir of the starting dir
    struct stat *dirAttributes = malloc(sizeof(struct stat)); // Holds information we've gained about subdir
    
    FILE* fileToOpen; 
    int i; 
    int n = 0; 

    fileToOpen = fopen(file, "r"); 
   // printf("FILE: %s\n", rooms[i]->fileName); 
    if(fileToOpen == NULL)
    {
        printf("ERROR"); 
    }
    else
    {
        //printf("SUCCESS"); 

         while(fgets(buf1, sizeof buf1, fileToOpen) != NULL)
         {

            memset(buf2, '\0', 256);
            strtok(buf1, ":");
            strcpy(buf2, strtok(NULL, "")); 

            if(buf1[strlen(buf1) -1] == '\n')
            {
                buf1[strlen(buf1)-1] = '\0';
            }

         
            buf2[strlen(buf2)-1] = '\n';
            
            strcpy(buf3, buf2);
             if (strstr(buf1, "Connect") != NULL)
             {
                // printf("SIZE %d", sizeof(buf2)); 
                // printf("BUF2 %s\n", buf2); 

                int ind = getRoom(buf2);
                // printf("BUF: %s");  
                //printf("ind: %d\n", ind);
                
               connectRoom(rooms[index], rooms[ind]); 
                // memset(buf2, '\0', 256); 
         
                // printf("I: %s", rooms[i]->name);
                // printf("CONNEC %s", buf2); 
            }
        }
    
    }
    
}


int main()
{
    char dir[50];
    memset(newestDirName, '\0', sizeof(newestDirName));
    roomArrayInit(); 
    open(dir); 
    char p[10];
    memset(p, '\0', 10);
    strcpy(p, "Office\n"); 
   int i, j, n; 
    printf("NEWEST DIR %s\n", newestDirName); 
    for (i = 0; i < 7; i++)
    {
    //   printf("FILE %s ", fileNames[i]); 
    //   printf("I: %d\n", i); 
      makeConnections(fileNames[i], i); 
    }

    for (j = 0; j < 7; j++)
    {
        printf("ROOM %s\n", rooms[j]->name); 
        for (n = 0; n < rooms[j]->numOutbound; n++)
        {
            printf("CONNECT %s\n", rooms[j]->outboundCon[n]->name);
        }
    }


   return 0;  
}