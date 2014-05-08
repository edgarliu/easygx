#include "sfpr_network.h"
#include <string.h>

#define TCP
#define NOBLOCK
#define SHORTCONN

#if defined VXWORKS
int server_main(int argc)
#else
int main(int argc,char *argv[])
#endif
{

	int ret = 0;
	sfpr_socket_t *sock;
	sfpr_socket_t *client;
	char buffer[256];
	int irsize = 0;
	int port = 0;
	struct sockaddr_in from;
	int optvalue;
	int optlen = sizeof(int);

#ifdef VXWORKS
	if(argc == 0)
	{
		printf("please input host and port.\n");
		exit(0);
	}
	port = argc;
#else
	if(argc < 2)
	{
		printf("please input host and port.\n");
		exit(0);
	}
	port = atoi(argv[1]);
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
	ret = sfpr_socket_init(sock, sfpr_socket_protocol_tcp, NULL, port, sfpr_socket_mode_server);
#else
	ret = sfpr_socket_init(sock, sfpr_socket_protocol_udp, NULL, port, sfpr_socket_mode_server);
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

#ifdef NOBLOCK
	ret = sfpr_socket_nonblock_set(sock);
	if(ret < 0)
	{
		printf("sfpr_socket_nonblock_set fail:%d\n", sock->error);
		exit(1);
	}
	else
	{
		printf("sfpr_socket_nonblock_set success.\n");
	}

	sfpr_socket_timeout_set(sock, 1000);
#endif

    ret = sfpr_socket_opt_get(sock, SOL_SOCKET, SO_REUSEADDR, &optvalue, &optlen);
	printf("before modify: reuseaddr:%d:%d\n", ret, optvalue);
	optvalue = 1;
    ret = sfpr_socket_opt_set(sock, SOL_SOCKET, SO_REUSEADDR, &optvalue, sizeof(optvalue));
	printf("modify reuseaddr:%d\n", ret);
    ret = sfpr_socket_opt_get(sock, SOL_SOCKET, SO_REUSEADDR, &optvalue, &optlen);
	printf("after modify: reuseaddr:%d:%d\n", ret, optvalue);
	
    ret = sfpr_socket_bind(sock);
	if(ret < 0)
	{
		printf("sfpr_socket_bind fail:%d\n", sock->error);
		sfpr_socket_close(sock);
		sfpr_socket_destroy(&sock);
		exit(1);
	}
	else
	{
		printf("sfpr_socket_bind success.\n");
	}

#ifdef TCP
	ret = sfpr_socket_listen(sock, 5);
	if(ret < 0)
	{
		printf("sfpr_socket_listen fail:%d\n", sock->error);
		sfpr_socket_close(sock);
		sfpr_socket_destroy(&sock);
		exit(1);
	}
	else
	{
		printf("sfpr_socket_listen success.\n");
	}

#ifdef NOBLOCK
	while(1)
	{
		ret = sfpr_socket_wait(sock, 1);
		if (ret != SFPR_SUCCESS) 
		{
			printf("there is no client for conn:%d\n", ret);
			continue;
		}
		break;
	}
	printf("check: there has client to conn.\n");
#endif

	while(1)
	{
		ret = sfpr_socket_accept(sock, &client);
		if(ret < 0)
		{
			printf("sfpr_socket_accept fail:%d\n", sock->error);
			sfpr_socket_close(sock);
			sfpr_socket_destroy(&sock);
			exit(1);
		}
		else
		{
			printf("sfpr_socket_accept success.\n");
		}
#ifdef NOBLOCK
		ret = sfpr_socket_nonblock_set(client);
		if(ret < 0)
		{
			printf("sfpr_socket_nonblock_set fail:%d\n", sock->error);
			sfpr_socket_close(sock);
			sfpr_socket_destroy(&sock);
			sfpr_socket_close(client);
			sfpr_socket_destroy(&client);
			exit(1);
		}
		else
		{
			printf("sfpr_socket_connect success.\n");
		}

		sfpr_socket_timeout_set(client, 1000);
#endif
#endif


		while(1)
		{

			//Ω” ’
			memset(buffer, 0x0, sizeof(buffer));
			
#ifdef TCP	
#ifdef NOBLOCK
			while(1)
			{
				ret = sfpr_socket_wait(client, 1);
				if (ret != SFPR_SUCCESS) 
				{
					printf("there is no data for recv:%d\n", ret);
					continue;
				}
				break;
			}
			printf("check: there has data to recv.\n");
#endif

			ret = sfpr_socket_recv(client, buffer, sizeof(buffer), &irsize);
#else
			while(1)
			{
				ret = sfpr_wait_for_io_or_timeout(sock, 1);
				if (ret != SFPR_SUCCESS) 
				{
					printf("there is no data for recv:%d\n", ret);
					continue;
				}
				break;
			}
			printf("check: there has data to recv.\n");
			memset(&from, 0x0, sizeof(from));
			ret = sfpr_socket_recvfrom(sock, &from, buffer, sizeof(buffer), &irsize);
#endif
			if(ret < 0)
			{
				printf("sfpr_socket_recv fail:%d\n", /*sock->error*/errno);
#ifdef TCP	
				sfpr_socket_close(client);
				sfpr_socket_destroy(&client);
#endif	
				break;
			}
			else
			{
				printf("sfpr_socket_recv success:%d, %s\n", /*irsize*/ret, buffer);
			}
			
			//∑¢ÀÕ
			memset(buffer, 0x0, sizeof(buffer));
			sprintf(buffer, "this is server's test.\n");
#ifdef TCP		
			ret = sfpr_socket_send(client, buffer, strlen(buffer), &irsize);
#else
			ret = sfpr_socket_sendto(sock, from, buffer, strlen(buffer), &irsize);
#endif
			if(ret < 0)
			{
				printf("sfpr_socket_send fail:%d\n", sock->error);
#ifdef TCP	
				sfpr_socket_close(client);
				sfpr_socket_destroy(&client);
#endif	
				break;
			}	
			else
			{
				printf("sfpr_socket_send success:%d\n", irsize);
#ifdef SHORTCONN
				sfpr_socket_close(client);
				sfpr_socket_destroy(&client);
				break;
#endif
			}
		}
	}
	
	sfpr_socket_close(sock);
	sfpr_socket_destroy(&sock);
	
#ifdef TCP	
	sfpr_socket_close(client);
	sfpr_socket_destroy(&client);
#endif	
	return 0;
}

