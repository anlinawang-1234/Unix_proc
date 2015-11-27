#include "poolfunc.h"
#include "pooldata.h"

#define MAXARGS 6

int runflg = ON;

void handle(int sig)
{
    runflg = OFF;
}
void func(void *args)
{
    char *str = (char *)args;
    printf(str);
}

int main(int ac, char **av)
{
    char *args[] = {"Hello\n", "World\n", "I\n", "Like\n", "Write\n", "Process\n"};
    int thnum = 0;
    int argnum = 0;

    if(1 >= ac){
       printf("Usage : %s <thread_num>\n", av[0]);
       return 1;
    }
    thnum = atoi(av[1]);
    if(SIG_ERR == signal(SIGINT, handle)){
        printf("signal error\n");
        return 1;
    }
    if(!pthread_pool_create(thnum)){
        printf("pthread create error\n");
        return 2;
    }
    while(runflg){
        //printf("app %08x\n",args[argnum]);
        pthread_pool_mission(func, args[argnum]);
        argnum = (argnum + 1) % MAXARGS;
        usleep(1000000);
    }
    pthread_pool_end();
    return 0;
}
