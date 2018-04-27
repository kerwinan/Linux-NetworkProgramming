#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int main(int argc, char *argv[])
{
	int sock_fd;
	struct sockaddr_in ser_addr;
	struct sockaddr_in cli_addr;
	char buff[1024] = {0};
	//调用socket函数创建监听套接字，协议族为AF_INET,socket类型为SOCK_STREAM流式套接字，协议为
	//IPPROTO_TCP协议,也可以为0，默认
	sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock_fd < 0){
		perror("socket error!");
		return -1;
	}
	//赋值监听地址信息
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(9000);
	ser_addr.sin_addr.s_addr = inet_addr("192.168.121.128");
	int len = sizeof(struct sockaddr_in);
	int ret;
	int client_fd;
	//调用bind函数将套接字与地址信息进行绑定
	client_fd = bind(sock_fd, (struct sockaddr*)&ser_addr, len);
	if (client_fd < 0){
		perror("bind error!!");
		return -1;
	}
	//调用listen函数开始监听套接字(即开始监听地址，等待连接请求的到来)
	ret = listen(sock_fd, 5);
	if (ret < 0){
		perror("listen error!!");
		return -1;
	}
	while (1){
		//调用accept函数接收到来的连接请求，并获取客户端的地址信息，返回一个传输socket套接字
		client_fd =  accept(sock_fd, (struct sockaddr*)&cli_addr, &len);
		if (client_fd < 0){
			perror("client_fd error!!");
			continue;
		}
		printf("new client:-ip:[%s], port:[%d]\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
		while (1){
			memset(buff, 0x00, 1024);
			//调用recv函数接收客户端传输过来的数据
			ret = recv(client_fd, buff, 1024, 0);
			printf("[%s]\n", buff);
			sleep(1);
		}
		//调用close函数关闭套接字
		close(client_fd);
	}
	close(sock_fd);
	return 0;
}
