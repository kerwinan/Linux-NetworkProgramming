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
	//调用socket函数创建套接字
	sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock_fd < 0){
		perror("socket error!");
		return -1;
	}
	//赋值发送地址信息,服务器端地址信息
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(9000);
	ser_addr.sin_addr.s_addr = inet_addr("192.168.121.128");

	cli_addr.sin_family = AF_INET;
	cli_addr.sin_port = htons(8000);
	cli_addr.sin_addr.s_addr = inet_addr("192.168.122.128");

	int len = sizeof(struct sockaddr_in);
	int ret;
	int client_fd;
	//调用 bind 函数绑定一个发送地址信息
	client_fd = bind(sock_fd, (struct sockaddr*)&cli_addr, len);
	//调用 connect 函数连接服务器
	ret = connect(sock_fd, (struct sockaddr*)&ser_addr, len);
	if (ret < 0){
		perror("connect error!!!");
		return -1;
	}
	int i = 0;
	while (1){
		memset(buff, 0x00, 1024);
		sprintf(buff, "test-%d", i++);
		//连接成功后，调用 send 函数发送数据到服务器端
		send(sock_fd, buff, 1024, 0);
		sleep(1);
	}
	//调用 close 函数关闭套接字
	close(sock_fd);
	return 0;
}
