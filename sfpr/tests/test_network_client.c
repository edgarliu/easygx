#include "sfpr_network.h"
#include <string.h>
#define TCP
#define NOBLOCK

#if defined VXWORKS
int client_main(char *addr, int in)
#else
int main(int argc,char *argv[])
#endif
{
	int ret = 0;
	sfpr_socket_t *sock;
	char buffer[256];
	int irsize = 0;
	char host[100];
	int port = 0;
	struct sockaddr_in to;
	struct sockaddr_in from;

#ifdef VXWORKS	
    if(addr == NULL || in == 0)
    {
		printf("please input host and port.\n");
		exit(0);
    }
	memset(host, 0x0, sizeof(host));
	strcpy(host, addr);
	port = in;
#else
	if(argc < 3)
	{
		printf("please input host and port.\n");
		exit(0);
	}

	memset(host, 0x0, sizeof(host));
	strcpy(host, argv[1]);
	port = atoi(argv[2]);

#endif

	ret = sfpr_socket_create(&sock);
	if(ret < 0)
	{
		printf("sfpr_socket_create fail.\n");
		exit(1);
	}

	ret = sfpr_socket_socket(sock, sfpr_socket_protocol_tcp);
	if(ret < 0)
	{
		printf("sfpr_socket_create fail.\n");
		exit(1);
	}
#ifdef TCP	
	ret = sfpr_socket_init(sock, sfpr_socket_protocol_tcp, host, port, sfpr_socket_mode_client);
#else
	ret = sfpr_socket_init(sock, sfpr_socket_protocol_udp, host, port, sfpr_socket_mode_client);
#endif

	if(ret < 0)
	{
		printf("sfpr_socket_create fail:%d\n", sock->error);
		sfpr_socket_destroy(&sock);
		exit(1);
	}
	else
	{
		printf("sfpr_socket_create success.\n");
	}

	while(1)
	{
#ifdef TCP	
		ret = sfpr_socket_connect(sock);
		if(ret < 0)
		{
			printf("sfpr_socket_connect fail:%d\n", sock->error);
			sfpr_socket_close(sock);
			sfpr_socket_destroy(&sock);
			exit(1);
		}
		else
		{
			printf("sfpr_socket_connect success.\n");
		}
#endif

#ifdef NOBLOCK
		ret = sfpr_socket_nonblock_set(sock);
		if(ret < 0)
		{
			printf("sfpr_socket_nonblock_set fail:%d\n", sock->error);
			sfpr_socket_close(sock);
			sfpr_socket_destroy(&sock);
			exit(1);
		}
		else
		{
			printf("sfpr_socket_connect success.\n");
		}

		sfpr_socket_timeout_set(sock, 1000);
		
#endif

		while(1)
		{
			//∑¢ÀÕ
			memset(buffer, 0x0, sizeof(buffer));
			sprintf(buffer, "this is client's test.\n");
#ifdef TCP		
			ret = sfpr_socket_send(sock, buffer, strlen(buffer), &irsize);
#else
			memset(&to, 0x0, sizeof(to));
			to.sin_family = AF_INET;
			to.sin_port = htons(port);
			to.sin_addr.s_addr = inet_addr(host);
			ret = sfpr_socket_sendto(sock, to, buffer, strlen(buffer), &irsize);
#endif
			if(ret < 0)
			{
				printf("sfpr_socket_send fail:%d\n", sock->error);
				sfpr_socket_close(sock);
				break;
			}
			else
			{
				printf("sfpr_socket_send success:%d\n", irsize);
			}

			//Ω” ’
			memset(buffer, 0x0, sizeof(buffer));
#ifdef NOBLOCK
			while(1)
			{
				ret = sfpr_socket_wait(sock, 1);
				if (ret != SFPR_SUCCESS) 
				{
					printf("there is no data for recv:%d\n", ret);
					continue;
				}
				break;
			}
			printf("check: there has data to recv.\n");
#endif

#ifdef TCP		
			ret = sfpr_socket_recv(sock, buffer, sizeof(buffer), &irsize);
#else
			ret = sfpr_socket_recvfrom(sock, &from, buffer, sizeof(buffer), &irsize);
#endif
			if(ret < 0)
			{
				printf("sfpr_socket_recv fail:%d\n", sock->error);
				sfpr_socket_close(sock);
				break;
			}
			else
			{
				printf("sfpr_socket_recv success:%d, %s\n", irsize, buffer);
				
#ifdef SHORTCONN
				sfpr_socket_close(sock);
#endif
			}
			sfpr_time_msleep(5000);
		}
	}

	sfpr_socket_close(sock);
	sfpr_socket_destroy(&sock);
	return 0;
}

