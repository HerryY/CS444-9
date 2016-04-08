#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#define MAX 32
struct Data {
  int number;
  int wait;
};
int currentIndex;

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
  while(1){
    pthread_mutex_lock(&mut);

    // Random sleep time between 3 and 7
    int sleepT = rand() % 5 + 3;

    //Check and wait if buffer is full
    while(currentIndex == MAX){
      pthread_cond_wait(&conditionProduce, &mut);
    }

    // Generate random values to add
    temp.number = rand() % 100;
    temp.wait = rand() % 8 + 2;
    buff[currentIndex] = temp;
    sleep(sleepT);
    printf("Added %i with wait time %i, at index %i. Slept for %i \n", temp.number, temp.wait, currentIndex, sleepT);
    currentIndex++;
    if(currentIndex >= MAX)
      currentIndex = 0;

    pthread_cond_signal(&conditionConsume);
    pthread_mutex_unlock(&mut);
  }
}

void *consumer(void *arg){
  int sleepT;
  struct Data temp;
  while(1){
    pthread_mutex_lock(&mut);

    //Check and wait if nothing to consume
    while(currentIndex == 0){
      pthread_cond_wait(&conditionConsume-1, &mut);
    }
    sleepT = buff[currentIndex].wait;
    sleep(sleepT);

    temp.number = buff[currentIndex-1].number;
    temp.wait = sleepT;
    printf("Got %i with wait time %i, at index %i. Slept for %i \n", temp.number, temp.wait, currentIndex-1, sleepT);
    currentIndex--;
    pthread_cond_signal(&conditionProduce);
    pthread_mutex_unlock(&mut);
  }
}

int main(){

  signal(SIGINT, interruptHandler);
  memset(buff, 0, sizeof(buff));

  printf("Initializing variables\n");
  pthread_cond_init(&conditionProduce, NULL);
  pthread_cond_init(&conditionConsume, NULL);
  pthread_mutex_init(&mut, NULL);

  printf("Creating threads\n");
  pthread_create(&consume, NULL, consumer, NULL);
  pthread_create(&produce, NULL, producer, NULL);


  pthread_join(produce, NULL);
  pthread_join(consume, NULL);

  return EXIT_SUCCESS;
}


