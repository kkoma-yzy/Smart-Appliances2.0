#ifndef _DB_H_
#define _DB_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include"sqlite3.h"
#include<sys/epoll.h>

#define MSG_BUF_SIZE 1024

using namespace std;
class DB{
	public:
		DB();
		void db_init();
		int  user_register( int connFd,char *p);
		int  user_login( int connFd,char *p);
		int sendData( int connFd,const char * data);
		
		~DB();
	private:
		
		char msg1[MSG_BUF_SIZE];
		int ret;
		
	static sqlite3 *sq2;
		
/*		char *username;
		char *sql1;
		char *pwd;
		char *buf;
*/
		char *zErrMsg;
};

#endif
