#include "pooldata.h"
#include "poolfunc.h"

_pool *thp;
int run = ON;
pthread_mutexattr_t mutexattr_lock;
pthread_mutex_t     mutex_lock;

void sig_had1(int sig)
{

}
void sig_had2(int sig)
{
    run = OFF;
}
void *umem_make(int key, int size, int *id)
{
    void *addr = NULL;
    int shmid = 0;

    if(0 >= size){
        thperr("size is less than zero\n");
        return addr;
    }
    if(ON == thp->runflg){
        thp("Already run\n");
        return addr;
    }
    shmid = shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
    if(0 > shmid){
        if(EEXIST != errno){
            thperr("get memory error\n");
            return addr;
        }
        thp = shmat(shmid, 0, 0);
    }else{
        thp = shmat(shmid, 0, 0);
    }
    if(NULL != thp){
        addr = thp;
        *id = shmid;
    } 
    return addr;
}
void *Malloc(size_t size)
{
    void *rst = malloc(size);
    if(NULL == rst){
        fprintf(stderr, "Over flow\n");
    }
    return rst;
}
Boolean pthread_mutex_set(pthread_mutexattr_t *attr, pthread_mutex_t *mutex)
{
    int rst = TRUE;

    if(rst && pthread_mutexattr_init(attr)){
        rst = FALSE;
    }
    if(rst && pthread_mutexattr_settype(attr, PTHREAD_MUTEX_NORMAL)){
        rst = FALSE;
    }
    if(rst && pthread_mutex_init(mutex, attr)){
        rst = FALSE;
    }
    return rst;
}
void *ufunc(void *fid)
{
    long id = (long)fid;

    while(run){
        pause();
        if(NULL != thp->thp[id].func){
            thp->thp[id].isbusy = BUSY;
            thp->thp[id].func(thp->thp[id].args);
            //printf("thread %08x\n",thp->thp[id].args);
        }
        thp->thp[id].func = NULL;
        thp->thp[id].args = NULL;
        thp->thp[id].isbusy = FREE;
        if(0 != thp->freeid){
            thp->freeid = thp->thp[id].tid;
        }
    }
    //printf("pthread [%ld] exit OK\n", thp->thp[id].tid);
    pthread_exit(NULL);
    return NULL;
}
int pthread_pool_create(int thnum)
{
    _pool *addr = NULL;
    void *id = NULL;
    int shmid  = 0;
    int rst = NG;
    int loop = 0;

    if(SIG_ERR == signal(SIGUSR1, sig_had1)){
        return rst;
    }
    if(SIG_ERR == signal(SIGUSR2, sig_had2)){
        return rst;
    }
    if(0 >= thnum){
        thperr("paramenter illegel\n");
        return rst;
    }
    pthread_mutex_set(&mutexattr_lock, &mutex_lock);

    thp = (_pool *)Malloc(sizeof(_pool));
    addr = (_pool *)umem_make(SHMKEY00, sizeof(_pool), &shmid);
    memset(thp, 0x00, sizeof(_pool));
    thp->shmid = shmid;
    thp->shmsize = sizeof(_pool);
    thp->runflg = ON;

    if(OK != configread(CONFPATH)){
        thperr("config read error\n");
        return rst;
    }
    if(thnum >= thp->threads){
        thperr("thread number is too big\n");
        return rst;
    }
    thp->threads = thnum;
    thp->thp = (_thread *)Malloc(sizeof(_thread) * thnum);
    for(loop = 0; loop < thnum; ++loop){
        strcpy((void *)&id, (void *)&loop);
        if(pthread_create(&(thp->thp[loop].tid),NULL, ufunc, (void *)id)){
            thperr("create thread error\n");
            break;
        }
    }
    rst = OK;
    return rst;
}
int pthread_pool_mission(void (*func)(void *args), void *args)
{
    int rst = NG;
    int fact = 0;
    pthread_t idle = 0;

    if(NULL == func || NULL == args){
        return rst;   
    }
    pthread_mutex_lock(&mutex_lock);
    if(0 != thp->freeid){
        idle = thp->freeid;
        thp->freeid = 0;
    }
    pthread_mutex_unlock(&mutex_lock);
    if(0 != idle){
        thp->thp[idle].func = func;
        thp->thp[idle].args = args;
        pthread_kill(idle, SIGUSR1);
        rst = OK;
    }else{
        for(fact = 0; fact < thp->threads; ++fact){
            if(BUSY == thp->thp[fact].isbusy){
                continue;
            }
            thp->thp[fact].func = func;
            thp->thp[fact].args = args;
            pthread_kill(thp->thp[fact].tid, SIGUSR1);
            rst = OK;
            break;
        }
    }
    return rst;
}
int pthread_pool_end(void)
{
    int shmid = thp->shmid;  
    int rst = NG;
    int loop = 0;

    for(loop = 0; loop < thp->threads; ++loop){
        pthread_kill(thp->thp[loop].tid, SIGUSR2);
    }
    pthread_mutex_lock(&mutex_lock);
    pthread_mutex_unlock(&mutex_lock);
    pthread_mutex_destroy(&mutex_lock);
    thp->runflg = OFF;
    for(loop = 0; loop < thp->threads; ++loop){
     // printf("loop : %d tid : %dn", loop, thp->threads);
        pthread_join(thp->thp[loop].tid, NULL);
    }
    if(0 > shmid){
        thperr("end error\n");
        rst = NG;
    }else{
        shmctl(shmid, IPC_RMID, 0);
        rst = OK;
    }
    return rst;
}



