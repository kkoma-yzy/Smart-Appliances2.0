#ifndef LOGINSERVER_H
#define LOGINSERVER_H

#include<unistd.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<sys/stat.h>
#include<string.h>
#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<stdarg.h>
#include<errno.h>
#include"locker.h"
#include"db.h"

class loginserver
{
public:

	static const int READ_BUFFER_SIZE = 1024;
	static const int WRITE_BUFFER_SIZE = 1024;
public:
	loginserver();
	~loginserver();
public:
	void init(int sockfd,const sockaddr_in &addr,DB *dbp);
	void close_conn(bool real_close = true);
	void process();
	bool read();
	
/*private:
	//void init();
	void login();
	void user_register();*/
public:
	static int m_epollfd;
	static int m_user_count;
private:
	int m_sockfd;
	sockaddr_in m_address;
	char m_read_buf[READ_BUFFER_SIZE];
	char m_write_buf[WRITE_BUFFER_SIZE];
	DB *mydbp;

};

#endif
