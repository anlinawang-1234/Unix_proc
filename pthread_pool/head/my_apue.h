#ifndef __HEAD_APUE__
#define __HEAD_APUE__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <pthread.h>

#define BUFFLEN       256

#define SHMKEY00      0xffffa000
#define MSGKEY00      0xffffb000
#define SEMKEY00      0xffffc000

#define thp(...) {                       \
  printf("[%s:%04d] ",__FILE__,__LINE__); \
  printf(__VA_ARGS__);                    \
}
#define thperr(...) {                                       \
  printf("[Error:%02d][%s:%04d] ",errno,__FILE__,__LINE__); \
  printf(__VA_ARGS__);                                      \
}

typedef struct {
    int    shmid         ;
    int    seque         ;
    char   cache[BUFFLEN];
}_shmctl;

typedef struct {
    long mtype;
    char msgtext[BUFFLEN];
}_msgctl;

typedef union {
    int              val  ;  /* Value for SETVAL */
    struct semid_ds *buf  ;  /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
}_un_semun;

#endif //__HEAD_APUE__
