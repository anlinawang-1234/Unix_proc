#include "monkey.h"

/*readme:
 * step1 : to reserve the monkey's name(R|r|L|l), wide, time, speed & the count into a struct
 * step2 : in the function gap_bridge fork a child process, if child process print let the parent
 *         pause, then if the child process reach the limit time send a signal to parent and let him
 *         pause. if either of them finished the print send a signal(different from the above signal)
 *         , and in the signal's handler, you can finish the last letter's print
 * attion : in order to aviod the print the same letter twice, before you print you should do some judge
 *          the judge detail as follow
 */

_monkey l_mon;
_monkey r_mon;
int c_run = ON;
int p_run = ON;

sigset_t ch_newmask, ch_oldmask, p_newmask, p_oldmask;
void err_arg_exit(char *str)
{
    printf("Usage : %s < R | r | l | L >\n", str);
    exit(0);
}
void sig_usr1(int signo)
{
    printf("child get signal [%d], printing...\n", signo);
}
void sig_usr2(int signo)
{
    printf("parent get signal [%d], printing...\n", signo);
}
void sig_alrm(int signo)
{
    int i = 0;
    int count = l_mon.count;
    int u_time = 0;
    int len = BRI_LEN;
    int flag = 0;
    
    if(1 == p_run){
        for(i = 0; i < count; ++i){
            if('\0' == l_mon.name[i]){
                continue;
            }
            printf("<<<<<<<<<<<< [ %c ] <<<<<<<<<<<\n", l_mon.name[i]);
            len += l_mon.wide[i];
            if('L' == l_mon.name[i]){
                flag = 1;
            }
        }
        if(1 == flag){
            u_time = len / OLD_SPEED;
        }else{
            u_time = len / YOU_SPEED;
        }
        printf("use time : %d\n", u_time);
        p_run = 0;
    }
}
void sig_int(int signo)
{
    int i = 0;
    int count = r_mon.count;
    int u_time = 0;
    int len = BRI_LEN;
    int flag = 0;
   
    /* if child finished print then c_run should not be '1'
     * if c_run == 0 then even if parent send a 'finish print' signal
     * this code will not to be exected
     */
    if(1 == c_run){
        for(i = 0; i < count; ++i){
            if('\0' == r_mon.name[i]){
                continue;
            }
            printf("<<<<<<<<<<<< [ %c ] <<<<<<<<<<<\n", r_mon.name[i]);
            if('R' == r_mon.name[i]){
                flag = 1;
            }
            len += r_mon.wide[i];
        }
        if(1 == flag){
            u_time = len / OLD_SPEED; 
        }else{
            u_time = len / YOU_SPEED;
        }
        printf("use time : %d\n", u_time);
        c_run = 0;
    }
}
void tell_wait()
{
    if(SIG_ERR == signal(SIGUSR1, sig_usr1)){
        // signal error
    }
    if(SIG_ERR == signal(SIGUSR2, sig_usr2)){
        // signal error
    }
    if(SIG_ERR == signal(SIGINT, sig_int)){
       // signal error
    }
    if(SIG_ERR == signal(SIGALRM, sig_alrm)){
      // signal error
    }
    sigemptyset(&ch_newmask);
    sigfillset(&ch_newmask);
    sigdelset(&ch_newmask, SIGUSR1);
    sigdelset(&ch_newmask, SIGUSR2);
    sigdelset(&ch_newmask, SIGALRM); 
    sigdelset(&ch_newmask, SIGINT);
    sigemptyset(&p_newmask);
    sigfillset(&p_newmask);
    sigdelset(&p_newmask, SIGUSR1);
    sigdelset(&p_newmask, SIGUSR2);
    sigdelset(&p_newmask, SIGALRM); 
    sigdelset(&p_newmask, SIGINT);

    if(0 > sigprocmask(SIG_BLOCK, &ch_newmask, &ch_oldmask)){
        // sigprocmask error
    }
    if(0 > sigprocmask(SIG_BLOCK, &p_newmask, &p_oldmask)){
        // sigprocmask error
    }
}
void child_print(pid_t pid)
{
    int i = 0;
    int is_old = 0;
    int len = BRI_LEN;
    int flag = 0;
    char ch = 0;
    int count = r_mon.count;
    int u_time = 0;

    for(i = 0; i < count; ++i){
        if('\0' == r_mon.name[i]){
            continue;
        }
        if('R' == r_mon.name[i] && 2 == is_old){
            continue;
        }
        printf(">>>>>>>>>>>> [ %c ] >>>>>>>>>>>>\n", r_mon.name[i]);
        ch = r_mon.name[i];
        len += r_mon.wide[i];
        if('R' == ch){
            flag = 1;
            is_old++;
        }
        if(1 == flag){
            u_time = len / OLD_SPEED;
        }else{
            u_time = len / YOU_SPEED;
        }
        r_mon.name[i] = '\0';
        if(LIMIT_TIME <= u_time){
            printf("use time : %d\n", u_time);
            kill(pid, SIGUSR2);
            return ;
        }
    }
    c_run = 0;    
    printf("use time : %d\n", u_time);
}
void parent_print(pid_t pid)
{
    int i = 0;
    int is_old = 0;
    int count = l_mon.count;
    int u_time = 0;
    int flag = 0;
    int len = BRI_LEN;
    int ch = 0;

    for(i = 0; i < count; ++i){
        if('\0' == l_mon.name[i]){
            continue;
        }
        if('L' == l_mon.name[i] && 2 == is_old){
            continue;
        }
        printf("<<<<<<<<<<<< [ %c ] <<<<<<<<<<<\n", l_mon.name[i]);
        ch = l_mon.name[i];
        len += l_mon.wide[i];
        if('L' == ch){
            flag = 1;
            is_old++;
        }
        if(1 == flag){
            u_time = len / OLD_SPEED;
        }else{
            u_time = len / YOU_SPEED;
        }
        l_mon.name[i] = '\0';
        if(LIMIT_TIME <= u_time){
            printf("use time : %d\n", u_time);
            kill(pid, SIGUSR1);
            return ;
        }
    }
    p_run = 0;    
    printf("use time : %d\n", u_time);
}
int gap_bridge()
{
    pid_t pid;

    tell_wait();
    pid = fork();
    if(0 > pid){
        // fork error
    }else if(0 == pid){
        while(c_run){
            child_print(getppid());
            /* if the child SIGINT handler had print, the you should not to pause*/
            if(c_run){
                pause();     
            }else{
                break;
            }
        }
        printf("child finished\n");
        kill(getppid(), SIGALRM);
        if(0 > sigprocmask(SIG_SETMASK, &ch_oldmask, NULL)){
            // sigprocmask error
        }
        _exit(0);
    }else{
        while(p_run){
            pause();
            /*if child finish print, then then send a signal to parent, the signal handler
             * print the last letter and let 'p_run = 0', so you can use this to justify if
             * the parent print in the handler*/
            if(p_run){
                parent_print(pid);
            }else{
                break;
            }
        }
        printf("parent finished\n");
        kill(pid, SIGINT);
        if(0 > sigprocmask(SIG_SETMASK, &p_oldmask, NULL)){
            // sig procmask error
        }
        waitpid(pid, NULL, 0);
        /* should not let the buffer contain to effect the corrent output*/
        _exit(0);
    }
}
int do_work(int ac, char **av)
{
    int loop = 0;
    int i = 0;
    int r = 0;
    int l = 0;

    if(2 > ac){
        err_arg_exit(av[0]);
    }
    for(loop = 1; loop < ac; ++loop){
        while(av[loop][i]){
            switch (av[loop][i]){
                case 'r' : r_mon.name[r] = 'r';
                           r_mon.wide[r] = YOU_WIDE;
                           r_mon.count++;
                           ++r;
                           break;
                case 'R' : r_mon.name[r] = 'R';
                           r_mon.wide[r] = OLD_WIDE;
                           r_mon.count++;
                           ++r;
                           break;
                case 'l' : l_mon.name[l] = 'l';
                           l_mon.wide[l] = YOU_WIDE;
                           l_mon.count++;
                           ++l;
                           break;
                case 'L' : l_mon.name[l] = 'L';
                           l_mon.wide[l] = OLD_WIDE;
                           l_mon.count++;
                           ++l;
                           break;
                default  :
                    err_arg_exit(av[0]);
            }
            i++;
        }
    }
    gap_bridge();
    return OK;
}
int main(int ac, char **av)
{
    do_work(ac, av);
    return 0;
}
