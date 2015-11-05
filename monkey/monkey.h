#include <apue.h>

#define BRI_LEN      (100)
#define YOU_WIDE   (5)
#define OLD_WIDE  (10)
#define YOU_SPEED  (10)
#define OLD_SPEED (5)
#define MAX_NUM      (1024)
#define LIMIT_TIME   (25)

typedef struct monkey{
    char name[MAX_NUM];
    int wide[MAX_NUM];
    int count;
}_monkey;

void err_arg_exit(char *str)   ;
void sig_usr1(int signo)       ;
void sig_usr2(int signo)       ;
void sig_alrm(int signo)       ;
void sig_int(int signo)        ;
void tell_wait()               ;
void child_print(pid_t pid)    ;
void parent_print(pid_t pid)   ;
int gap_bridge()               ;
int do_work(int ac, char **av) ;
