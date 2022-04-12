/*
 * TheBar.c
 *
 *  Created on: Dec 26, 2015
 *      Author: dchiu
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include "globals.h"
#include "customer.h"
#include "bartender.h"

void printBanner();
void init();
void cleanup();
typedef struct thread_args {
	int customer;
}thread_args;

/**
 * Main function
 */
int main(int argc, char **argv)
{
	//num threads based on how many people are entered
	if(atoi(argv[1]) > 0) {
		num_threads = atoi(argv[1]);
		printBanner();
		init(); // initialize semaphores

		pthread_t bttid;
		pthread_t tid[num_threads];
		thread_args args[num_threads];

		// TODO - fire off customer thread
		for(int i = 0; i < num_threads; i++) {
			args[i].customer = i;
			pthread_create(&tid[i], NULL, customer, &args[i]);
		}

		// TODO - fire off bartender thread
		pthread_create(&bttid, NULL, bartender, NULL);

		// TODO - wait for all threads to finish
		for (int i = 0; i < num_threads; i++) {
        	pthread_join(tid[i], NULL);
    	}
		pthread_join(bttid, NULL);

		cleanup(); // cleanup and destroy semaphores
	}
	else {
		printf("Cannot have a negative amount of customers\n");
	}
	return 0;
}

/**
 * Prints the activity banner.
 * Do not touch.
 */
void printBanner()
{
	printf("Customer:\t\t\t\t\t\t\t\t\t\t| Employee:\n");
	printf("Traveling\tArrived\t\tOrdering\tBrowsing\tAt Register\tLeaving");
	printf("\t| Waiting\tMixing Drinks\tAt Register\tPayment Recv\n");
	printf("----------------------------------------------------------------------------------------+");
	printf("-----------------------------------------------------------\n");
}

/**
 * Create and initialize semaphores
 */
void init()
{
	// TODO - unlink semaphores
	//cs lock
	sem_unlink("/mutex");
	//customer semaphores
	sem_unlink("/custTravel");
	sem_unlink("/custArrive");
	sem_unlink("/custOrders");
	sem_unlink("/custBrowse");
	sem_unlink("/custAtReg");
	sem_unlink("/custPays");
	//bartender semaphores
	sem_unlink("/btWait");
	sem_unlink("/btMakeDrink");
	sem_unlink("/btAtReg");
	sem_unlink("/btPayed");

	// TODO - create semaphores
	//cs lock
	mutex = sem_open("/mutex", O_CREAT, 0600, 1);
	//customer semaphores
	custTravel = sem_open("/custTravel",  O_CREAT, 0600, 0);
	custArrive = sem_open("/custArrive",  O_CREAT, 0600, 0);
	custOrders = sem_open("/custOrders",  O_CREAT, 0600, 0);
	custBrowse = sem_open("/custBrowse",  O_CREAT, 0600, 0);
	custAtReg = sem_open("/custAtReg",  O_CREAT, 0600, 0);
	custPays = sem_open("/custPays",  O_CREAT, 0600, 0);
	//bartender semaphores
	btWait = sem_open("/btWait",  O_CREAT, 0600, 0);
	btMakeDrink = sem_open("/btMakeDrink",  O_CREAT, 0600, 0);
	btAtReg = sem_open("/btAtReg",  O_CREAT, 0600, 0);
	btPayed = sem_open("/btPayed",  O_CREAT, 0600, 0);
}

/**
 * Cleanup and destroy semaphores
 */
void cleanup()
{
	// TODO - close semaphores
	//cs lock
	sem_close(mutex);
	//customer semaphores
	sem_close(custTravel);
	sem_close(custArrive);
	sem_close(custOrders);
	sem_close(custBrowse);
	sem_close(custAtReg);
	sem_close(custPays);
	//bartender semaphores
	sem_close(btWait);
	sem_close(btMakeDrink);
	sem_close(btAtReg);
	sem_close(btPayed);

	//unlink them also
	//cs lock
	sem_unlink("/mutex");
	//customer semaphores
	sem_unlink("/custTravel");
	sem_unlink("/custArrive");
	sem_unlink("/custOrders");
	sem_unlink("/custBrowse");
	sem_unlink("/custAtReg");
	sem_unlink("/custPays");
	//bartender semaphores
	sem_unlink("/btWait");
	sem_unlink("/btMakeDrink");
	sem_unlink("/btAtReg");
	sem_unlink("/btPayed");
}