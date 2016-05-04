/*
Dining Philosophers Problem
McKenna Jones
CS444 -- Oregon State University
*/

/*
References
http://adit.io/posts/2013-05-11-The-Dining-Philosophers-Problem-With-Ron-Swanson.html
http://pseudomuto.com/development/2014/03/01/dining-philosophers-in-c/
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

#define PHILOSOPERS 5

/* Variables for Mersenne Twister */
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

typedef struct {
        const char *name;
        int table_pos;
        sem_t *fork;
        sem_t *lock;
        int eat_count;
} values;

void *philosopher();
void get_forks(values *temporary);
void eat(const char *name, int pos);
void put_forks(values *temporary);
void think(const char *name, int pos);
void interrupt_handler(int signal);
void print_forks(sem_t frks[PHILOSOPERS]);
void init_genrand(unsigned long s);
unsigned long genrand_int32(void);

pthread_t phils[PHILOSOPERS];

int main()
{
        sem_t forks[PHILOSOPERS];
        sem_t lock;

        signal(SIGINT, interrupt_handler);

        const char *ph[5];
        ph[0] = "Aristotle";
        ph[1] = "Plato";
        ph[2] = "Socrates";
        ph[3] = "Confucius";
        ph[4] = "Epicurus";

        /* Initialize Semaphores here */
        for(int i = 0; i < PHILOSOPERS; i++){
                sem_init(&forks[i], 0, 1);
        }
        /*
        Initialize table lock to value 4, so only 4 of the
        philosophers can have a fork at one time
        */
        sem_init(&lock, 0, 4);

        /*
        Creation of five philosopher threads
        Each philosopher gets assigned a name and table position here.
        Each philosopher also gets passed the forks array and the table lock
        */
        for(int i = 0; i < PHILOSOPERS; i++){
                values *passing = malloc(sizeof(values));
                passing->name = ph[i];
                passing->table_pos = i;
                passing->fork = forks;
                passing->lock = &lock;
                passing->eat_count = 0;
                printf("creating phil\n");

                pthread_create(&phils[i], NULL, philosopher, (void *)passing);
        }

        for(int i = 0; i < PHILOSOPERS; i++){
                pthread_join(phils[i], NULL);
        }

        return EXIT_SUCCESS;
}

void interrupt_handler(int signal)
{
	if(signal == SIGINT)
	{
                /* Detatch both threads */
		for(int i = 0; i < PHILOSOPERS; i++){
                        pthread_detach(phils[i]);
                }
		exit(EXIT_SUCCESS);
	}
}

void think(const char *name, int pos)
{
        srand(time(NULL));
        int think_time;

        /* Random think time between 1 and 20 using Mersenne Twister*/
        think_time = genrand_int32() % 20 + 1;
        printf("(%d) %s is thinking \n", pos, name);
        sleep(think_time);
}

void eat(const char *name, int pos)
{
        srand(time(NULL));
        int eat_time;

        /* Random eat time between 2 and 9 using Mersenne Twister*/
        eat_time = genrand_int32() % 7 + 2;
        printf("(%d) %s is eating \n", pos, name);
}

void print_forks(sem_t frks[PHILOSOPERS])
{
        int sval;
        /* Prints out the state of each fork */
        for(int i = 0; i < PHILOSOPERS; i++){
                sem_getvalue(&frks[i], &sval);
                if(sval == 1){
                        printf("Fork %i: In use\n", i);
                }
                else{
                        printf("Fork %i: Not being used\n", i);
                }
        }
}

void get_forks(values *temp)
{
        /* Wait on the table lock, and the fork to the right and left */
        sem_wait(temp->lock);
        sem_wait(&temp->fork[temp->table_pos]);
        sem_wait(&temp->fork[(temp->table_pos + 1) % PHILOSOPERS]);
}

void put_forks(values *temp)
{
        /* Put down each fork, and release table lock */
        sem_post(&temp->fork[temp->table_pos]);
        sem_post(&temp->fork[(temp->table_pos + 1) % PHILOSOPERS]);
        sem_post(temp->lock);
}

void *philosopher(void *arg)
{
        values temp = *(values *)arg;

        while(1){
                think(temp.name, temp.table_pos);
                get_forks(&temp);
                eat(temp.name, temp.table_pos);
                /* Eat counter to prove that no philosopher is starving */
                temp.eat_count++;
                printf("%s has eaten %d times\n", temp.name, temp.eat_count);
                put_forks(&temp);
                print_forks(temp.fork);
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
