#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <error.h>
#include <pthread.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "./include/tcp.h"
#include "./include/jpg.h"

#define REQ_DATA_SIZE   32
#define HDR_DATA_SIZE   128

//#define DEBUG

extern pthread_mutex_t jpg_mutex;

int client_process(int connfd, struct jpg_buf_t *jpg)
{
	/*int ret;
	char request[REQ_DATA_SIZE] = {0};
	char response[HDR_DATA_SIZE] = {0};

	memset(request, 0, sizeof(request));
	ret = tcp_server_recv(connfd, request, sizeof(request));
	if (ret <= 0)
		return -1;

#ifdef DEBUG
	fprintf(stdout, "server->read: the request is\n%s\n", request);
#endif

	if (strstr(request, "pic") != NULL) {
		int ret;

		pthread_mutex_lock(&jpg_mutex);
		snprintf(response, sizeof(response), "%dlen", jpg->len);
#ifdef DEBUG
		fprintf(stdout, "server->response: the response is: %s\n", response);
#endif
		ret = tcp_server_send_exact_nbytes(connfd, response, sizeof(response));
		if (ret != sizeof(response)) {
			fprintf(stderr, "server->write: send response failed\n");
			return -1;
		}

		ret = tcp_server_send_exact_nbytes(connfd, jpg->buf, jpg->len);
		if (ret != jpg->len) {
			fprintf(stderr, "server->write: send response failed\n");
			return -1;
		}

		pthread_mutex_unlock(&jpg_mutex);
	}

	return 0;*/
	unsigned int piclen = 0;
	unsigned char * buf ;
	char response[20] = {0};
	char msg[3] = {0};
	int total = 0;
	int ret1 = 0;
	
	ret1 = read(connfd,msg,3);
	printf("msg  :  %s\n",msg);
	
	pthread_mutex_lock(&jpg_mutex);
	piclen = jpg->len;
	printf("piclen = %d\n",piclen);
	snprintf(response, sizeof(response), "%dlen", piclen);
	
	buf = (unsigned char *)malloc(piclen);
	memset(buf,0,piclen);
	memcpy(buf,jpg->buf,piclen);
	
	pthread_mutex_unlock(&jpg_mutex);
	
	int ret = write(connfd,response,20);
	printf("ret = %d\n",ret);
	while(piclen>total)
	{
	    ret1 = write(connfd,buf+total,piclen-total);
	    if(ret1 < 0)
	    {
	        break;
	    }
	    total += ret1;
	    printf("total = %d\n",total);
	}
	
	return 0;
}
