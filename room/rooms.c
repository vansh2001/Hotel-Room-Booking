/* ECE 209 - Fall 2017
* rooms.c -- calls Schedule.h and does room prints and reservation
*  Usage: rooms
*  The rooms does scheduling for number of rooms. Checks if schedule can be fitted
 * 
*  Grading preference: I would like this to be graded on an EOS machine
*  
*  H. Mareddy, Fall 2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schedule.h"
#include "rooms.h"

int totalRooms;
Schedule* storeRoom;


void setupRooms(int nrooms, Time open, Time close){
    storeRoom = (Schedule*) malloc(sizeof(Schedule)*nrooms); //create enough space for rooms
    totalRooms = nrooms; // Equal total rooms
    int j = 0;
    Schedule s;
    while(j<nrooms){
        s = createSchedule(open, close);
        storeRoom[j] = s;
        j++;
    }
}

/* function to return number of rooms --
 * must match value used in setupRooms call */
int numRooms(){
    return totalRooms;
}

/* reserve a time in the first available room */
/* return 1 if success, 0 if time not available in any room */
int makeReservation(const char* name, Time start, Time end){
    int j;
    int y;
    int check;
    for(j=0; j<totalRooms; j++) {
        y = isBusy(storeRoom[j], start, end);
        printf("Busy :%d\n", y);
        if(y == 0){ //Do this only if not busy
            printf("reserve worked");
            check = reserve(storeRoom[j], name, start, end); //call reserve from schdule.h
            return j+1; 
        } else{
            return 0; // not able to reserve
        }
    }
    return 0;
}

/* delete an existing reservation in a particular room */
/* return 1 if success, 0 if matching reservation not found */
int cancelReservation(int room, const char* name, Time start){
    Schedule s = storeRoom[room-1];
    return cancel(s, name, start); //Call cancle

}
//check witch time is less compare hours 
int lessTime(Time one, Time two) {
    if (one.hr == two.hr){
        if(one.min < two.min) {
            return 1;
        } 
        return 0;
    } else{
        if(one.hr < two.hr){
            return 1;
        } else{
            return 0;
        }
    }
}
//Manage plus reserve that can find reserve
void plusReserve(struct rNode* reserve, struct iNode* inter, const char* name, int rN){
  struct iNode* curr = inter;
  struct reservation new;
  struct rNode* current = reserve;
  //Alocate new memory
  struct rNode* newnode = (struct rNode*) malloc(sizeof(struct rNode));
  
  //Keep loop if not
  while(curr != NULL){
        if (!strcmp(curr->interval.owner, name)){
            new.roomNumber = rN;
            new.interval = &(curr->interval);
            
            while (lessTime(current->res.interval->end, new.interval->start)){
                curr = curr->next;
            }
            newnode->next = current->next;
            current->next = newnode;
        }
        curr = curr->next;
    }
    
}
/* create and return a list of reservations with the
 * specified owner, NULL (empty list) if no reservations */
struct rNode * findReservations(const char * name) {
    int j = 0;
    //Check to see nu
    struct rNode* new = NULL;
    //interate over 
    while(j<totalRooms) {
        plusReserve(new, storeRoom[j]->busy, name, j+1);
        plusReserve(new, storeRoom[j]->idle, name, j+1);
        j++;
     }
    return new;
}

/* print reservations from a list, using specified stream */
void printReservations(const struct rNode* list, FILE* stream){
    struct rNode* curr = (struct rNode*) list;
    
    while(curr != NULL){
        fprintf(stream, "ROOM %d: ", curr->res.roomNumber);
        fprintf(stream, "%02d:", curr->res.interval->start.hr);
        fprintf(stream, "%02d - ", curr->res.interval->start.min);
        fprintf(stream, "%02d:", curr->res.interval->end.hr);
        fprintf(stream, "%02d ", curr->res.interval->end.min);
        fprintf(stream, "%s\n", curr->res.interval->owner);

        curr = curr->next;   
    }
    
}

/* print schedule for every room, using specified stream */
void printRoomSchedules(FILE* stream){
    
    int j;
    for(j=0; j<totalRooms; j++){
        fprintf(stream, "\n");
        fprintf(stream, "Room %d:\n", j+1);
        printSchedule(storeRoom[j], stream);
    }
    
}