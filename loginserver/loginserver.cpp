#include"loginserver.h"
#include<iostream>
    
using namespace std;
int setnonblocking ( int fd)
{
	int old_option = fcntl(fd,F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd,F_SETFL,new_option);
	return old_option;
	
}

void addfd(int epollfd,int fd,bool one_shot)
{
	epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
	if(one_shot)
	{
		event.events |= EPOLLONESHOT;
	}
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
	setnonblocking(fd);
}

void removefd(int epollfd,int fd)
{
	epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
	cout<<" i have"<<endl;	
	close(fd);
}

void modfd(int epollfd,int fd,int ev)
{
	epoll_event event;
	event.data.fd = fd;
	event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
	epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
	
}

int loginserver::m_user_count = 0;
int loginserver::m_epollfd = -1;

void loginserver::close_conn(bool real_close)
{
	if(real_close && (m_sockfd != -1))
	{
		removefd(m_epollfd,m_sockfd);
		cout<<"close ceshi"<<endl;
		m_sockfd = -1;
		m_user_count--;
		
	}
}


void loginserver::init(int sockfd,const sockaddr_in &addr,DB *dbp)
{
	m_sockfd = sockfd;
	m_address = addr;
	mydbp=dbp;
	int reuse = 1;
	setsockopt(m_sockfd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
	addfd(m_epollfd,sockfd,true);
	m_user_count++;
	
	
}

loginserver::loginserver()
{
	
}

loginserver::~loginserver()
{
	delete [] m_read_buf;
	delete [] m_write_buf;
}

bool loginserver::read()
{
	int ret=recv(m_sockfd,m_read_buf,READ_BUFFER_SIZE,0);
	if(ret<0)
	{
		return false;
	}
	else if(ret==0)
	{
		return false;
	}
	else 
	{
		return true;
	}
}



void loginserver::process()
{
	char *buf=NULL;
	char *sign=NULL;
	char *mess=NULL;
	buf=strdup(m_read_buf);
	sign=strsep(&buf,",");
	mess=buf;
	if(strcmp(sign,"register")==0)
	{
		int ret=mydbp->user_register((const int)m_sockfd,mess);
		if(ret==0)
		{
			cout<<ret<<endl;
			close_conn(true);
		}
			
	}
	else if(strcmp(sign,"login")==0)
	{
		 int ret=mydbp->user_login((const int)m_sockfd,mess);
		if(ret==0)
		 {
		 	close_conn(true);
		 }
				
	}

}




















