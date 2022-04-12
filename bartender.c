/*
 * bartender.c
 *
 *  Created on: Dec 26, 2015
 *      Author: dchiu
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include "globals.h"
#include "bartender.h"

/**
 * Code for bartender thread.
 * Do not touch.
 */
void* bartender(void* args)
{
	int i;
	for (i = 0; i<num_threads; i++)
	{
		waitForCustomer();
		makeDrink();
		receivePayment();
	}
	return NULL;
}

/**
 * Waits in a closet until a customer enters.
 */
void waitForCustomer()
{
	//TODO - synchronize
	sem_wait(custTravel);
	sem_wait(mutex);
	printf("\t\t\t\t\t\t\t\t\t\t\t| Bartender\n");
	sem_post(mutex);
	sem_post(btWait);
}


/**
 * When a customer places an order it takes the Bartender
 * a random amount of time between 5 ms and 1000 ms to make the drink.
 */
void makeDrink()
{
	//TODO - synchronize
	sem_wait(custOrders);
	sem_wait(mutex);
	int randomNum = (rand() % (1000 - 5 + 1)) + 5;
	usleep(randomNum);
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\tBartender\n");
	sem_post(mutex);
	sem_post(btAtReg);
	sem_post(btMakeDrink);
}


/**
 * Gets payment from the correct customer
 */
void receivePayment()
{
	//TODO - synchronize
	//at the register waiting for customer to pay
	sem_wait(btAtReg);
	sem_wait(mutex);
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\t\t\tBartender\n");
	sem_post(mutex);
	//now wait for the customer to give money
	sem_wait(custPays);
	sem_wait(mutex);
	//got the payment from the right customer!
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\t\t\t\t\tBartender\n");
	sem_post(mutex);
	sem_post(btPayed);
}
