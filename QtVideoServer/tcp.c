#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#define MAX_BACKLOG	1024

int tcp_server_init(const char *ip, const char *port)
{
	int ret;
	int listenfd;
	int opt = 1;
	struct sockaddr_in srvaddr;

	memset(&srvaddr, 0, sizeof(struct sockaddr_in));
	srvaddr.sin_family = AF_INET;

	if (ip != NULL) {
		ret = inet_pton(AF_INET, ip, &srvaddr.sin_addr);
		if (ret != 1) {
			fprintf(stderr, "server->ip: ip is error\n");
			return -1;
		}
	} else
		srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (port != NULL)
		srvaddr.sin_port = htons(atoi(port));
	else {
		fprintf(stderr, "server->port: port must be assigned\n");
		return -1;
	}

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1) {
		perror("server->socket");
		return -1;
	}
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	ret = bind(listenfd, (struct sockaddr *)&srvaddr, sizeof(struct sockaddr_in));
	if (ret == -1) {
		perror("server->bind");
		close(listenfd);
		return -1;
	}

	ret = listen(listenfd, MAX_BACKLOG);
	if (ret == -1) {
		perror("server->listen");
		close(listenfd);
		return -1;
	}

	return listenfd;
}

int tcp_server_wait_connect(int listenfd)
{
	int connfd;
	socklen_t addrlen;
	struct sockaddr_in cliaddr;

	addrlen = sizeof(struct sockaddr_in);
	memset(&cliaddr, 0, sizeof(struct sockaddr_in));
	connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &addrlen);
	if (connfd == -1) {
		perror("server->accept");
		return -1;
	}

	return connfd;
}

ssize_t tcp_server_recv(int connfd, void *buf, size_t count)
{
	ssize_t ret;

	assert(buf != NULL);

	ret = read(connfd, buf, count);
	if (ret == -1) {
		perror("server->read");
		return -1;
	} else if (ret == 0) {
		fprintf(stderr, "server->read: end-of-file\n");
		return 0;
	} else
		return ret;
}

ssize_t tcp_server_send(int connfd, const void *buf, size_t count)
{
	ssize_t ret;

	assert(buf != NULL);

	ret = write(connfd, buf, count);
	if (ret == -1) {
		perror("server->read");
		return -1;
	} else
		return ret;
}

ssize_t tcp_server_recv_exact_nbytes(int connfd, void *buf, size_t count)
{
	ssize_t ret;
	ssize_t total = 0;

	assert(buf != NULL);

	while (total != count) {
		ret = read(connfd, buf + total, count - total);
		if (ret == -1) {
			perror("server->read");
			return -1;
		} else if (ret == 0) {
			fprintf(stderr, "server->read: end-of-file\n");
			return total;
		} else
			total += ret;
	}

	return total;
}

ssize_t tcp_server_send_exact_nbytes(int connfd, const void *buf, size_t count)
{
	ssize_t ret;
	ssize_t total = 0;

	assert(buf != NULL);

	while (total != count) {
		ret = write(connfd, buf + total, count - total);
		if (ret == -1) {
			perror("server->write");
			return total;
		} else
			total += ret;
	}

	return total;
}

int tcp_server_disconnect(int connfd)
{
	if (close(connfd)) {
		perror("server->close");
		return -1;
	}

	return 0;
}

int tcp_server_exit(int listenfd)
{
	if (close(listenfd)) {
		perror("server->close");
		return -1;
	}

	return 0;
}

int tcp_client_init(void)
{
	int sockfd;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("client->socket");
		return -1;
	}

	return sockfd;
}

int tcp_client_connect(int sockfd, const char *ip, const char *port)
{
	int ret;
	struct sockaddr_in srvaddr;

	memset(&srvaddr, 0, sizeof(struct sockaddr_in));
	srvaddr.sin_family = AF_INET;

	if (ip != NULL) {
		ret = inet_pton(AF_INET, ip, &srvaddr.sin_addr);
		if (ret != 1) {
			fprintf(stderr, "client->ip: server ip is error\n");
			return -1; 
		}
	} else
		fprintf(stderr, "client->ip: server ip must be assigned\n");

	if (port != NULL)
		srvaddr.sin_port = htons(atoi(port));
	else {
		fprintf(stderr, "client->port: server port must be assigned\n");
		return -1; 
	}

	ret = connect(sockfd, (struct sockaddr *)&srvaddr, sizeof(struct sockaddr_in));
	if (ret == -1) {
		perror("client->connect");
		return -1;
	}

	return 0;
}

ssize_t tcp_client_recv(int sockfd, void *buf, size_t count)
{
	ssize_t ret;

	assert(buf != NULL);

	ret = read(sockfd, buf, count);
	if (ret == -1) {
		perror("server->read");
		return -1;
	} else if (ret == 0) {
		fprintf(stderr, "server->read: end-of-file\n");
		return 0;
	} else
		return ret;
}

ssize_t tcp_client_send(int sockfd, const void *buf, size_t count)
{
	ssize_t ret;

	assert(buf != NULL);

	ret = write(sockfd, buf, count);
	if (ret == -1) {
		perror("server->read");
		return -1;
	} else
		return ret;
}

ssize_t tcp_client_recv_exact_nbytes(int sockfd, void *buf, size_t count)
{
	ssize_t ret;
	ssize_t total = 0;

	assert(buf != NULL);

	while (total != count) {
		ret = read(sockfd, buf + total, count - total);
		if (ret == -1) {
			perror("server->read");
			return -1;
		} else if (ret == 0) {
			fprintf(stderr, "server->read: end-of-file\n");
			return total;
		} else
			total += ret;
	}

	return total;
}

ssize_t tcp_client_send_exact_nbytes(int sockfd, const void *buf, size_t count)
{
	ssize_t ret;
	ssize_t total = 0;

	assert(buf != NULL);

	while (total != count) {
		ret = write(sockfd, buf + total, count - total);
		if (ret == -1) {
			perror("server->read");
			return total;
		} else
			total += ret;
	}

	return total;
}

int tcp_client_exit(int sockfd)
{
	if (close(sockfd)) {
		perror("client->close");
		return -1;
	}

	return 0;
}
