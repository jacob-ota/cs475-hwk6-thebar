/*
 * globals.h
 *
 *  Created on: Dec 26, 2015
 *      Author: dchiu
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_

int num_threads;	// number of customer threads
int now_serving;	// customer's ID who is being served

//TODO - declare some semaphores
//critical section lock
sem_t* mutex;
//customers semaphores
sem_t* custTravel;
sem_t* custArrive;
sem_t* custOrders;
sem_t* custBrowse;
sem_t* custPays;
sem_t* custLeaves;
//bartender semaphores
sem_t* btWait;
sem_t* btMakeDrink;
sem_t* btPayed;

#endif /* GLOBALS_H_ */
