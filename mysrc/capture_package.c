#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include "sl_log.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <pcap/pcap.h>
#include "capture_package.h"

//当数据有到来时，出发该回调函数，对数据进行处理
void getPacket(u_char * arg, const struct pcap_pkthdr * pkthdr, const u_char * packet)  
{  
    int * id = (int *)arg;  
    
    printf("id: %d\n", ++(*id));  
    printf("Packet length: %d\n", pkthdr->len);  
    printf("Number of bytes: %d\n", pkthdr->caplen);  
    printf("Recieved time: %s", ctime((const time_t *)&pkthdr->ts.tv_sec));   

    HEAD  *head = NULL;  
    head = (HEAD*)packet;  //struct ether_header 以太网帧头部  

    //获取源MAC 
    printf("Mac Destination Address is %02x:%02x:%02x:%02x:%02x:%02x\n",head->mac_head.DstMAC[0],head->mac_head.DstMAC[1],head->mac_head.DstMAC[2],head->mac_head.DstMAC[3],head->mac_head.DstMAC[4],head->mac_head.DstMAC[5]);
    //获取目标MAC 
    printf("Mac      Source Address is %02x:%02x:%02x:%02x:%02x:%02x\n",head->mac_head.SrcMAC[0],head->mac_head.SrcMAC[1],head->mac_head.SrcMAC[2],head->mac_head.SrcMAC[3],head->mac_head.SrcMAC[4],head->mac_head.SrcMAC[5]);
    //获取类型 IP ARP  RARP
    printf("type%x---------------------------\n",head->mac_head.FrameType);

    //获取ip信息，总共二十个字节
    printf("Ver_HLen is %2x\n",head->ip_head.Ver_HLen);
    printf("Tos is %02x\n",head->ip_head.TOS);
    printf("TotalLen is %04x\n",ntohs(head->ip_head.TotalLen));
    printf("ID IP is %04x\n",ntohs(head->ip_head.ID));
    printf("Flag_Segment is %04x\n",ntohs(head->ip_head.Flag_Segment));
    printf("TTL is %02x\n",head->ip_head.TTL);
    printf("Protocol is %02x\n",head->ip_head.Protocol);
    printf("Checksum is %04x\n",ntohs(head->ip_head.Checksum));
    printf("SrcIP is %08x\n",ntohl(head->ip_head.SrcIP));
    printf("DstIP is %08x\n",ntohl(head->ip_head.DstIP));
    //将网路字节IP转换为常见的IP字符串
    struct in_addr addr;
    addr.s_addr = head->ip_head.SrcIP;
    printf("SrcIP is %s\n",inet_ntoa(addr));
    addr.s_addr = head->ip_head.DstIP;
    printf("SrcIP is %s\n",inet_ntoa(addr));

    //输出数据包的前六十个字节,数据不重要,只要收到数据头的信息就可以知道网络状况
    int i;  
    for(i=0; i<HEAD_LENGTE; ++i)  
    {  
        printf(" %02x", packet[i]);  
        if( (i + 1) % 16 == 0 )  
        {  
            printf("\n");  
        }  
    }  
    printf("\n\n");  
    sleep(1);
}  

void *capture_package(void *arg)  
{  
    char errBuf[PCAP_ERRBUF_SIZE], * devStr;  

    /* 获得网卡设备 */  
    devStr = pcap_lookupdev(errBuf);  
    if(devStr)  
    {  
        printf("success: device: %s\n", devStr);  
    }  
    else  
    {  
        printf("error: %s\n", errBuf);  
        exit(1);  
    }  

    //打开网络设备，直到数据到来
    pcap_t * device = pcap_open_live(devStr, 65535, 1, 0, errBuf);  
    if(!device)  
    {  
        printf("error: pcap_open_live(): %s\n", errBuf);  
        exit(1);  
    }  

    /* 设置过滤条件,此处可以设置指定抓取某一个网段的数据包*/  
    //struct bpf_program filter;  
    // pcap_compile(device, &filter, "src host 192.168.93.128", 1, 0);  
    //pcap_setfilter(device, &filter);  

    /* 当有数据时，启用回调函数getPacket()*/  
    pcap_loop(device, -1, getPacket,NULL);  

    //关闭网卡设备
    pcap_close(device);  
    return 0;  
}  

//创建线程，该线程从网络接口抓取数据，并放到各个处理线程中去

int start_capture_package()
{
    pthread_t tid;
    int ret =  pthread_create(&tid,NULL,(void*)capture_package,NULL);
    if( ret == -1 )
    {
        ERR("create capture_package pthread faile!\n");
        return -1;
    }
    return 0;
}
