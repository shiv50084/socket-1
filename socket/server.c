#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define QUEUE_LINE 12
#define SOURCE_PORT 8000

#define SOURCE_IP_ADDRESS "192.168.177.128"
void process_info(pid_t pid)
{
	int recv_num;
	int send_num;
	char recv_buf [50];
	char send_buf []="shenlei";
	while (1)
	{
		printf("begin recv:\n");
		recv_num = recv(pid, recv_buf, sizeof(recv_buf), 0);
		if (recv_num < 0)
		{
			perror("recv error");
			exit(1);
		}
		else
		{
			recv_buf[recv_num] = '\0';
		}
		printf("receive %d bytes: %s\n",recv_num, recv_buf);
		send(pid, send_buf, sizeof(send_buf), 0);
	}
}


int main()
{
	int sock_fd, conn_fd;
	int client_len;
	pid_t pid;
	struct sockaddr_in addr_serv, addr_client;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0)
	{
		perror("socket");
		exit(1);
	}
	else
	{
		printf ("sock successfully\n");
	}
	//Init the server addr
	memset(&addr_serv, 0, sizeof(struct sockaddr_in));
	addr_serv.sin_family = AF_INET;
	addr_serv.sin_port = htons(SOURCE_PORT);
	addr_serv.sin_addr.s_addr = inet_addr(SOURCE_IP_ADDRESS);
	client_len = sizeof(struct sockaddr_in);
	
	//bind the server addr
	if (bind(sock_fd, (struct sockaddr*)&addr_serv, sizeof(struct sockaddr_in)) < 0)
	{
		perror("bind error");
		exit(1);
	}
	else
	{
		printf("bind success");	
	}
	if (listen(sock_fd, 5) < 0)
	{
		perror("listen error");
	}
	else
	{
		printf("listen success");	
	}
	//waiting for client connect
	while (1)
	{
		printf("begin accept");
		conn_fd = accept(sock_fd, (struct sockaddr*)&addr_client, (socklen_t*)&client_len);
		if (conn_fd < 0)
		{
			perror("accept error");
			exit(1);
		}
		printf("accept a new client ,ip:%s", inet_ntoa(addr_client.sin_addr));
		pid = fork();
		if (pid == 0)
		{
			//child 
			close(sock_fd);
			process_info(conn_fd);
		}
		else
		{
			close(conn_fd);
		}
	}
	
}






