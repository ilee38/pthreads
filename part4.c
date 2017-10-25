/*
 * part4.c
 *
 *  Created on: Oct 25, 2017
 *      Author: iramlee
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define MAX_NUM_THREADS 50	//define the max number of allowed threads

int nthreads;
pthread_mutex_t chopstick[MAX_NUM_THREADS];
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
}


/*
 * Function: pickUpChopsticks()
 * */
void pickUpChopsticks(int threadIndex){
	int right;
	int left = threadIndex;
	if(threadIndex == 0){	//handle case where threadIndex = 0
		right = nthreads - 1;
	}else{
		right = threadIndex - 1;
	}
	pthread_mutex_trylock(&chopstick[left]);		//use pthread_mutex_trylock() to prevent blocking
	pthread_mutex_trylock(&chopstick[right]);
}


/*
 * Function: putDownChopsticks()
 * */
void putDownChopsticks(int threadIndex){
	int right;
	int left = threadIndex;
	if(threadIndex == 0){		//handle case where threadIndex = 0
		right = nthreads - 1;
	}else{
		right = threadIndex - 1;
	}
	pthread_mutex_unlock(&chopstick[left]);
	pthread_mutex_unlock(&chopstick[right]);
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
	for(int i = 0; i < nthreads; i++){
		pthread_mutex_init(&chopstick[i], NULL);	//create mutex for each chopstick
		//printf("Mutex %d id = %d\n", i, chopstick[i]);
	}
	createPhilosophers(nthreads);
	fclose(fp);		//close output file.
	/* Destroy mutexes after program completes */
	for(int j = 0; j < nthreads; j++){
		pthread_mutex_destroy(&chopstick[j]);
	}
}


