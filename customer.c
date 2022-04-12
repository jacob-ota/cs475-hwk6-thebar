/*
 * customer.c
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
#include "customer.h"

/**
 * This is what the thread will call.
 * Do not touch.
 */
void* customer(void* args)
{
	unsigned int *custID = (unsigned int*) args;
	custTravelToBar(*custID);
	custArriveAtBar(*custID);
	custPlaceOrder();
	custBrowseArt();
	custAtRegister();
	custLeaveBar();
	return NULL;
}


/**
 * Each customer takes a random amount of time between 20 ms and 5000 ms to travel to the bar.
 */
void custTravelToBar(unsigned int custID)
{
	//TODO - synchronize
	//let the customers travel to the bar
	sem_wait(mutex);
	int randomNum = (rand() % (5000 - 20 + 1)) + 20;
	usleep(randomNum);
	printf("Cust %u\t\t\t\t\t\t\t\t\t\t\t|\n", custID);
	sem_post(mutex);
	//signal that the customers have gotten to the bar
	sem_post(custTravel);
}


/**
 * If there is already another customer in the bar the current customer has
 * to wait until bar is empty before entering.
 */
void custArriveAtBar(unsigned int custID)
{
	//TODO - synchronize
	//wait for the bartender to be in a waiting stage
	sem_wait(btWait);
	now_serving = custID;
	printf("\t\tCust %u\t\t\t\t\t\t\t\t\t|\n", custID);
	//singal that a customer has arrived
	sem_post(custArrive);
}


/**
 * The customer in the bar places an order
 */
void custPlaceOrder()
{
	//TODO - synchronize
	//wait for a customer to arrive
	sem_wait(custArrive);
	printf("\t\t\t\tCust %u\t\t\t\t\t\t\t|\n", now_serving);
	//signal for the customer to browse around after ordering
	sem_post(custBrowse);
}


/**
 * The customer in the bar can browse the wall art for a random amount of time between 3ms and 4000ms.
 */
void custBrowseArt()
{
	//TODO - synchronize
	//wait for a customer to browse until they order
	sem_wait(custBrowse);
	int randomNum = (rand() % (4000 - 3 + 1)) + 3;
	usleep(randomNum);
	printf("\t\t\t\t\t\tCust %u\t\t\t\t\t|\n", now_serving);
	//signal that a customer orders so the bt can start mixing the drinks
	sem_post(custOrders);
	//signal for the customer to go to the register
	sem_post(custAtReg);
}


/**
 * If their drink is not ready by the time they are done admiring the art they must wait
 * until the bartender has finished. When the bartender is finished, the customer pays.
 *
 */
void custAtRegister()
{
	//TODO - synchronize
	//wait for a customer to finish browsing
	sem_wait(custAtReg);
	//wait for a bt to finish making the drinks
	sem_wait(btMakeDrink);
	printf("\t\t\t\t\t\t\t\tCust %u\t\t\t|\n", now_serving);
	//signal that the customer has payed
	sem_post(custPays);
}


/**
 * The customer in the bar leaves the bar.
 */
void custLeaveBar()
{
	//TODO - synchronize
	//wait for the bt to get the right payment
	sem_wait(btPayed);
	printf("\t\t\t\t\t\t\t\t\t\tCust %u\t|\n", now_serving);
	//mutex is finished for this customer
	sem_post(mutex);
}
