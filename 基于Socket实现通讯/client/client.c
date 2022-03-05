#include "client.h"
#include "ui.h"

char self[12];


/*
 *
 *
 *
 *
 *
 *
 */
int init_client(){
	struct sockaddr_in ser_addr;
	int comm_sock;
	int res;

	comm_sock = socket(AF_INET, SOCK_STREAM, 0);
	
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	ser_addr.sin_port = 9999;

	res = connect(comm_sock, (struct sockaddr*)&ser_addr, sizeof(struct sockaddr));
	if(res == -1){
		printf("连接失败，退出\n");
		exit(-1);
	}
	return comm_sock;
}


/*
 *函数名: login()
 *参数：初始化的 socket
 *描述：客户端登录函数,command 
 *	command = 1,sendto = login, mess = login, self = scanf()
 *返回值：
 */
void login(int comm_sock){
	int msglen;
	msg sendmsg;
	msglen=sizeof(msg);
	memset(&sendmsg,'\0',msglen);
	memset(self,'\0',12);

	sendmsg.command = 1;

	strcpy(sendmsg.sendto,"login");

	strcpy(sendmsg.mess,"login");

	scanf("%s",self);

	strcpy(sendmsg.self,self);

	write(comm_sock,&sendmsg,msglen);
}


/*
 *
 *
 *
 *
 */
void logout(int comm_sock){
	int len;
	msg sendmsg;

	len = sizeof(msg);
	memset(&sendmsg, '\0', len);
	sendmsg.command = 8;
	strcpy(sendmsg.sendto, "logout");
	strcpy(sendmsg.mess, "exit");
	strcpy(sendmsg.self, self);
	write(comm_sock, &sendmsg, len);
}

/*
 *函数名；	chat_read()
 *描述：	聊天过程中读线程
 *参数：	comm_sock
 *返回值：
 *
 */
void *chat_read(void *p){
	int comm_sock = *(int*)p;

	msg mymsg;
	int len;

	len = sizeof(msg);
//	printf("i am here");
//	system("pause");
	while(1){
		memset(&mymsg,  '\0', len);
		read(comm_sock, &mymsg, len);
		printf("%s send to self : %s\n", mymsg.self, mymsg.mess);
		if(strcmp(mymsg.mess, "exit") == 0) 
			break;
	}
}

/*
 *
 *
 *
 *私聊，command == 2
 */
void deal_Pchat(int comm_sock){
	int len;
	msg sendmsg;

	len = sizeof(msg);
	while(1){
		memset(&sendmsg, '\0', len);
		sendmsg.command = 2;
		printf("Sendto：");
		scanf("%s", sendmsg.sendto);
		printf("Massage:");
		scanf("%s", sendmsg.mess);
		strcpy(sendmsg.self, self);
		if(strcmp(sendmsg.mess, "quit") == 0)
			break;
		write(comm_sock, &sendmsg, len);
	}
}


/*
 *
 *
 *
 *群聊 command == 3,
 */
void deal_Tchat(int comm_sock){
	int len;
	msg sendmsg;

	len = sizeof(msg);
	while(1){
		memset(&sendmsg, '\0', len);
		sendmsg.command = 3;
		printf("Sendto(groupid)：");
		scanf("%s", sendmsg.sendto);
		printf("Massage:");
		scanf("%s", sendmsg.mess);
		strcpy(sendmsg.self, self);
		if(strcmp(sendmsg.mess, "quit") == 0)
			break;
		write(comm_sock, &sendmsg, len);
	}
}

/*
 *
 *
 *加群 command == 4
 *
 */
void addto_group(int comm_sock){
	int len;
	msg sendmsg;

	len = sizeof(msg);
	memset(&sendmsg, '\0', len);
	sendmsg.command = 4;
	printf("GroupID:");
	scanf("%s", sendmsg.mess);
	printf("%s", sendmsg.mess);
	strcpy(sendmsg.sendto, "addto");
	strcpy(sendmsg.self, self);
	write(comm_sock, &sendmsg, len);

//	system("pause");
}

/*
 *
 *
 *
 *查看在线 command == 5
 */
void ask_online(int comm_sock){	
	int len;
	msg sendmsg;

	len = sizeof(msg);
	memset(&sendmsg, '\0', len);
	sendmsg.command = 5;
	strcpy(sendmsg.mess, "ask_for_online");
	strcpy(sendmsg.sendto, "server");
	strcpy(sendmsg.self, self);
	write(comm_sock, &sendmsg, len);
	getchar();
}


/*
 *
 *
 *
 *发送离线消息 command == 6
 */
void send_offline(int comm_sock){
	int len;
	msg sendmsg;

	len = sizeof(msg);
	while(1){
		memset(&sendmsg, '\0', len);
		sendmsg.command = 6;
		printf("Sendto:");
		scanf("%s", sendmsg.sendto);
		printf("Message");
		scanf("%s", sendmsg.mess);
		strcpy(sendmsg.self, self);
		if(strcmp(sendmsg.mess, "quit") == 0)
			break;
		write(comm_sock, &sendmsg, len);
	}
}

/*
 *
 *
 *
 *请求离线消息 command == 7
 */
void ask_offline(int comm_sock){	
	int len;
	msg sendmsg;

	len = sizeof(msg);
	memset(&sendmsg, '\0', len);
	sendmsg.command = 7;
	strcpy(sendmsg.mess, "ask_for_offline");
	strcpy(sendmsg.sendto, "server");
	strcpy(sendmsg.self, self);
	write(comm_sock, &sendmsg, len);
	getchar();
}

int main(){
	LoginUI();
	
	int comm_sock;
	int c;
	pthread_t tid;
	void *r;
	int len;
	msg sendmsg;

	comm_sock = init_client();
	login(comm_sock);

	MainUI();
	pthread_create(&tid, 0, chat_read, (void*)(&comm_sock));
	len = sizeof(msg);
	while(1){
		MainUI();
		scanf("%d", &c);
		if(c == 0){
			logout(comm_sock);
			break;
		}
		switch(c){
			case 0://logout
				logout(comm_sock);
				break;
			case 1://私聊
				deal_Pchat(comm_sock);
				break;

			case 2://群聊
				deal_Tchat(comm_sock);
				break;

			case 3://加群
				addto_group(comm_sock);
				break;

			case 4://查看在线名单
				ask_online(comm_sock);
				getchar();
				break;
			case 5://发送离线消息
				send_offline(comm_sock);
				break;
			case 6://接收离线消息
				ask_offline(comm_sock);
				getchar();
				break;

		}
	}

	pthread_join(tid, &r);

}
