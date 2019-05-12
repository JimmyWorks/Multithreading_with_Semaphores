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

#include <stdio.h>
#include <stdlib.h> // rand
#include "program.h"

/*  Customer Subroutine
 *  Customer threads will run the following routine and return to main
 *  when done to join with master thread.
 *
 *  void *parg	reference to thread number passed in
 *  return	value of thread number
 */
void *customer(void *parg)
{
  int *arg = (int*)parg;
  printf("Guest %d created\n", *arg);

  // Generate random number of bags guest brought (0-5 bags)
  int bag_count = rand() % 6;

  // Print number of bags
  if(bag_count == 0)
     printf("Guest %d enters the hotel with no bags\n", *arg);
  else if(bag_count == 1)
     printf("Guest %d enters the hotel with 1 bag\n", *arg);
  else  
     printf("Guest %d enters the hotel with %d bags\n", *arg, bag_count);

  // Get in frontdesk line to get a room
  sem_wait(&frontdesk_queue_mutex);
     printf("Guest %d gets in line to get a room number\n", *arg);
     frontdesk_line.push(*arg);
  sem_post(&frontdesk_queue_mutex);

  // Post that customer is waiting for a room
  sem_post(&get_vacant_room);
  sem_wait(&customer_waiting[*arg]); // Wait for response from frontdesk employee

  // Take key from frontdesk employee
  printf("Guest %d receives room key for room %d from front desk employee %d\n", *arg, customerRoom[*arg], employee[*arg]);
  sem_post(&frontdesk_waiting[employee[*arg]]);

  // If bag count greater than 2, drop off bags with bellhop
  if(bag_count > 2)
  {
     // Get in bellhop line to drop off baggage
     sem_wait(&bellhop_queue_mutex);
        printf("Guest %d requests help with bags\n", *arg);
        bellhop_line.push(*arg);
     sem_post(&bellhop_queue_mutex);

     // Post that customer is waiting to drop off bags
     sem_post(&dropped_off_bags);
     sem_wait(&customer_waiting[*arg]); // Wait for response from bellhop
  }

  // Enter room
  printf("Guest %d enters room %d\n", *arg, customerRoom[*arg]);

  // If dropped off bags with bellhop, wait for delivery
  if(bag_count > 2)
  {
     //signal bellhop that customer has gone to room
     sem_post(&bellhop_waiting[employee[*arg]]);     
     sem_wait(&customer_waiting[*arg]); // Wait for bellhop

     // Take bags from bellhop and give him a tip
     printf("Guest %d receives bags from bellhop %d and gives tip\n", *arg, employee[*arg]);
     sem_post(&bellhop_waiting[employee[*arg]]);
  }

  // Customer retires for the night
  printf("Guest %d retires for the evening\n", *arg);

  // Return to main routine to join thread with master thread
  return parg;
}
