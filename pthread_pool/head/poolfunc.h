#ifndef _POOLFUNC_H_
#define _POOLFUNC_H_

#include "pooldata.h"

int pthread_pool_create(int thunm);
int pthread_pool_mission(void (*func)(void *args), void *args);
int pthread_pool_end(void);
#endif
