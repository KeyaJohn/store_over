#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <aio.h>
#include "init_def_info.h"
#include "init_disk_info.h"
#include "sl_log.h"
#include "write_data_pth.h"
#include "sl_com.h"
#include "deal_data_pth.h"
#include "sig_recv_deal.h"
#include "capture_package.h"

int     daemon_flags = 0;
char    log_type[8] = {0};

//获取运行程序的参数
static int get_opt(int argc, char *argv[])
{
    int opt;
    while((opt = getopt(argc, argv, "di:")) != -1) 
    {
        switch(opt) 
        {
            case 'd':
                daemon_flags = 1;
                break;
            case 'i':
                printf("-----param is i,[%s]\n", optarg);
                break;
            default:
                return -1;
        }
    }
    return 0;
}

//c初始化系统
void init_system()
{
    //从配置文件中获得默认数据
	init_def_info();
    print_data();
    //初始化日志系统
	if (open_log(def_info->log_file,log_type))
	{
		return ;
	}
    
    //初始磁盘信息
	init_disk_info();
    print_disk_info();
    
    //初始化写线程信息
    puts("\n\n");
	init_write_pth();
    print_write_pth_info();
    
    //初始化数据处理线程信息
    puts("\n\n");
	init_deal_pth_info();
    print_deal_pth_info();
}

void choose_mode(int argc,char *argv[])
{
    if (get_opt(argc, argv) < 0) 
	{
		printf("parse option error\n");
		return ;
	}
	if (daemon_flags) 
	{
        daemon(1,0);
		strcpy(log_type, "f_cat");
	}
	else 
	{
		strcpy(log_type, "o_cat");
	}
}

int main(int argc, char *argv[])
{
    //选择该系统运行模式,守护进程模式与调试模式
    choose_mode(argc,argv);
	//c初始化系统
	init_system();
    
    //启动信号检测模块
    //start_sig_listen();

    //启动数据写入模块
    start_write_data();

    //启动数据处理模块
    start_deal_data();

    start_capture_package();
	while(1) 
	{
//		DBG("Run..........\n");
		sleep(10);
	}
	return 0;
}
