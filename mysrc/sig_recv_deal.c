#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <aio.h>
#include "sl_log.h"
#include "init_def_info.h"
#include "sig_recv_deal.h"
#include "init_disk_info.h"

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
    disk_info_t *d_info;
    d_info = (disk_info_t*)info->si_ptr;
    
    d_info->w_flag = 1;
    DBG("\t\t写入成功！---------------disk__id:%d---------------------------------------------------------",d_info->disk_id);
    return ;
}

int make_sig_action()
{
    struct sigaction sig_act;
    //清空信号集和
    if (sigemptyset(&sig_act.sa_mask) < 0) 
    {
        return -1;
    }
    sig_act.sa_flags = SA_SIGINFO;
    sig_act.sa_sigaction = sig_write;

    if (sigaction(SIG_RETURN, &sig_act, NULL)) 
    {
        return -1;
    }
    //block_allsig(UNMASK_SIG);
    return 0;
}




