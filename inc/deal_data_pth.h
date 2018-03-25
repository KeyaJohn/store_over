#ifndef __SL_READ_H__
#define __SL_READ_H__

#include "sl_com.h"
#include "init_disk_info.h"
#define BUFF 250
typedef struct node
{
    char buff[BUFF];
}data_node_t;
typedef struct __rthr_info_t
{
    int thr_id;
    int cpu_id;
    int min_disk_id;
    int disk_num;
    node_info_t **buffer;
    que_t * data_que;
}rthr_info_t;

extern rthr_info_t *rthr_info;

int start_deal_data();
int init_deal_pth_info();
void print_deal_pth_info();
#endif
