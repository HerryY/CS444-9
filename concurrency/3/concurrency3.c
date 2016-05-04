/*
McKenna Jones
CS444 Oregon State University 2016

Concurrency Assignment 3
*/

/* REFERENCES
http://www.cprogramming.com/snippets/source-code/singly-linked-list-insert-remove-add-count
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

typedef struct Node{
        int data;
        struct Node *next;
}node_t;

node_t *list;

pthread_t searcher[2];
pthread_t inserter[2];
pthread_t deleter[2];


pthread_mutex_t delete_mut;
pthread_mutex_t insert_mut;

sem_t delete_ins_lock;
sem_t delete_search_lock;

/* Variables for Mersenne Twister */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

void printLinkedList(node_t *list);
void *search(void *arg);
void *insert(void *arg);
void *delete(void *arg);

void init_genrand(unsigned long s);
unsigned long genrand_int32(void);

int main()
{
        pthread_mutex_init(&insert_mut, NULL);
        pthread_mutex_init(&delete_mut, NULL);

        sem_init(&delete_ins_lock, 0, 1);
        sem_init(&delete_search_lock, 0, 1);

        for(int i = 0; i < 2; i++){
                pthread_create(&searcher[i], NULL, search, NULL);
                pthread_create(&inserter[i], NULL, insert, NULL);
                pthread_create(&deleter[i], NULL, delete, NULL);
        }

        for(int i = 0; i < 2; i++){
                pthread_join(inserter[i], NULL);
                pthread_join(searcher[i], NULL);
                pthread_join(deleter[i], NULL);
        }
}

/* Multiple searchers can search at once, therefore no locks */
void *search(void *arg)
{
        while(1){
                /* Wait for delete to finish here */
                sem_wait(&delete_search_lock);
                printf("Searcher is searching: ");
                printLinkedList(list);
                sleep(5);
        }
}

void *insert(void *arg)
{
        node_t *first;
        node_t *temp;

        first = list;
        int newVal;
        int sleepT;

        while(1){
                newVal = genrand_int32() % 10;

                /* wait for delete to finish here */
                sem_wait(&delete_ins_lock);
                /* Lock so only one inserter can be inserting at once */
                pthread_mutex_lock(&insert_mut);
                if(first == NULL){
                        first = (node_t *)malloc(sizeof(node_t));
                        first -> next =  NULL;
                        first -> data = newVal;
                        list = first;
                        printf("Inserted into head of list with value %d\n", newVal);
                }
                else{
                        temp = (node_t *)malloc(sizeof(node_t));
                        temp -> data = newVal;
                        temp -> next = NULL;

                        /* Find the last node */
                        while(first -> next != NULL){
                                first = first -> next;
                        }

                        first -> next = temp;
                        printf("Inserted into end of list with value %d\n", newVal);
                }
                sleep(5);
                pthread_mutex_unlock(&insert_mut);
        }
}

void *delete(void *arg)
{
        node_t *temp;
        node_t *prev;
        int delVal;

        while(1){
                temp = list;

                delVal = genrand_int32() % 10;
                /* Lock so only one deleter can delete at a time */
                pthread_mutex_lock(&delete_mut);
                while(temp != NULL){
                        if(temp -> data == delVal){
                                if(temp == list){
                                        list = temp -> next;
                                        free(temp);
                                        printf("Deleted head with val %d\n", delVal);
                                        break;
                                }
                                else{
                                        prev->next = temp->next;
                                        free(temp);
                                        printf("Deleted node with val %d\n", delVal);
                                        break;
                                }
                        }
                        else{
                                prev = temp;
                                temp = temp->next;
                        }
                        break;
                }
                pthread_mutex_unlock(&delete_mut);
                /* Wake up searcher and inserter if they have been waiting */
                sem_post(&delete_ins_lock);
                sem_post(&delete_search_lock);
                sleep(5);

        }


}

void printLinkedList(node_t *list)
{
        if(list == NULL){
                printf("List currently empty\n");
        }
        else{
                while(list != NULL){
                        printf("%d ", list->data);
                        list = list -> next;
                }
                printf("\n");
        }
}

/* initializes mt[N] with a seed */
void init_genrand(unsigned long s)
{
    mt[0]= s & 0xffffffffUL;
    for (mti=1; mti<N; mti++) {
        mt[mti] =
	    (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti);
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt[mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long genrand_int32(void)
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N+1)   /* if init_genrand() has not been called, */
            init_genrand(5489UL); /* a default initial seed is used */

        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }

    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}
