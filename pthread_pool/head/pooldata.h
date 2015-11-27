#ifndef _POOLDATA_H_
#define _POOLDATA_H_

#include "apue.h"
#include "exitcode.h"

#define CONFPATH "config/poolconfig.txt"
#define BUFFSIZE 256

typedef unsigned char Boolean;
#define TRUE  1
#define FALSE 0

typedef struct thread {
    pthread_t tid;
    int isbusy;
    void (*func)(void *args);
    void *args;
} _thread;

typedef struct pool {
    int shmid;
    int shmsize;
    pthread_t freeid;
    int threads;
    int runflg;
    _thread *thp;
} _pool;

#endif
