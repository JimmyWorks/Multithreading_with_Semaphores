//   Exploring Multiple Threads and Semaphores
//
//   Author: Jimmy Nguyen
//   Email:  Jimmy@JimmyWorks.net
//
//   Description:
//   Using multiple threads, simulate a hotel operation with each
//   individual operating as a separate thread.  Individuals include:
//
//   - 2 Front Desk Employees
//   - 2 Bellhops
//   - 25 Customers Checking In
//
//   Semaphores and Queues are used for mutual exclusion and ordered
//   execution of events.  The program starts up and begins creating
//   customers and employees.  As customers line up and when front
//   desk employees are available, they service the customers in the
//   order they lined up.  Keys to unique rooms are given to the 
//   customer served by that employee.  Customers also line up with
//   a random (0-5) number of bags.  Customers with two or more bags
//   will check their bags with the bellhop which will deliver it to
//   the rooms after the customer has entered their rooms.  Customers
//   that have checked their bags will also tip the bellhop before he
//   returns to the the bellhop desk.
//
//   Bellhop Routine

#include <stdio.h>
#include "program.h"

/*  Bellhop Routine  
 *  Handles the bellhop queue which helps guests with three or more
 *  bags delivering the bags to the room after the guest has gone to
 *  their room.  Bellhop shall wait for a tip before returning to the
 *  bellhop counter.
 */
void *bellhop(void *parg)
{
  int *arg = (int*)parg;
  printf("Bellhop %d created\n", *arg);

  // Private copy of customer serving
  int customerNo;

  // Bellhop works until the hotel closes for the day (program end)
  while(true)
  {
     // Wait someone to request help with bags
     sem_wait(&dropped_off_bags);

     // Mutual exclusion to dequeue the line
     sem_wait(&bellhop_queue_mutex);
        customerNo = bellhop_line.front(); // get customer
	bellhop_line.pop(); // dequeue
     sem_post(&bellhop_queue_mutex);

     // Deliver bags
     printf("Bellhop %d receives bags from guest %d\n", *arg, customerNo);
     employee[customerNo] = *arg; // write bellhop number into employee serving array
     sem_post(&customer_waiting[customerNo]); // take bags
     sem_wait(&bellhop_waiting[*arg]); // wait for customer to go to room
     printf("Bellhop %d delivers bags to guest %d\n", *arg, customerNo);
     sem_post(&customer_waiting[customerNo]); // deliver bags
     sem_wait(&bellhop_waiting[*arg]); // wait for a tip
  }
}

