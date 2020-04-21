
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

struct Room 
{
    int id; 
    char* name; 
    int numOutboundConnections; 
    struct room* outboundConnections[6];
};

int main()
{
  
    srand(time(NULL)); 
    int randomNumber = (rand() % 7) + 1; 
    struct room startRoom;
    startRoom.id = 0;
    startRoom.name = calloc(16, sizeof(char));
  

    strcpy(startRoom.name, "Living Room");

    printf("Starting Room's name and id are: (%s/%d)\n",
         startRoom.name, startRoom.id);

    printf("RANDOM NUMBER: %d", randomNumber);
}