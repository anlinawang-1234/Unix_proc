#include "pooldata.h"

extern _pool *thp;
int configread(char *path)
{
    int rst = NG;
    int lpos = 0;
    int rpos = 0;
    FILE *fp;
    char buf[BUFFSIZE] = {0};
    char tmp[BUFFSIZE] = {0};

    if(NULL == path){
        return rst;
    }
    fp = fopen(path, "r");
    if(NULL == fp){
        thperr("open file error\n");
        return rst;
    }
    while(NULL != fgets(buf, BUFFSIZE, fp)){
        if('#' == buf[0] || '\n' == buf[0] || '\0' == buf[0]){
            continue;
        }
        while('=' != buf[lpos]){
            ++lpos;
        }
        ++lpos;
        rpos = lpos;
        while(';' != buf[rpos]){
            ++rpos;
        }
    }
    if(0 > (rpos - lpos)){
     return rst;
    }
    strncpy(tmp, buf+lpos, rpos-lpos);
    thp->threads = atoi(tmp);
    rst = OK;
    return rst;
}
