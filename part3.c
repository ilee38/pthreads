/*
 * part3.c
 *
 *  Created on: Oct 23, 2017
 *      Author: iramlee
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX_NUM_THREADS 50	//define the max number of allowed threads

int nthreads;
int philosopherIdx = 0;		//Index for eating order
pthread_mutex_t chopstick_mutex;
pthread_cond_t eating_turn;
FILE *fp = fopen("status.txt", "a");	//create output file


/*
 * Function: thinking()
 * */
void thinking(){
	int rnd = (random() % 499) + 1;		//Generate random number from 1 to 500
	usleep(rnd);

}


/*
 * Function: eating()
 * */
void eating(int threadIndex){
	int rnd = (random() % 499) + 1;		//Generate random number from 1 to 500
	fprintf(fp, "Philosopher %d is eating\n", threadIndex);
	usleep(rnd);
	if(philosopherIdx == (nthreads - 1)){	//Increment philosopherIdx after eating. Reset if it is the last philosopher
		philosopherIdx = 0;
	}else{
		philosopherIdx += 1;
	}
	/* Check index for eating turn and broadcast signal to waiting threads (Philosophers) */
	if(philosopherIdx == (threadIndex + 1)){
		pthread_cond_broadcast(&eating_turn);
	}
}


/*
 * Function: pickUpChopsticks()
 * */
void pickUpChopsticks(int threadIndex){
	pthread_mutex_lock(&chopstick_mutex);		//lock mutex
	/* wait for eating turn (i.e. philosohperIdx == threadIndex) */
	while(philosopherIdx != threadIndex){
		pthread_cond_wait(&eating_turn, &chopstick_mutex);
	}
}


/*
 * Function: putDownChopsticks()
 * */
void putDownChopsticks(int threadIndex){
	pthread_mutex_unlock(&chopstick_mutex);
}


/*
 * Function: PhilosopherThread()
 * Executed by each thread
 * */
void *PhilosopherThread(void *threadID){
	long tid;
	tid = (long)threadID;
	thinking();
	pickUpChopsticks(tid);
	eating(tid);
	putDownChopsticks(tid);
	pthread_exit(NULL);
}


/*
 * Function: createPhilosophers()
 * Creates nthreads treads and assigns it to a "philosopher"
 * */
void createPhilosophers(int nthreads){
	pthread_t threads[nthreads];
	int rc;
	long i;
	void *status;
	for(i = 0; i < nthreads; i++){
		rc = pthread_create(&threads[i], NULL, PhilosopherThread, (void *)i);
		if(rc){
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}
	for(i = 0; i < nthreads; i++){
		rc = pthread_join(threads[i], &status);
		if(rc){
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
	}
	pthread_exit(NULL);
}


/* Main function
 * Takes the number of threads to be created as a command line argument
 * */
int main(int argc, char ** argv ){
	nthreads = atoi(argv[1]);
	srandom(time(NULL));		//seed the random generator only once
	/* Initialize mutex and cond variable*/
	pthread_mutex_init(&chopstick_mutex, NULL);
	pthread_cond_init(&eating_turn, NULL);
	createPhilosophers(nthreads);
	fclose(fp);		//close output file.
	/* Destroy mutex and cond variable after program completes */
	pthread_mutex_destroy(&chopstick_mutex);
	pthread_cond_destroy(&eating_turn);
}
