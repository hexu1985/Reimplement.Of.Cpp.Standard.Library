#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore>

using std::counting_semaphore;
using std::binary_semaphore;

#define	NBUFF	 10

int		nitems;					/* read-only by producer and consumer */
struct {	/* data shared by producer and consumer */
    int	buff[NBUFF];
    binary_semaphore *mutex;
    counting_semaphore<> *nempty, *nstored;
} shared;

void produce();
void consume();

int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("usage: prodcons2 <#items>\n");
        exit(1);
    }
    nitems = atoi(argv[1]);

    /* 4initialize three semaphores */
    shared.mutex = new binary_semaphore{1};
    shared.nempty = new counting_semaphore<>{NBUFF};
    shared.nstored = new counting_semaphore<>{0};

    std::thread thr_produce(produce);
    std::thread thr_consume(consume);

    thr_produce.join();
    thr_consume.join();

    delete shared.mutex;
    delete shared.nempty;
    delete shared.nstored;
    exit(0);
}

void produce()
{
    int		i;

    for (i = 0; i < nitems; i++) {
        shared.nempty->acquire();	/* acquire for at least 1 empty slot */
        shared.mutex->acquire();
        shared.buff[i % NBUFF] = i;	/* store i into circular buffer */
        shared.mutex->release();
        shared.nstored->release();	/* 1 more stored item */
    }
}

void consume()
{
    int		i;

    for (i = 0; i < nitems; i++) {
        shared.nstored->acquire();		/* acquire for at least 1 stored item */
        shared.mutex->acquire();
        if (shared.buff[i % NBUFF] != i)
            printf("buff[%d] = %d\n", i, shared.buff[i % NBUFF]);
        shared.mutex->release();
        shared.nempty->release();		/* 1 more empty slot */
    }
}
