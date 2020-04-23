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


typedef struct Room 
{
    char* name; 
    char* type; 
    int numOutbound;  
    struct Room* outboundCon[6]; 
} Room; 



void shuffleArray(char **array, int n); 
void createDir(int pid);
bool sameRoom(Room* x, Room* y);
bool canAddOutboundFrom(Room* x);
void connectRooms(Room* x, Room* y);
void addRandomConnection(Room* room);
void createFiles(int pid, Room* roomArray);
bool connectionExists(Room* x, Room* y); 
void outputFiles(Room* rooms); 
bool isGraphComplete(Room* rooms); 

void generateGraph(Room* rooms); 


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

bool sameRoom(Room* x, Room* y)
{
    assert(x && y); 
    if (x == y)
    {
        return true; 
    }
    else
    {
        return false; 
    }
    
}


bool canAddOutboundFrom(Room* x)
{

    if (x->numOutbound < 6)
    {
        return true; 
    }
    else
    {
        return false; 
    }
    
    
}


bool connectionExists(Room* x, Room* y)
{
    assert(x && y); 
    int i = 0; 
    for (i = 0; i < x->numOutbound; i++)
    {
        if (x->outboundCon[i] == y) return true; 
    }
    return false; 

}

void connectRooms(Room* x, Room* y)
{
    if (canAddOutboundFrom(x) || canAddOutboundFrom(y))
    {
        x->outboundCon[x->numOutbound++] = y; 
        y->outboundCon[y->numOutbound++] = x; 
    }
    
}



void addRandomConnection(Room* room)
{
    assert(room); 
    Room* x; 
    Room* y; 

    do 
    {
        x = &room[rand() % 7]; 
    }while (!canAddOutboundFrom(x)); 

    do 
    {
        y = &room[rand() % 7]; 
    }while (!canAddOutboundFrom(y) || sameRoom(x, y) || connectionExists(x, y));

    connectRooms(x,y); 
}


bool isGraphComplete(Room* rooms)
{
    int i, con;
    for (i = 0; i < 7; i++) {
        con = rooms[i].numOutbound;
        if (con < 3 || con > 6)
            return false;
    }


    return true;
}

void createFiles(int pid, Room* rooms)
{
    int i, j, r; 
  
    char file[100];
     
    sprintf(dir, "jonewill.rooms.%d", pid); 

    for (i = 0; i < 7; i++)
    {
        
     
        char file[100];
        for (j = 0; j < 100; j++) file[j] = '\0'; 
        {
        srand(time(NULL)); 

        strcat(file, dir); 
        strcat(file, "/"); 
        strcat(file, fileNames[i]); 

        FILE* fileDescriptor = fopen(file, "w"); 
      
        fprintf(fileDescriptor, "NAME: %s\n", roomNames[i]); 
        
        fprintf(fileDescriptor, "Room Type: %s\n", rooms[i].type); 
      
        
        

         for (r = 0; r < rooms[i].numOutbound; r++)
        {
            fprintf(fileDescriptor, "Connection %d: %s\n", r+1, rooms[i].outboundCon[r]->name);
        }
        fclose(fileDescriptor); 
        }
 
        
    }
}


void generateGraph(Room* rooms)
{

    int i, j; 
    int randomNumber1 = 0;
    int randomNumber2 = 0;

    srand(time(NULL)); 
    while (randomNumber1 == randomNumber2)
    {
        randomNumber1 = (rand() % 7); 
        randomNumber2 = (rand() % 7); 
    }

    shuffleArray(roomNames, 10);
    for (i = 0; i < 7; i++)
    {
        rooms[i].name = roomNames[i];
        if (i == randomNumber1)
        {
            rooms[i].type = "Start Room"; 
        } 
        else if(i == randomNumber2)
        {
            rooms[i].type = "End Room"; 
        }
        else
        {
            rooms[i].type = "Mid Room"; 
        }

        rooms[i].numOutbound = 0; 
        for (j = 0; j < 6; j++) rooms[i].outboundCon[j] = NULL;
    }


    

    while(isGraphComplete(rooms) == false )
    {
        addRandomConnection(rooms);
    }
}



int main()
{
    Room roomArray[7]; 
    int pid = getpid(); 
    generateGraph(roomArray); 
    createDir(pid); 
    
    createFiles(pid, roomArray); 
  
    return 0; 
}