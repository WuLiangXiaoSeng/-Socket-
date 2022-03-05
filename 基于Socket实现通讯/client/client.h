#include <unistd.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <semaphore.h>

#define NUM 32

typedef struct {
	int command;
	char sendto[12];
	char self[12];
	char mess[256];
}msg;


int init_client();		//初始化客户端

void login(int comm_sock);	//登录

void logout(int comm_sock);	//退出

void *chat_read(void *p);	//线程函数，负责读从服务器端发送过来的数据

void deal_Pchat(int comm_sock);	//处理私聊

void deal_Tchat(int comm_sock);	//处理群聊

void addto_group(int comm_sock);//加群

void ask_online(int comm_sock);	//查看在线的好友

void send_offline(int comm_sock);//发送离线消息

void ask_offline(int comm_sock);//接收离线消息
