#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <aio.h>
#include "sl_log.h"
#include "init_def_info.h"
#include "sig_recv_pth.h"
#include "init_disk_info.h"

sthr_info_t * sthr_info = NULL;

//flag: 0-屏蔽信号 1-解除屏蔽
int block_allsig(int flag)
{
    sigset_t  signal_set;

    if (sigemptyset(&signal_set) < 0) 
    {
        return -1;
    }
    if (sigfillset(&signal_set) < 0) 
    {
        return -1;
    }
    if (pthread_sigmask(flag, &signal_set, NULL) != 0) 
    {
        return -1;
    }
    return 0;
}


void init_sig_pth()
{
    sthr_info = (sthr_info_t*)malloc(sizeof(sthr_info_t));
    if( sthr_info == NULL )
    {
        ERR("malloc sthr_info err");
        return ;
    }
    int i;
    for(i=0; i<def_info->sthr_num; i++)
    {
        sthr_info[i].thr_id = i;
        sthr_info[i].cpu_id = i;
    }
}



/*
*     信号处理:
*             1. 判断数据是否写入成功
*             2. 成功则将node_info写入到空闲队列中
*             3. 失败则要重新写入数据
*             
*/
void sig_write(int signo, siginfo_t *info, void *ctext)
{
    /*disk_info_t *d_info;
    DBG("get signal:[%d]", signo);
    if (signo == SIG_RETURN)
    {
        d_info = (disk_info_t*)info->si_ptr;
        if (aio_error(d_info->my_aiocb) == 0) 
        {
            //写入成功
            if (d_info->my_aiocb->aio_nbytes != aio_return(d_info->my_aiocb)) 
            {
                //写入数据不完全
                while(aio_write(d_info->my_aiocb) < 0) 
                {
                    usleep(10);
                    continue;
                }
            }
            else 
            {
                while(!write_inval(d_info->fbuff, node_info_t, d_info->node_info)) 
                {
                    continue;
                }
                d_info->w_flag = 1;
            }
        }
        else 
        {
            while(aio_write(d_info->my_aiocb) < 0) 
            {

                usleep(10);
                continue;
            }
        }
    }
    */
    puts("jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj");
    return ;
}


void *ths_run(void *args)
{
    struct sigaction sig_act;
    if (sigemptyset(&sig_act.sa_mask) < 0) 
    {
        return NULL;
    }
    sig_act.sa_flags = SA_SIGINFO;
    sig_act.sa_sigaction = sig_write;

    if (sigaction(SIG_RETURN, &sig_act, NULL)) 
    {
        return NULL;
    }
    //block_allsig(UNMASK_SIG);

    puts("检测  oooooooooooooooooooooooooooooooiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiio\n");
    while(1) 
    {
        sleep(10);
    }
    /*
    int         sig;
    siginfo_t   info;
    sigset_t    signal_set;
    block_allsig(UNMASK_SIG);
    sigemptyset(&signal_set);
    sigaddset(&signal_set, SIG_RETURN);
    while(1) 
    {
        sig = sigwaitinfo(&signal_set, &info);
        if (sig == SIG_RETURN) {
        DBG("recv signale SIG_RETURN");
    }
    }
    */
    return NULL;
}

int start_sig_listen()
{
    pthread_t tid;
/*
    int i = 0;
    for (i = 0; i < def_info->sthr_num; i++) 
    {
        if (pthread_create(&tid, NULL, ths_run, NULL) < 0) 
        {
            ERR("start write thread error!");
            return -1;
        }
    }
    */
   pthread_create(&tid, NULL, ths_run, NULL);
    return 0;
}




