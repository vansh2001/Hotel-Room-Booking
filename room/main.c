/* ECE 209 - Fall 2017
 * User interface for room reservation system
 * Creates and initializes schedules for requested
 * number of rooms.  Implements the following commands:
 *
 * r = create reservation
 * x = cancel reservation
 * s = print complete room schedule
 * f = print all reservations made for a given name
 * q = quit
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "schedule.h"
#include "rooms.h"

/* weird macro stuff to limit the input size for names */
#define stringify(S) #S
#define STRFMT(X) "%" stringify(X) "s"
#define STRNAME  STRFMT(NAME_LENGTH)

int main() 
{
    
    /* open and close will change when we test your .c files! */
    Time open = {7, 0};   /* 7am */
    Time close = {21, 0}; /* 9pm */
    /* open and close will change when we test your .c files! */
    int nrooms = 0;
    char command[2];  /* one-letter command */
    char name[NAME_LENGTH+1]; /* user input for name */

    /* temporary variables */
      Time start, end;
      int status, r;
      struct rNode *res_list = NULL;


     printf("How many rooms? ");
     fflush(stdout); 
     //make sure prompt is printed before scanf 
     scanf("%d", &nrooms);
     setupRooms(nrooms, open, close);

    while (1) {
        printf("\nEnter command (r,x,s,f,q): ");
        fflush(stdout);
        scanf("%1s", command);
        // why use string for a one-letter command?
         //so we don't have to worry about leading spaces */

        if (strcmp(command,"q") == 0) break;

        else if (strcmp(command, "s") == 0) {
            printf("=== CURRENT SCHEDULE ===\n");
            printRoomSchedules(stdout);
        }

        else if (strcmp(command, "r") == 0) {
            printf("=== MAKE RESERVATION ===\n");
            printf("Enter name (one word): ");
            fflush(stdout);
            scanf(STRNAME, name);
            printf("Start time (hh:mm, 24-hr): ");
            fflush(stdout);
            scanf("%d:%d", &start.hr, &start.min);
            printf("End time (hh:mm, 24-hr): ");
            fflush(stdout);
            scanf("%d:%d", &end.hr, &end.min);

            status = makeReservation(name, start, end);
            if (status) {
                printf("Success!  Room number: %d\n", status);
            }
            else {
                printf("Unable to make reservation.\n");
            }
        }

        else if (strcmp(command, "x") == 0) {
            printf("=== CANCEL RESERVATION ===\n");
            printf("Enter room number: ");
            fflush(stdout);
            scanf("%d", &r);
            printf("Enter name (one word): ");
            fflush(stdout);
            scanf(STRNAME, name);
            printf("Start time (hh:mm, 24-hr): ");
            fflush(stdout);
            scanf("%d:%d", &start.hr, &start.min);

            status = cancelReservation(r, name, start);
            if (status) {
                printf("Cancelled.\n");
            }
            else {
                printf("Unable to cancel.\n");
            }
        }

        else if (strcmp(command, "f") == 0) {
            printf("=== FIND RESERVATIONS ===\n");
            printf("Enter name (one word): ");
            fflush(stdout);
            scanf(STRNAME, name);
            res_list = findReservations(name);
            if (res_list) {
                printReservations(res_list, stdout);
            }
            else {
                printf("No reservations found for %s\n", name);
            }
        }
    }

    
    
  /*
  Time open = {7, 0}; 
  Time close = {23, 0};
  
  Time t1= {7,20};
  Time t2= {7,45};
  Time t3= {12,0};
  Time t4= {15,0};
  Time t5= {16,45};
  Time t6= {17,30};
  Time t7= {20,45};
    
  
  Schedule test= (Schedule) malloc(sizeof(struct schedule));
  test->start = open;
  test->end=close;
  test->idle=(struct iNode*) malloc(sizeof(struct iNode));
  test->idle->interval.start = open;
  test->idle->interval.end=t1;


  struct iNode* idle1=(struct iNode*) malloc(sizeof(struct iNode));
  test->idle->next=idle1;
  idle1->interval.start=t2;
  idle1->interval.end=t3;

  struct iNode* idle2=(struct iNode*) malloc(sizeof(struct iNode));
  idle1->next=idle2;
  idle2->interval.start=t3;
  idle2->interval.end=t4;

  struct iNode* idle3=(struct iNode*) malloc(sizeof(struct iNode));
  idle2->next=idle3;
  idle3->interval.start=t5;
  idle3->interval.end=t6;
  

  test->busy=(struct iNode*) malloc(sizeof(struct iData));
  test->busy->interval.start=t1;
  test->busy->interval.end=t2;
  strcpy(test->busy->interval.owner,"Bryant");

  struct iNode* busy1=(struct iNode*) malloc(sizeof(struct iNode));
  test->busy->next=busy1;
  busy1->interval.start=t4;
  busy1->interval.end=t5;
  strcpy(busy1->interval.owner,"Felix");

  struct iNode* busy2=(struct iNode*) malloc(sizeof(struct iNode));
  busy1->next=busy2;
  busy2->interval.start=t6;
  busy2->interval.end=t7;
  strcpy(busy2->interval.owner,"ANDY");

  struct iNode* busy3=(struct iNode*) malloc(sizeof(struct iNode));
  busy2->next=busy3;
  busy3->interval.start=t7;
  busy3->interval.end=close;
  strcpy(busy3->interval.owner,"HAT");
  busy3->next=NULL;
  

printSchedule(test, stdout);
printf("\n");
printf("yolo");
Time start = {15, 0};
Time end = {16, 45};
int y;
int x;
*/ 
//printf("yolo");
//y = isBusy(test, start, end);
//printf("%d", y); 


//x = reserve(test, "atom", start, end);
//printf("reserved return value: %d\n", x);
//reserve(test, "yoyo", start,  end);
 /*       
//printSchedule(test, stdout);
cancel(test, "Felix", start);

printf("final\n");
printSchedule(test, stdout);
*/
  /*    
  Schedule test; 
  test = createSchedule(open,close);
  printf("Works21\n");
  
  printSchedule(test, stdout);
  return EXIT_SUCCESS;
 */
  
}


