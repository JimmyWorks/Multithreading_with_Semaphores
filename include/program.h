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
//   Header file 

#include <semaphore.h>
#include <queue>
using namespace std;

#ifndef _PROGRAM_1_H_
#define _PROGRAM_1_H_

// Defined customers, bellhops, and front desk employees
#define CUSTOMER_THREADS 25
#define BELLHOP_THREADS 2
#define FRONTDESK_THREADS 2
#define PSHARED 0 // semaphore shared between threads

// Semaphores for activities
extern sem_t get_vacant_room,
             dropped_off_bags, 
             frontdesk_queue_mutex, 
             bellhop_queue_mutex;

// Semapthores for thread-specific wait-signal
extern sem_t customer_waiting[CUSTOMER_THREADS],
             bellhop_waiting[BELLHOP_THREADS],
             frontdesk_waiting[FRONTDESK_THREADS];

// Queues for front desk and bellhop service
extern queue<int> frontdesk_line, bellhop_line;

// Global arrays for managing assigned rooms and employees serving guests
extern int customerRoom[CUSTOMER_THREADS], employee[CUSTOMER_THREADS];

// Simple unique room counter
extern int roomNumber;

// Thread routines
void *customer(void *arg);
void *bellhop(void *arg);
void *frontdesk(void *arg);

#endif
