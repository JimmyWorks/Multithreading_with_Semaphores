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
//   Front Desk Routine
//
#include <stdio.h>
#include "program.h"

/*  Front Desk Routine
 *  Handles the front desk queue which assigns each guest a unique
 *  room and hands keys to guest.
 *
 *  void *parg	reference to the front desk employee number
 */
void *frontdesk(void *parg)
{
  int *arg = (int*)parg;
  printf("Front desk employee %d created\n", *arg);

  // Private copy of the customer being served and room number assigned
  int servingCustomerNo, vacantRoom;

  // Front desk employee works till hotel closes (program ends) for the day
  while(true)
  {
     // Wait till someone is in line
     sem_wait(&get_vacant_room);

     // Use mutual exlusion to dequeue the front desk line
     sem_wait(&frontdesk_queue_mutex);
        servingCustomerNo = frontdesk_line.front(); //get next guest
	frontdesk_line.pop(); // dequeue
	vacantRoom = ++roomNumber; // get next free room
     sem_post(&frontdesk_queue_mutex);

     // Assign the room to the guest
     printf("Front desk employee %d registers guest %d and assigns room %d\n", *arg, servingCustomerNo, vacantRoom);
     customerRoom[servingCustomerNo] = vacantRoom; //assigned room
     employee[servingCustomerNo] = *arg; //put employee number serving guest
     sem_post(&customer_waiting[servingCustomerNo]); //notify customer
     sem_wait(&frontdesk_waiting[*arg]); //wait for customer to take keys before serving next
  }
}

