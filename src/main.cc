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
//   Main Routine

#include <pthread.h>
#include <stdio.h>
#include <cstdlib>
#include <semaphore.h>
#include <queue>
#include "program.h"
using namespace std;

// Event and Mutual Exculsion Semaphores
sem_t get_vacant_room, // Counter of guests waiting to get a room
      dropped_off_bags, // Counter of guests waiting to drop off luggage
      frontdesk_queue_mutex, // Mutual Exclusion to get into and out of queue
      bellhop_queue_mutex; // Mutual Exlcusion to get into and out of queue

// Thread-specific Semaphores for One-on-One Wait-Signal
sem_t customer_waiting[CUSTOMER_THREADS],
      bellhop_waiting[BELLHOP_THREADS],
      frontdesk_waiting[FRONTDESK_THREADS];

// Lines for the front desk and bellhop service
queue<int> frontdesk_line, bellhop_line;

// Arrays for room assignment and current employee helping customer
int customerRoom[CUSTOMER_THREADS], employee[CUSTOMER_THREADS];

// Simple counter to show rooms assigned are unique
int roomNumber;

/*  Main Routine
 *  Initializes all semaphores.
 *  Creates all threads runs them for the respective simulated individual.
 *  Joins customer threads as they complete.
 */
int main(int argc, char* argv[])
{
   int i; // counter for for-loops
   roomNumber = 0; // initialized room number

 //Create thread arrays and id arrays
   pthread_t customerThread[CUSTOMER_THREADS];
   int customerID[CUSTOMER_THREADS];

   pthread_t bellhopThread[BELLHOP_THREADS];
   int bellhopID[BELLHOP_THREADS];

   pthread_t deskThread[FRONTDESK_THREADS];
   int deskID[FRONTDESK_THREADS];

 //Initialize all semaphores
   // Front desk queue mutex, only 1 can access at a time
   if(sem_init(&frontdesk_queue_mutex, PSHARED, 1) == -1)
   {
      printf("Failed to initialize frontdesk_queue_mutex");
      exit(1);
   }

   // Bellhop queue mutex, only 1 can access at a time
   if(sem_init(&bellhop_queue_mutex, PSHARED, 1) == -1)
   {
      printf("Failed to initialize bellhop_queue_mutex");
      exit(1);
   }

   // Semaphore for number of guests waiting in front desk line
   if(sem_init(&get_vacant_room, PSHARED, 0) == -1)
   {
      printf("Failed to initialize semaphore for getting vacant room");
      exit(1);
   }
   
   // Semaphore for number of guests waiting for bellhop service
   if(sem_init(&dropped_off_bags, PSHARED, 0) == -1)
   {
      printf("failed to initialize semaphore for dropping off bags");
      exit(1);
   }

 // Thread-specific semaphores

   // Customer thread semaphores
   for(i = 0; i < CUSTOMER_THREADS; i++)
   {
      if(sem_init(&customer_waiting[i], PSHARED, 0) == -1)
      {
         printf("failed to initialize semaphore for customer_waiting");
         exit(1);
      }
   }

   // Bellhop thread semaphores
   for(i = 0; i < BELLHOP_THREADS; i++)
   {
      if(sem_init(&bellhop_waiting[i], PSHARED, 0) == -1)
      {
         printf("failed to initialize semaphore for bellhop waiting");
         exit(1);
      }
   }

   // Front desk thread semaphores
   for(i = 0; i < FRONTDESK_THREADS; i++)
   {
      if(sem_init(&frontdesk_waiting[i], PSHARED, 0) == -1)
      {
         printf("failed to initialize semaphore for front desk waiting");
         exit(1);
      }
   }

  // Fork and execute all threads

   // Bellhop threads
   try{
      for(i = 0; i < BELLHOP_THREADS; i++)
      {
         bellhopID[i] = i;

         int returnCode = pthread_create(&bellhopThread[i], NULL, bellhop, &bellhopID[i]);

         if(returnCode) throw returnCode;
      }
   }catch(int returnCode){
      printf("Error code %d encountered when making thread", returnCode);
   }

   // Front desk threads
   try{
      for(i = 0; i < FRONTDESK_THREADS; i++)
      {
         deskID[i] = i;

         int returnCode = pthread_create(&deskThread[i], NULL, frontdesk, &deskID[i]);

         if(returnCode) throw returnCode;
      }
   }catch(int returnCode){
      printf("Error code %d encountered when making thread: ", returnCode);
   }

   // Customer threads
   try{
      for(i = 0; i < CUSTOMER_THREADS; i++)
      {
         customerID[i] = i;

         int returnCode = pthread_create(&customerThread[i], NULL, customer, &customerID[i]);

         if(returnCode) throw returnCode;
      }
   }catch(int returnCode){
      printf("Error code %d encountered when making thread: ", returnCode);
   }

 // Join customer threads as they finish
   try{
      for(i = 0; i < CUSTOMER_THREADS; i++)
      {
         printf("Guest %d joined\n", i);
         int returnCode = pthread_join(customerThread[i], NULL);

         if(returnCode) throw returnCode;
      }
   }catch(int returnCode){
      printf("Error encountered when joining customer thread: %d\n", returnCode);
   }

   // End simulation once last customer thread is joined
   printf("Simulation ends\n");

   return 0;
}


