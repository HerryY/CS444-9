#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "mt19937ar.c"

#define MAX 32
struct Data {
        int number;
        int sleepTime;
};
int currentIndex = 0;

struct Data buff[MAX];
pthread_mutex_t mut;
pthread_cond_t conditionConsume;
pthread_cond_t conditionProduce;
pthread_t produce;
pthread_t consume;

void interruptHandler(int signal)
{
	if(signal == SIGINT)
	{
                // Detatch both threads
		pthread_detach(produce);
                pthread_detach(consume);
		exit(EXIT_SUCCESS);
	}
}

void *producer(void *arg){
        struct Data temp;
        int index = 0;
        // Generate random sleep time for producer
        int sleepT = genrand_int32() % 5 + 3;
        while(1){
                sleep(sleepT);
                // Protect buffer
                pthread_mutex_lock(&mut);
                // Check if the buffer is full
                while (currentIndex == MAX){
                        printf("Wainting for a consume\n");
                        pthread_cond_wait(&conditionProduce, &mut);
                }
                // Generate random values to produce
                temp.number = genrand_int32() % 10;
                temp.sleepTime = genrand_int32() % 8 + 2;
                // Add to buffer
                buff[currentIndex] = temp;
                printf("Producer: Produced item %d, with number %d, and sleep time %d\n", currentIndex, temp.number, temp.sleepTime);
                currentIndex++;
                // Wake up consumer
                pthread_cond_signal(&conditionConsume);
                pthread_mutex_unlock(&mut);
        }
}

void *consumer(void *arg)
{
        int index = 0;
        while(1){
                // Protect Buffer
                pthread_mutex_lock(&mut);
                // Check to see if there's anything in the buffer
                // If not, wait
                while (currentIndex == 0){
                        printf("waiting for produce\n");
                        pthread_cond_wait(&conditionConsume, &mut);
                }
                pthread_mutex_unlock(&mut);

                // 'Consume' item
                sleep(buff[currentIndex-1].sleepTime);

                pthread_mutex_lock(&mut);
                printf("Consumer: Consumed item: %d, with value: %d\n", currentIndex-1, buff[currentIndex-1].number);
                currentIndex--;
                pthread_cond_signal(&conditionProduce);
                pthread_mutex_unlock(&mut);
        }
}

int main()
{
        // Initalize twister
        init_genrand(time(NULL));
        signal(SIGINT, interruptHandler);
        memset(buff, 0, sizeof(buff));

        // Initializing Variables
        pthread_cond_init(&conditionProduce, NULL);
        pthread_cond_init(&conditionConsume, NULL);
        pthread_mutex_init(&mut, NULL);

        // Creating threads
        pthread_create(&consume, NULL, consumer, NULL);
        pthread_create(&produce, NULL, producer, NULL);

        // Joining threads
        pthread_join(produce, NULL);
        pthread_join(consume, NULL);

        return 0;
}
