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
	//wait for customers to travel
	sem_wait(custTravel);
	//mutex everything from once all the customers travel to when a customer leaves
	sem_wait(mutex);
	printf("\t\t\t\t\t\t\t\t\t\t\t| Bartender\n");
	//signal that the bartender is waiting
	sem_post(btWait);
}


/**
 * When a customer places an order it takes the Bartender
 * a random amount of time between 5 ms and 1000 ms to make the drink.
 */
void makeDrink()
{
	//TODO - synchronize
	//wait for the customer to order their drinks
	sem_wait(custOrders);
	int randomNum = (rand() % (1000 - 5 + 1)) + 5;
	usleep(randomNum);
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\tBartender\n");
	//signal for the bt to head to the register and that the drink is ready
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
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\t\t\tBartender\n");
	//now wait for the customer to give money
	sem_wait(custPays);
	//got the payment from the right customer!
	printf("\t\t\t\t\t\t\t\t\t\t\t| \t\t\t\t\t\tBartender\n");
	//signal that the bt got payed
	sem_post(btPayed);
}
