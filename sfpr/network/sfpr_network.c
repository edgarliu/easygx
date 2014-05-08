#include <string.h>
#include "sfpr_network.h"
#if defined WIN32
#include <WinSock2.h>
#elif defined VXWORKS
#include "selectLib.h"
#else
#include <sys/select.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

sfpr_int_t sfpr_socket_start_up()
{
	int ret = 0;
	#if defined WIN32
	WSADATA wsData;
	ret = WSAStartup(MAKEWORD(2,2), &wsData);
	if(ret != 0) //加载套接字库，加裁失败则返回
		return -1;
	#endif
	return ret;
}

sfpr_int_t sfpr_socket_clean_up()
{
#if defined WIN32
	WSACleanup();
#endif
}

sfpr_int_t sfpr_socket_create(sfpr_socket_t **sock)
{
	sfpr_int_t ret = SFPR_SUCCESS;
	sfpr_socket_t *_sock = NULL;
	if(sock == NULL){
		return SFPR_NOMEM;
	}
	_sock = malloc(sizeof(sfpr_socket_t));
	if(_sock == NULL){
		return SFPR_NOMEM;
	}
	memset(_sock, 0x0, sizeof(sfpr_socket_t));

	_sock->sock = -1;
	_sock->timeout = -1;
	*sock = _sock;
	return ret;
}

sfpr_int_t sfpr_socket_destroy(sfpr_socket_t **sock)
{
	sfpr_int_t ret = SFPR_SUCCESS;
	
	sfpr_socket_t *_sock = *sock;
	if (_sock == NULL)
		return ret;
    /*销毁锁*/
	if ((_sock->mode == sfpr_socket_mode_server) && (_sock->flag & SFPR_SOCKET_FLAG_MUTEX)){
		ret = sfpr_mutex_destroy(&(_sock->end.server.mutex));
	}
	free(_sock);
	*sock = NULL;
	return ret;
}


sfpr_int_t sfpr_socket_init(sfpr_socket_t *sock, sfpr_socket_protocol_t protocol, sfpr_char_t *host, sfpr_int_t port, sfpr_socket_mode_t mode)
{
	sfpr_int_t ret = SFPR_SUCCESS;
	if (sfpr_socket_mode_server == mode){
		sock->end.server.port = port;
		/*初始化锁*/
	}else{
		sock->end.client.server_host = host;
		sock->end.client.server_port = port;
	}
	sock->mode = mode;
	sock->protocol = protocol;
	sock->error = -1;
	sock->flag = -1;
	return ret;
}

sfpr_int_t sfpr_socket_isvalid(sfpr_socket_t *sock)
{
	if(!sock)
		return -1;
	if(sock->sock < 0)
		return -1;
	return 0;
}

sfpr_int_t sfpr_socket_socket(sfpr_socket_t *sock, sfpr_socket_protocol_t protocol)
{
	sfpr_int_t ret = SFPR_SUCCESS;
	int optvalue;
	int optlen = sizeof(optvalue);
	if (sfpr_socket_protocol_tcp == protocol){
		sock->sock = socket(AF_INET,SOCK_STREAM,0);
	}else{
		sock->sock = socket(AF_INET,SOCK_DGRAM,0);
	}
	
	if (sock->sock < 0){
		#ifdef WIN32
		sock->error = WSAGetLastError();
		#else
		sock->error = errno;		
		#endif
		ret = SFPR_ERROR;
	}
	return ret;
}

sfpr_int_t sfpr_socket_shutdown(sfpr_socket_t *sock, sfpr_sd_type_t how)
{
	sfpr_int_t ret = SFPR_SUCCESS;
	ret = shutdown(sock->sock, how);
	if (ret < 0){
		#ifdef WIN32
		sock->error = WSAGetLastError();
		#else
		sock->error = errno;		
		#endif
	}
	return ret;
}

sfpr_int_t sfpr_socket_close(sfpr_socket_t *sock)
{
	sfpr_int_t ret = SFPR_SUCCESS;
	if (sock->sock != -1){
		#ifdef WIN32
		ret = closesocket(sock->sock);
		#else
		ret = close(sock->sock);		
		#endif
		if(ret < 0){
			#ifdef WIN32
			sock->error = WSAGetLastError();
			#else
			sock->error = errno;		
			#endif
			ret = SFPR_ERROR;
		}
		sock->sock = -1;
	}
	return ret;
}

sfpr_int_t sfpr_socket_nonblock_set(sfpr_socket_t *sock)
{
	sfpr_int_t ret = SFPR_SUCCESS;
	
	sfpr_uint32_t ul = 1; 
	#ifdef WIN32
	ret = ioctlsocket(sock->sock, FIONBIO, &ul); /*设置为非阻塞模式*/
	#else
	ret = ioctl(sock->sock, FIONBIO, ul); /*设置为非阻塞模式*/
	#endif
	if(0 == ret)
		sock->no_block = 1;
	else{
		#ifdef WIN32
		sock->error = WSAGetLastError();
		#else
		sock->error = errno;		
		#endif
	}
	return ret;
}

/**
 * @brief set_block
 */
sfpr_int_t sfpr_socket_block_set(sfpr_socket_t *sock)
{
	sfpr_int_t ret = SFPR_SUCCESS;
	
	sfpr_uint32_t ul = 0; 
	#ifdef WIN32
	ret = ioctlsocket(sock->sock, FIONBIO, &ul); /*设置为阻塞模式*/
	#else
	ret = ioctl(sock->sock, FIONBIO, ul); /*设置为阻塞模式*/
	#endif
	if(0 == ret)
		sock->no_block = 0;
	else{
		#ifdef WIN32
		sock->error = WSAGetLastError();
		#else
		sock->error = errno;		
		#endif
	}
	return ret;
}

sfpr_int_t sfpr_socket_bind(sfpr_socket_t *sock)
{
	sfpr_int_t ret = SFPR_SUCCESS;
	struct sockaddr_in	 saddr;
	memset(&saddr,0,sizeof(saddr));
	
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(sock->end.server.port);
	saddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock->sock, (struct sockaddr*)&saddr, sizeof(saddr)) < 0){
		#ifdef WIN32
		sock->error = WSAGetLastError();
		#else
		sock->error = errno;		
		#endif
		return SFPR_ERROR;
	}
	return ret;
}

sfpr_int_t sfpr_socket_listen(sfpr_socket_t *sock, sfpr_int_t iMaxNum)
{
	sfpr_int_t ret = SFPR_SUCCESS;
	
	if (listen(sock->sock, iMaxNum) < 0){
		#ifdef WIN32
		sock->error = WSAGetLastError();
		#else
		sock->error = errno;		
		#endif
		return SFPR_ERROR;
	}
	return ret;
}

sfpr_int_t sfpr_socket_accept(sfpr_socket_t *sock, sfpr_socket_t **sock_client)
{
	sfpr_int_t ret = SFPR_SUCCESS;
	int optvalue;
	int optlen = sizeof(optvalue);
	/*创建客户端对应的socket结构*/
	sfpr_socket_t *client = NULL;
	
	ret = sfpr_socket_create(&client);
	if(ret != SFPR_SUCCESS)
		return ret;

	memset(&(client->sockaddr),0,sizeof(client->sockaddr));
	client->sockaddr_len = sizeof(client->sockaddr);
	client->sock = accept(sock->sock, (struct sockaddr *)&(client->sockaddr), &(client->sockaddr_len));

	if (client->sock < 0){
		sfpr_socket_close(client);
		sfpr_socket_destroy(&client);
		#ifdef WIN32
		sock->error = WSAGetLastError();
		#else
		sock->error = errno;		
		#endif
		return SFPR_ERROR;
	}

	ret = sfpr_socket_init(client, sock->protocol, NULL, 0, sock->mode);
	if(ret != SFPR_SUCCESS){
		sfpr_socket_close(client);
		sfpr_socket_destroy(&client);
		#ifdef WIN32
		sock->error = WSAGetLastError();
		#else
		sock->error = errno;
		#endif
	}
	*sock_client = client;
	return ret;
}

sfpr_int_t sfpr_socket_connect(sfpr_socket_t *sock)
{	
	sfpr_int_t ret = SFPR_SUCCESS,error_code = 0;
	sfpr_char_t buff[5];
	sfpr_int_t itmp;
	struct sockaddr_in 		saddr;
	memset(&saddr,0,sizeof(saddr));

	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(sock->end.client.server_port);
	saddr.sin_addr.s_addr = inet_addr(sock->end.client.server_host);
	do{
		ret = connect(sock->sock,(struct sockaddr *)&saddr, sizeof(saddr));
	#ifdef WIN32
	}while(SOCKET_ERROR == ret && WSAGetLastError() == WSAEINTR);	
	#else
	}while(ret < 0 && errno == EINTR);
	#endif

	if (ret < 0){
		#if defined WIN32
		error_code = WSAGetLastError();
		if ((error_code == WSAEINPROGRESS || error_code == WSAEALREADY) && sock->no_block && sock->timeout)		
		#else
		if ((errno == EINPROGRESS || errno == EALREADY) && sock->no_block && sock->timeout)
		#endif
		{
			ret = sfpr_socket_wait(sock, 0);
			if (ret != SFPR_SUCCESS){
				ret = SFPR_ERROR;
				#ifdef WIN32
				sock->error = WSAGetLastError();
				#else
				sock->error = errno;
				#endif		
				return ret;
			}else{
				memset(buff,0x0,sizeof(buff));
				ret = sfpr_socket_send(sock, buff, 0, &itmp);
				if(ret < 0){
					#ifdef WIN32
					sock->error = WSAGetLastError();
					#else
					sock->error = errno;
					#endif
				}
				memset(&(sock->sockaddr),0,sizeof(sock->sockaddr));
				sock->sockaddr_len = sizeof(sock->sockaddr);
				getsockname(sock->sock, (struct sockaddr *)&(sock->sockaddr), &(sock->sockaddr_len));
				return ret;
			}
		}
		sock->error = errno;
	}
	memset(&(sock->sockaddr),0,sizeof(sock->sockaddr));
	sock->sockaddr_len = sizeof(sock->sockaddr);
	getsockname(sock->sock, (struct sockaddr *)&(sock->sockaddr), &sock->sockaddr_len);
	return ret;
}


sfpr_int_t sfpr_socket_send(sfpr_socket_t *sock, const sfpr_char_t *data, sfpr_int_t size, sfpr_int_t *iHasSendSize)
{
    sfpr_int_t ret = SFPR_SUCCESS;
	sfpr_int_t iSendCnt = 0;
	sfpr_int_t iHasSendCnt = 0;

	if (sock->sock <= 0){
		*iHasSendSize = 0;
		return SFPR_ERROR;
	}

	/*加锁*/
	if((sock->mode == sfpr_socket_mode_server) && (sock->flag & SFPR_SOCKET_FLAG_MUTEX)){
		sfpr_mutex_lock(&(sock->end.server.mutex));
	}
	while( iHasSendCnt < size){
		do{
			iSendCnt = send(sock->sock, data + iHasSendCnt, size - iHasSendCnt, 0);
		#ifdef WIN32
		}while (iSendCnt == SOCKET_ERROR  && WSAGetLastError() == WSAEINTR);
		#else
		}while (iSendCnt == -1 && errno == EINTR);
		#endif

        if(iSendCnt < 0){
			#if defined WIN32
			if ((WSAGetLastError() == EAGAIN) && sock->no_block && sock->timeout){
			#else
			if ((errno == EAGAIN || errno == EWOULDBLOCK) && sock->no_block && sock->timeout){			
			#endif
				ret = sfpr_socket_wait(sock, 0);
				if (ret != SFPR_SUCCESS){
					ret = SFPR_ERROR;
					#ifdef WIN32
					sock->error = WSAGetLastError();
					#else
					sock->error = errno;
					#endif
					break;
				}
				continue;
			}
			#ifdef WIN32
			sock->error = WSAGetLastError();
			#else
			sock->error = errno;
			#endif
			ret = SFPR_ERROR;
			break;
        } 
		else if(iSendCnt == 0){
			ret = SFPR_CLOSED;
			break;
		}
		else{
            iHasSendCnt += iSendCnt;
        }
    }
	/*解锁*/
	if((sock->mode == sfpr_socket_mode_server) && (sock->flag & SFPR_SOCKET_FLAG_MUTEX)){
		sfpr_mutex_unlock(&(sock->end.server.mutex));
	}
	*iHasSendSize = iHasSendCnt;
    return ret;
}

sfpr_int_t sfpr_socket_sendto(sfpr_socket_t *sock, struct sockaddr_in *to, const sfpr_char_t *data, sfpr_int_t size, sfpr_int_t *iHasSendSize)
{
    sfpr_int_t ret = SFPR_SUCCESS;
	sfpr_int_t iSendCnt = 0;
	sfpr_int_t iHasSendCnt = 0;

	if (sock->sock <= 0){
		*iHasSendSize = 0;
		return SFPR_ERROR;
	}

	if ((sock->mode == sfpr_socket_mode_server) && (sock->flag & SFPR_SOCKET_FLAG_MUTEX)){
		sfpr_mutex_lock(&(sock->end.server.mutex));
	}
	while (iHasSendCnt < size){
	    do{
	        iSendCnt = sendto(sock->sock, data+iHasSendCnt, size-iHasSendCnt, 0, 
	                    (const struct sockaddr*)to, 
	                    sizeof(struct sockaddr_in));
		#ifdef WIN32
		}while (iSendCnt == SOCKET_ERROR  && WSAGetLastError() == WSAEINTR);
		#else
		}while (iSendCnt == -1 && errno == EINTR);
		#endif
		
        if(iSendCnt < 0){
			#if defined WIN32
			if ((WSAGetLastError() == EAGAIN) && sock->no_block && sock->timeout)
			#else
			if ((errno == EAGAIN || errno == EWOULDBLOCK) && sock->no_block && sock->timeout)
			#endif
			{
				ret = sfpr_socket_wait(sock, 0);
				if (ret != SFPR_SUCCESS){
					ret = SFPR_ERROR;
					#ifdef WIN32
					sock->error = WSAGetLastError();
					#else
					sock->error = errno;
					#endif
					break;
				}
				continue;
			}
			#ifdef WIN32
			sock->error = WSAGetLastError();
			#else
			sock->error = errno;
			#endif
			ret = SFPR_ERROR;
			break;
        } 
		else if(iSendCnt == 0){
			ret = SFPR_CLOSED;
			break;
		}
		else{
            iHasSendCnt += iSendCnt;
        }
	}
	if ((sock->mode == sfpr_socket_mode_server) && (sock->flag & SFPR_SOCKET_FLAG_MUTEX)){
		sfpr_mutex_unlock(&(sock->end.server.mutex));
	}
	*iHasSendSize = iHasSendCnt;
    return ret;
}

sfpr_int_t sfpr_socket_recv(sfpr_socket_t *sock, sfpr_char_t *buffer, sfpr_int_t size, sfpr_int_t *iRealRecvSize)
{
    sfpr_int_t ret = SFPR_SUCCESS;
	sfpr_int_t iRecvCnt = 0;
    sfpr_int_t iHasRecvCnt = 0;
	if (sock->sock <= 0){
		*iRealRecvSize = 0;
		return SFPR_ERROR;
	}

	do{
		iRecvCnt = recv(sock->sock, buffer, size,0);
	#ifdef WIN32
	}while (iRecvCnt == SOCKET_ERROR  && WSAGetLastError() == WSAEINTR);
	#else
	}while (iRecvCnt == -1 && errno == EINTR);
	#endif
	
    if (iRecvCnt < 0){
		#if defined WIN32
		if ((WSAGetLastError() == EAGAIN) && sock->no_block && sock->timeout)
		#else
		if ((errno == EAGAIN || errno == EWOULDBLOCK) && sock->no_block && sock->timeout)
		#endif		
		{
			ret = sfpr_socket_wait(sock, 1);
			if (ret != SFPR_SUCCESS){
				ret = SFPR_ERROR;
				#ifdef WIN32
				sock->error = WSAGetLastError();
				#else
				sock->error = errno;
				#endif
			}else{
				do{
					iRecvCnt = recv(sock->sock, buffer, size,0);
				#ifdef WIN32
				}while (iRecvCnt == SOCKET_ERROR  && WSAGetLastError() == WSAEINTR);
				#else
				}while (iRecvCnt == -1 && errno == EINTR);
				#endif

				if(iRecvCnt < 0)
					ret = SFPR_ERROR;
				else if(iRecvCnt == 0)
					ret = SFPR_CLOSED;
				else
					ret = SFPR_SUCCESS;
				#ifdef WIN32
				sock->error = WSAGetLastError();
				#else
				sock->error = errno;
				#endif
			}
		}else{
			#ifdef WIN32
			sock->error = WSAGetLastError();
			#else
			sock->error = errno;
			#endif
			ret = SFPR_ERROR;
		}
	}
	else if(iRecvCnt == 0){
		ret = SFPR_CLOSED;
	}
	*iRealRecvSize = iRecvCnt;
	return ret;
}

sfpr_int_t sfpr_socket_recv_by_size(sfpr_socket_t *sock, sfpr_char_t *buffer, sfpr_int_t size, sfpr_int_t *iRealRecvSize)
{
    sfpr_int_t ret = SFPR_SUCCESS;
	sfpr_int_t iRecvCnt = 0;
    sfpr_int_t iHasRecvCnt = 0;
	
	if (sock->sock <= 0){
		*iRealRecvSize = 0;
		return SFPR_ERROR;
	}

	while (iHasRecvCnt < size){
		do{
			iRecvCnt = recv(sock->sock, buffer+iHasRecvCnt, size-iHasRecvCnt,0);
		#ifdef WIN32
		}while (iRecvCnt == SOCKET_ERROR  && WSAGetLastError() == WSAEINTR);
		#else
		}while (iRecvCnt == -1 && errno == EINTR);
		#endif
		
	    if (iRecvCnt < 0){
			#if defined WIN32
			if ((WSAGetLastError() == EAGAIN) && (sock->no_block) && (sock->timeout > 0))
			#else
			if ((errno == EAGAIN || errno == EWOULDBLOCK) && sock->no_block && sock->timeout)
			#endif
			{
				ret = sfpr_socket_wait(sock, 1);
				if (ret != SFPR_SUCCESS) {
					ret = SFPR_ERROR;
					#ifdef WIN32
					sock->error = WSAGetLastError();
					#else
					sock->error = errno;
					#endif
					break;
				}
				continue;
			}else{
				#ifdef WIN32
				sock->error = WSAGetLastError();
				#else
				sock->error = errno;
				#endif
				ret = SFPR_ERROR;
				break;
			}
		}
		else if (iRecvCnt == 0){
			ret = SFPR_CLOSED;
			break;
		}
		iHasRecvCnt += iRecvCnt;
	}
	*iRealRecvSize = iHasRecvCnt;
	return ret;
}

sfpr_int_t sfpr_socket_recvfrom(sfpr_socket_t *sock, struct sockaddr_in *from, sfpr_char_t *buffer, sfpr_int_t size, sfpr_int_t *iRealRecvSize)
{
    sfpr_int_t ret = SFPR_SUCCESS;
	sfpr_int_t iRecvCnt = 0;
    sfpr_int_t iHasRecvCnt = 0;
	sfpr_int_t rlen;
	rlen = sizeof(struct sockaddr_in);
	
	if (sock->sock <= 0){
		*iRealRecvSize = 0;
		return SFPR_ERROR;
	}
    do{
        iRecvCnt = recvfrom(sock->sock, buffer, size, 0, (struct sockaddr*)from, &rlen);
	#ifdef WIN32
	}while (iRecvCnt == SOCKET_ERROR  && WSAGetLastError() == WSAEINTR);
	#else
	}while (iRecvCnt == -1 && errno == EINTR);
	#endif
	
    if (iRecvCnt < 0){
		#if defined WIN32
		if ((WSAGetLastError() == EAGAIN) && sock->no_block && sock->timeout)
		#else
		if ((errno == EAGAIN || errno == EWOULDBLOCK) && sock->no_block && sock->timeout)
		#endif
		{
			ret = sfpr_socket_wait(sock, 0);
			if (ret != SFPR_SUCCESS){
				ret = SFPR_ERROR;
				#ifdef WIN32
				sock->error = WSAGetLastError();
				#else
				sock->error = errno;
				#endif
			}else{
				do{
					iRecvCnt = recvfrom(sock->sock, buffer, size, 0, 
								  (struct sockaddr*)from, &rlen);
				#ifdef WIN32
				}while (iRecvCnt == SOCKET_ERROR  && WSAGetLastError() == WSAEINTR);
				#else
				}while (iRecvCnt == -1 && errno == EINTR);
				#endif
				if (iRecvCnt < 0)
					ret = SFPR_ERROR;
				else if (iRecvCnt == 0)
					ret = SFPR_CLOSED;
				else
					ret = SFPR_SUCCESS;
				#ifdef WIN32
				sock->error = WSAGetLastError();
				#else
				sock->error = errno;
				#endif
			}
		}else{
			#ifdef WIN32
			sock->error = WSAGetLastError();
			#else
			sock->error = errno;
			#endif
			ret = SFPR_ERROR;
		}
	}
	else if (iRecvCnt == 0){
		ret = SFPR_CLOSED;
	}
	*iRealRecvSize = iRecvCnt;
    return ret;
}

sfpr_int_t sfpr_socket_wait(sfpr_socket_t *sock, sfpr_int_t for_read)
{
    struct timeval timeout;
    sfpr_int_t type = 0;
    sfpr_int_t ret;
	fd_set fds; 
	int maxfdp;

	if(sock->timeout <= 0){
		sock->timeout = 1000;
	}
	timeout.tv_sec = (sock->timeout)/1000;
	timeout.tv_usec = (sock->timeout)%1000*1000;

    #ifdef WIN32
    if(sock->sock <= 0){
		sfpr_time_msleep(timeout);
		return SFPR_TIMEOUT;
    }
    #endif

	FD_ZERO(&fds); 
	FD_SET(sock->sock, &fds); 
	maxfdp = sock->sock + 1; 	

	do{
		if(for_read){
			ret = select(maxfdp,&fds,NULL,NULL,&timeout);
		}
		else{
			ret = select(maxfdp,NULL,&fds,NULL,&timeout);
		}
	#ifdef WIN32
	}while(SOCKET_ERROR == ret && WSAGetLastError() == WSAEINTR);	
	#else
	}while(ret < 0 && errno == EINTR);
	#endif
	if (ret > 0){
		return SFPR_SUCCESS;
	}
    return SFPR_ERROR;
}


sfpr_int_t sfpr_socket_timeout_set(sfpr_socket_t *sock, sfpr_uint32_t timeout)
{
    sfpr_int_t ret;
	sock->timeout = timeout;
	
    return SFPR_SUCCESS;
}

sfpr_int_t sfpr_socket_opt_set(sfpr_socket_t *sock, sfpr_int_t level, sfpr_int_t optname, void *optval, sfpr_int_t optlen)
{
	sfpr_int_t ret;
	ret = setsockopt(sock->sock, level, optname, optval, optlen);

	return ret; 
}

sfpr_int_t sfpr_socket_opt_get(sfpr_socket_t *sock, sfpr_int_t level, sfpr_int_t optname, void *optval, sfpr_int_t *optlen)
{
	sfpr_int_t ret;
	ret = getsockopt(sock->sock, level, optname, optval, optlen);

	return ret; 
}

sfpr_int_t sfpr_socket_hostname_get(sfpr_char_t *host_name, sfpr_int_t size)
{

	if (gethostname(host_name, size) != 0) {
		host_name[0] = '\0';
		return SFPR_ERROR;
	}
    else if (!memchr(host_name, '\0', size)){ 
        host_name[0] = '\0';
		return SFPR_ERROR;
    }
	return SFPR_SUCCESS;
}

sfpr_int_t sfpr_socket_set_flag(sfpr_socket_t *sock, sfpr_int_t flag)
{
	int ret;
	sock->flag |= flag;
	if((sock->mode == sfpr_socket_mode_server) && (sock->flag & SFPR_SOCKET_FLAG_MUTEX)){
		ret = sfpr_mutex_create(&(sock->end.server.mutex), 0);
	}
	return SFPR_SUCCESS;
}

sfpr_int_t sfpr_socket_get_flag(sfpr_socket_t *sock)
{
	return sock->flag;
}

#define IPTOSBUFFERS	32
sfpr_char_t *sfpr_iptos(unsigned long in)
{
	static char output[IPTOSBUFFERS][3*4+3+1];
	static short which = 0;
	unsigned char *p;

	p = (unsigned char *)&in;
	which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
	sprintf(output[which], "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	return output[which];	
}

static int g_byte_order = 0;

sfpr_int_t sfpr_byte_order_check()
{
	sfpr_uint16_t order = 1;
	char *pcheck = (char*)&order;
	if(pcheck[0] == 1){
		return sfpr_little_end;
	}else{
		return sfpr_big_end;
	}
}

unsigned short sfpr_htons(unsigned short inval)
{
	int i;
	unsigned short outval = 0;

	if(g_byte_order == 0){
		g_byte_order = sfpr_byte_order_check();
	}
	
	if(g_byte_order == sfpr_little_end){
		for(i=0; i<2; i++)
			outval = ( outval << 8) + ( ( inval >> ( i * 8)) & 255);
		return outval;
	}else{
		return inval;
	}
}

unsigned long sfpr_htonl(unsigned long inval)
{
	int i;
	unsigned long outval = 0;

	if(g_byte_order == 0){
		g_byte_order = sfpr_byte_order_check();
	}

	if(g_byte_order == sfpr_little_end){	
		for(i=0; i<4; i++)
			outval = ( outval << 8) + ( ( inval >> ( i * 8)) & 255);
		return outval;
	}else{
		return inval;		
	}
}


#ifdef __cplusplus
}
#endif
