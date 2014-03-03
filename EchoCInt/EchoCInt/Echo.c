#include <stdio.h>
#include <WinSock2.h>
#include <stdlib.h>

#pragma comment(lib, "ws2_32.lib")			/*Winsock使用的库函数*/

#define ECHO_DEF_PORT		7				/*连接的默认端口*/
#define ECHO_BUF_SIZE		256				/*缓冲区的大小*/

int main(int argc, char **argv)
{
	WSADATA wsa_data = {0};
	SOCKET echo_soc = 0;					/*socket句柄*/
	struct sockaddr_in serv_addr = {0};			/*服务器地址*/
	unsigned short port = ECHO_DEF_PORT;	/*端口号*/
	int result = 0;						/*存储Winsock函数返回值*/
	int send_len = 0;						/*字符串长度*/

	char *test_data = "Hello World!";		/*测试数据*/
	char	 recv_buf[ECHO_BUF_SIZE];			/*缓冲区*/

	if (argc < 2)	
	{
		printf("input %s server_address [port]!\n", argv[0]);
		return EXIT_FAILURE;
	}

	if (argc >= 3)	
	{
		port = atoi(argv[2]);	//将字符串转换为整型数
	}

	if ( 0 != WSAStartup(MAKEWORD(2, 2), &wsa_data))
	{
		printf("[Echo Client] WSAStartup error : %d.\r\n", WSAGetLastError());
	}
	/*初始化Winsocket资源,它必须是被应用程序调用的第一个Winsock函数,允许应用程序指定要使用的Winsock版本*/
	
	send_len = strlen(test_data);

	/*服务器地址*/
	serv_addr.sin_family = AF_INET;		//地址簇, Internet地址簇都用AF_INET标识
	serv_addr.sin_port = htons(port);		//十六位端口号,必须是网络字节序
	serv_addr.sin_addr.S_un.S_addr = inet_addr(argv[1]);//32位IP地址,inet_addr函数将点分十进制数转换为u_long型数

	if (serv_addr.sin_addr.s_addr == INADDR_NONE)
	{
		printf("[ECHO] invalid address!\n");
		return EXIT_FAILURE;
	}

	echo_soc = socket(AF_INET, SOCK_STREAM, 0);			/*创建socket*/
	result = connect(echo_soc, (struct sockaddr *)&serv_addr, sizeof(serv_addr));	/*建立与服务器的连接*/
	if (0 == result)	/*连接成功*/
	{
		result = send(echo_soc, test_data, send_len, 0);
		result = recv(echo_soc, recv_buf, ECHO_BUF_SIZE, 0);
	}

	if (result > 0)
	{
		recv_buf[result] = '\0';
		printf("[Echo Client] receives: \"%s\"\r\n", recv_buf);
	}
	else
	{
		printf("[Echo Client] error: %d.\r\n", WSAGetLastError());
	}
	
	/*关闭连接*/
	closesocket(echo_soc);
	WSACleanup();

	return 0;
}