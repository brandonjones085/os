// rooms
// creates file that hold descriptions of rooms and how they're connected

// create directory jonewill.rooms.PID


// generate 7 rooms files

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>
#include <fcntl.h> 


char dir[20];
int* pid; 



struct Room 
{
    char* name; 
    char* type; 
    // outbound connections 
    // 
}; 


const char *fileNames[7] =
{
    "first room", 
    "second room", 
    "third room", 
    "fourth room", 
    "fifth room", 
    "sixth room", 
    "seventh room"
};


char *roomNames[10] = 
{
    "one", 
    "two", 
    "three", 
    "four", 
    "five", 
    "six", 
    "seven", 
    "eight", 
    "nine", 
    "ten"
};


struct Room rooms[7];

void shuffleArray(char **array, int n)
{
    size_t i; 
    srand(time(NULL)); 
    int t = 0; 
    for (i = 0; i < n -1; i++)
    {
        size_t j = i + rand() / (RAND_MAX / (n - i) + 1); 
        array[t] = array[j]; 
        array[j] = array[i]; 
        array[i] = array[t]; 
    }
}

// Creates directory 
void createDir(int pid)
{
    sprintf(dir, "jonewill.rooms.%d", pid); 
    int result = mkdir(dir, 0755);
    printf("Result of mkdir(): %d\n", result); 
    
}

void createFiles(int pid)
{
    int i, j; 
    int fileDescriptor; 
    char file[100];
    int randomNumber; 
    
    shuffleArray(roomNames, 10);

    sprintf(dir, "jonewill.rooms.%d", pid); 

    for (i = 0; i < 7; i++)
    {
        

        char file[100];
        for (j = 0; j < 100; j++) file[j] = '\0'; 
        {
        srand(time(NULL)); 
        randomNumber = (rand() % 10); 
        strcat(file, dir); 
        strcat(file, "/"); 
        strcat(file, fileNames[i]); 

        FILE* fileDescriptor = fopen(file, "w"); 
      
        fprintf(fileDescriptor, "NAME: %s\n", roomNames[i]); 
        rooms[i].name = roomNames[i];

        if (i == 0)
        {
            rooms[i].type = "Start Room"; 
        } 
        else if(i == 6)
        {
            rooms[i].type = "End Room"; 
        }
        else
        {
            rooms[i].type = "Mid Room"; 
        }
        fprintf(fileDescriptor, "Room Type: %s\n", rooms[i].type); 
        
        }
    }
}




int main()
{
    int pid = getpid(); 
    createDir(pid); 
    createFiles(pid); 

    return 0; 
}