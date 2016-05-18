/*
McKenna Jones
Oregon State University Spring 2016
CS444 -- Operating Systems Two
*/

/*
REFERENCES:
https://lab.cs.ru.nl/algemeen/images/8/8f/Opgavenserie9.pdf
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <signal.h>

#define NUM_THREADS 5
sem_t mutex;
sem_t blocker;

int cur_active;
int cur_waiting;
int wait = 0;

pthread_t threads[5];

void* threadFn(void *arg);

int main(){
        sem_init(&mutex, 0, 1);
        sem_init(&blocker, 0, 0);

        for(int i = 0; i < NUM_THREADS; i++){
                printf("Creating threads\n");
                pthread_create(&threads[i], NULL, threadFn, NULL);
        }

        for(int i = 0; i < NUM_THREADS; i++){
                pthread_join(threads[i], NULL);
        }
        return 0;
}

void* threadFn(void *arg){
        while(1){
                sem_wait(&mutex); /* Enter critical section */
                if(wait){         /* If there are three, then wait for them to leave */
                        cur_waiting++;
                        sem_post(&mutex);
                        printf("Waiting for all users to leave\n");
                        sem_wait(&blocker);
                        cur_waiting--;
                }
                cur_active++;
                printf("Current active: %d\n", cur_active);
                if(cur_active == 3){  /* Check for three active users */
                        sleep(5);     /* Sleep here to see when this happens */
                        wait = 1;
                }
                else{
                        wait = 0;
                }

                /* If there are threads waiting, but not 3 yet, unblock one */
                if(cur_waiting > 0 && wait == 0){
                        printf("Unblocking a thread\n");
                        sem_post(&blocker);
                }
                else{
                        printf("Opening mutual exclusion for a thread\n");
                        sem_post(&mutex);
                }

                /* Critical section again */
                sem_wait(&mutex);
                printf("Entered mutal exclusion\n");
                cur_active--;
                if(cur_active == 0){
                        printf("No others active\n");
                        wait = 0;
                }
                /* If there are threads waiting, but not 3 yet, unblock one */
                if(cur_waiting != 0 && wait == 0){
                        printf("Unblocking a thread\n");
                        sem_post(&blocker);
                }
                else{
                        printf("Opening mut exclusion\n");
                        sem_post(&mutex);
                }
        }
}
