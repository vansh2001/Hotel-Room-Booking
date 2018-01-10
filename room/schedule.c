/* ECE 209 - Fall 2017
* schedule.h -- used by rooms.c, makes and checks if reservation can be made and prints the schedule
*  Usage: rooms
*  The rooms does scheduling for number of rooms. Checks if schedule can be fitted.
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

#define busyBefore -1
#define busyAfter -2
#define fail -3
#define NullEffectBusy 302
#define NullEffectIdle 301
#define moveOn 303

//Create new node function 

struct iNode* newNode(Time start, Time end) {
  
    struct iNode *new_node = (struct iNode*)malloc(sizeof(struct iNode));
    new_node->interval.start = start;
    new_node->interval.end = end;
    new_node->next = NULL; //Next value initialized to NULL
    
    return new_node; //return newly created time
}
//Time check
int checkTime (Time checkBusy, Time checkIdle){
    
    Time tempBusy = checkBusy;
    Time tempIdle = checkIdle;
    
    if(tempBusy.hr == tempIdle.hr ){
        
        //busy block is later than idle
        if(tempBusy.min > tempIdle.min) {
            return busyAfter; //Return Busy after Idle
            
        //busy block is before idle
        } else if(tempBusy.min < tempIdle.min){ 
            return busyBefore; //Return Busy before Idle
        
        } else if(tempBusy.min == tempIdle.min){
            return busyBefore;
            printf(" error in check time for min\n");
        }
 
    } else if(tempBusy.hr > tempIdle.hr){
           return busyAfter;

    } else if (tempBusy.hr < tempIdle.hr){
        return busyBefore; 
        
    } else{
            printf("error in check time");
            return 0; 
    }
    return 0;
}

/* allocate and return a new schedule */
Schedule createSchedule(Time start, Time end){

   Schedule new_schedule = (Schedule)malloc(sizeof(struct schedule));
   new_schedule->end = end;
   new_schedule->busy = NULL;
   new_schedule->idle = newNode(start,end);
   
   return new_schedule;
}

int convert(int hour,  int min){
    int total = (hour * 60) + (min);
    return total;
}


void shrinkIdle(Schedule s){
    struct iNode *curr = s->idle;
    struct iNode *next;
    
    while(curr->next != NULL){
        if(convert(curr -> interval.end.hr, curr->interval.end.min) == convert(curr->interval.start.hr, curr-> interval.end.min)){
            
            next = curr-> next;
            next = curr->next;
            curr->interval.end = next->interval.end;
            curr->next = next->next;
            //free(next);
        }
        curr = curr -> next; 
    }
}
/* check whether a desired period of time is available */
/* return 1 if busy, 0 if not busy */
int isBusy(Schedule s, Time start, Time end){
   
   struct iNode *curr = s->busy;
   int start1 = convert(start.hr, start.min);
   int end1  = convert(end.hr, end.min);
   int nstart = 0;
   int nend = 0;
   
   //Check for hours creater than open and close
   if( convert(start.hr, start.min) < convert(s->start.hr, s->start.min)){
       return 1;
       
   } else if(convert(end.hr, end.min) > convert(s->end.hr, s->end.min)) {
       return 1; 
   }
   
   //Check if time is allowed and if it busy
    if (s->busy == NULL){
       return 0;
        
    } else {
        while (curr->next != NULL){
 
            nstart = convert(curr->interval.start.hr, curr->interval.start.min);
            nend = convert(curr->interval.end.hr, curr->interval.end.min);
            
            if(start1 == nstart) {
                return 1;
            
            } else if (start1 >= nstart && start1 < nend){
                return 1;
            
            } else if(end1 <= nend && end1 > nstart){
                return 1;
             
            } else {
                curr = curr ->next;
            }  
            if (start1 < nstart && end1 > nend){
                return 1;
            } 
        }    
    }
    return 1;
}

struct iNode* insert (struct iNode* head, const char *name, Time start, Time end){
    
   // printf(" entred insert\n");
    
    struct iNode *newnode = newNode(start, end);
    strcpy(newnode->interval.owner,name);
    struct iNode *h;
    struct iNode *step;
    struct iNode *p_step;
    
    
    if(head == NULL){
        //printf("work 1\n");
        h = newnode;
        return head; 
    } 
    
    step = head;
    p_step = NULL;
    
    while((step != NULL) && convert(step->interval.start.hr, step->interval.start.min ) < convert(newnode->interval.start.hr , newnode->interval.start.min)) {
         printf("work 2\n");
        p_step = step;
        step = step -> next;
    } 
    
    if(p_step == NULL && convert(newnode->interval.start.hr, newnode->interval.start.min ) < convert(step->interval.start.hr , step->interval.start.min)){
        head = newnode;
        newnode -> next = step;
        printf("work 6\n");
        return head;
        
    } else if(p_step == NULL){
        head = newnode;
        printf("work 3\n");
    }
    else {
        printf("work 4\n");
        p_step -> next = newnode;
    }
    newnode -> next = step;
    printf("work 5\n");
    return head;
}

struct iNode* delete(struct iNode* head, Time start){
   
    struct iNode* curr = head;
    struct iNode* p_step = NULL;
    
    while(curr != NULL){ 
        if (convert(start.hr , start.min )  == convert(curr->interval.start.hr, curr->interval.start.min)) {

             if(p_step == NULL){
                 head = curr->next;
                 break;
                 
             } else {
                 p_step->next = curr->next;
                 curr->next = NULL;
                 break;
             }  
        }
        p_step = curr;
        curr = p_step->next;
        } 
    
    return head; /* means function failed to find specified time slot */
}   

/* mark a particular time interval as busy, for specified owner */
/* return 1 if success, 0 if could not reserve */
int reserve(Schedule s, const char *name, Time start, Time end){
    
    struct iNode *curr = s->busy;
    int check = 45;
    //struct iNode* newt = newNode(start, end);
//    strcpy()
    check = isBusy(s,start, end);
    //printf("%d",check);

    if (check == 1) {
        return 0;//Could not reserve 

    }else if(check == 0) {
        //printf("reserve worked");
        insert(s->busy, name, start, end);
        printf("reserve worked\n");
        shrinkIdle(s);
        return 1; 
        
    }
    return 0; 
}
/* free up a previously-reserved interval */
/* merge contiguous idle intervals */
/* return 1 if success, 0 if matching interval not found */
int cancel(Schedule s, const char *name, Time start){
    struct iNode* curr = s->busy; //Go through busy
    struct iNode* check;
    check = delete(curr, start);
    shrinkIdle(s);
    
    if(check == curr){
        return 1; //if return same as input then failed
    }
    return 0; // return 0 for success 
  }
     
//}

/* print all free and busy intervals to specified stream */
void printSchedule(Schedule s, FILE* stream){
    
    printf("printSchedule Sucess");
    struct iNode *busyStepper = s->busy;
    struct iNode *idleStepper = s->idle;

    int result, tracker1, tracker2   = 0; //store values
    //Keep in while until not 2
    while((tracker1 + tracker2) != 2){
       if((busyStepper == NULL) && (idleStepper == NULL)){
           printf("while");
           break; 
           //tracker1 = 1;
           //tracker2 = 1;
           
       } else if ((busyStepper == NULL) && (idleStepper != NULL)) {
           result = busyAfter;
           Time checkIdle = idleStepper->interval.start; 
           Time checkIdleEnd = idleStepper->interval.end;
           printf("Print 2\n");
           
       } else if ((busyStepper != NULL) && (idleStepper == NULL)){
           result = busyBefore;
           Time checkBusy = busyStepper->interval.start;
           Time checkBusyEnd = busyStepper->interval.end;
           printf("Print 1\n");
           
       } else {
            // initialize Times
               Time checkBusy = busyStepper->interval.start;
               Time checkBusyEnd = busyStepper->interval.end;
               Time checkIdle = idleStepper->interval.start; 
               Time checkIdleEnd = idleStepper->interval.end;
               result = checkTime(checkBusy, checkIdle); // Call to check time
               printf("Print 3\n");
       }
        if (result == busyBefore) {
            Time checkBusy = busyStepper->interval.start;
            Time checkBusyEnd = busyStepper->interval.end;

            fprintf(stream ,"BUSY ");
            fprintf(stream , "%02d:%02d - %02d:%02d %s\n",checkBusy.hr, checkBusy.min,checkBusyEnd.hr, checkBusyEnd.min, busyStepper->interval.owner);
            
            if (busyStepper -> next != NULL){
                
                busyStepper = busyStepper -> next; 
                checkBusy = busyStepper->interval.start;
                checkBusyEnd = busyStepper->interval.end;
                printf("Print 4\n");
 
            } else {
                busyStepper = busyStepper -> next; 
                tracker1 = 1;
                printf("Print 5\n");
            }
                
        }else if(result == busyAfter ){ //result = -2 
            Time checkIdle = idleStepper->interval.start; 
            Time checkIdleEnd = idleStepper->interval.end;
            fprintf(stream ,"IDLE ");
            fprintf(stream , "%02d:%02d - %02d:%02d\n",checkIdle.hr, checkIdle.min, checkIdleEnd.hr, checkIdleEnd.min);
               
            if (idleStepper -> next != NULL){
                  
                idleStepper = idleStepper -> next;  
                checkIdle  = idleStepper->interval.start;
                checkIdleEnd = idleStepper->interval.end;
                printf("Print 5\n");
     
            } else {
                idleStepper = idleStepper -> next;
                tracker2 = 1; 
                printf("Print 6\n");
           } 
        } 
    } 
 
}   

   