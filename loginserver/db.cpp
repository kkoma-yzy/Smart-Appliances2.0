#include<iostream>
#include"db.h"

#define _DEBUG_
using namespace std;



DB::DB()
{
	cout<<"gou zao hanshu"<<endl;
	
	msg1[MSG_BUF_SIZE]={0};
	ret=0;
	sq2=NULL;	
/*	username=NULL;
	pwd=NULL;
	sql1=NULL;
	buf=NULL;
	*/
	zErrMsg=NULL;
}


sqlite3 * DB::sq2;
void  DB::db_init()
{
	cout<<"init db!"<<endl;
	
	ret=sqlite3_open("/root/project/mysqlite",&sq2);
	cout<<"0"<<endl;
	cout<<sq2<<endl;
	if(ret)
	{
		fprintf(stderr,"cannot open database:%s\n",sqlite3_errmsg(sq2));
		sqlite3_close(sq2);
	}
	cout<<"mysqlite create/open success!"<<endl;
	char *sql1=(char *)"CREATE TABLE user1(username text primary key,pwd text not null,seradd text not null,seruser text not null,serpwd text not null);";
	cout<<sql1<<endl;
	sqlite3_exec(sq2,sql1,NULL,NULL,&zErrMsg);
	cout<<"1"<<endl;
	cout<<sq2<<endl;
#ifdef _DEBUG_
	cout<<zErrMsg<<endl;
	sqlite3_free(zErrMsg);
#endif
		
	
	

}



int DB::user_register( int connFd,char *p)


{

	cout<<"2"<<endl;
	
cout<<p<<endl;	
	char *buf1=strdup(p);
	
	char *username1=strsep(&buf1,",");
	char *pwd1=strsep(&buf1,",");
	char *seradd=strsep(&buf1,",");
	char *seruser=strsep(&buf1,",");
	char *serpwd=strsep(&buf1,",");
	cout<<username1<<endl;
	cout<<seradd<<endl;	
	cout<<pwd1<<endl;	
	cout<<seruser<<endl;	
	cout<<serpwd<<endl;	
	char sql[1024]={0};
	
	sprintf(sql,"INSERT INTO 'user1' VALUES('%s','%s','%s','%s','%s');",username1,pwd1,seradd,seruser,serpwd);
	cout<<"wo shi ni ba"<<endl;
	int led=sqlite3_exec(sq2,sql,NULL,NULL,&zErrMsg);
	
#ifdef _DEBUG_
	cout<<zErrMsg<<endl;
	sqlite3_free(zErrMsg);
#endif

	if(led<0)
	{
		sendData(connFd,"register error");
		return -1;
	}
	else
	{
	        cout<<"3"<<endl;
		cout<<"register ok"<<endl;
		char sendmsg[1024]={0};
		sprintf(sendmsg,"register ok,%s,%s,%s",seradd,seruser,serpwd);
	   	
		sendData(connFd,sendmsg);
		
		sleep(2);
		return 0;
	}
}



int DB::user_login( int connFd,char *p)
{
    char *buf=strdup(p);
	char *username=strsep(&buf,",");
	char *pwd=strsep(&buf,",");
	cout<<username<<endl;
	cout<<pwd<<endl;
    char **ppTable=NULL;
    char *errmsg=NULL;
    int   irow=0;
    int  icolumn=0;
    char sql[1024]={0};
    sprintf(sql,"select pwd,seradd,seruser,serpwd from user1 where username='%s';",username);
    cout<<sql<<endl;
    int led=sqlite3_get_table(sq2,sql,&ppTable,&irow,&icolumn,&errmsg);
    if(NULL!=errmsg)
    {
        cout<<"error"<<endl;
        sendData(connFd,"login error");        
        return -1;                         
        
    }
    if(NULL!=ppTable)
    {
        for(int i=0;i<irow+1;i++)
                    {
                        for(int j=0;j<icolumn;j++)
                        {
			
                            if(NULL!=ppTable[i*icolumn+j])
                            {
                                
                                cout<<ppTable[i*icolumn+j];
				cout<<" ";
                            }   
                        }
                        printf("\n");
                   } 
    }
    if(0==strcmp(pwd,ppTable[4]))
    {
	    char sendmsg[1024]={0};
	    
	   sprintf(sendmsg,"login ok,%s,%s,%s",ppTable[5],ppTable[6],ppTable[7]);
	    cout<<sendmsg<<endl;
        ret=sendData(connFd,sendmsg);
                if(ret<0)
               {
                    return -1;
                }
    }
    else
    {
        sendData(connFd,"login error");
        return -1;
    }

  
   	sleep(2); 
	cout<<"login ok"<<endl;
    return 0;
}

int DB::sendData(int connFd,const char* data)
{
	if(connFd<0||NULL==data)
	{
		return -1;
	}
	memset(msg1,0,sizeof(msg1));
	strcpy(msg1,data);
	ret=send(connFd,msg1,sizeof(msg1),0);
	if(ret<0)
	{
		cout<<"send error"<<endl;
		return -1;
	}
	else
	{
		cout<<"send ok"<<endl;

	}

}
 DB::~DB()
{
	delete [] msg1;
	//delete sq;
}
