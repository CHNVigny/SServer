#include <iostream>
#include <string>

#include <pthread.h>

#include <arpa/inet.h>
#include <unistd.h>

#include "webDeal.h"

using namespace std;

void* acceptPetition(void* sockID)
{
    int *s=(int *)sockID;
    cout<<*s<<endl;
    webDeal w=webDeal(*s);
    w.start();
    shutdown(*s, SHUT_RDWR);
    close(*s);
    pthread_exit(NULL);
}

int main( int argc, char *argv[] )
{


    int socketDescriptorClient  = 0;
    int socketDescriptor        = 0;
    pthread_t thread            = 0;
    socklen_t socketLength            = 0;
    int socketPort              = 8080;//服务端开放端口
    
    struct sockaddr_in  s_server = { 0 };//声明server的IP
    struct sockaddr_in  s_client = { 0 };//声明client的IP
    
    if((socketDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == -1)//创建server的socket
    {
        cout<<"Error creating the socket\n";
        return -1;
    }
    cout << "creating the socket!\n";
    s_server.sin_family         = AF_INET;//声明协议组为ipv4
    s_server.sin_addr.s_addr    = INADDR_ANY;//绑定server的IP，为任意IP，其实就是本地IP，允许以任何形式访问
    s_server.sin_port           = htons(socketPort);//绑定server的port
    
    socketLength=sizeof(s_server);
 cout << "socketLength "<<socketLength<<endl;
    if(bind(socketDescriptor, (struct sockaddr *) &s_server, socketLength) == -1)
		//将socket与IP、端口号进行绑定
    {
        cout << "Error at associate port and socket!\n";
        close(socketDescriptor);
        return -1;
    }
    cout << "associate port and socket!\n";
    if(listen(socketDescriptor, 5) == -1)//开启监听，5为半连接状态的数目
    {
        cout << "Error at listen!" << endl;
        close(socketDescriptor);
        return -1;
    }

    while(1)
    {
        socketLength = sizeof(struct sockaddr_in);//得到addr长度，从而可以进行数据赋值
        if((socketDescriptorClient = accept(socketDescriptor, (struct sockaddr *) &s_client, &socketLength)) != -1)
        {
        	cout<<"SockedID  dfsafsaf"<<socketDescriptorClient << endl;
            if(pthread_create(&thread, NULL, acceptPetition, (void *) (&socketDescriptorClient)) != 0)
			//开启一个新的进程，其中运行的是acceptPetition函数，在functions中，并且传入参数为服务器的socket
			//父进程继续等待，accept，子进程进入acceptPetittion中进行处理
            {
				cout<<"SockedID  "<<socketDescriptorClient << endl;
                //fprintf(stderr, "SockedID: %d \t %s\n", socketDescriptorClient, "Error creating thread");
            }
        }
    }
    close(socketDescriptor);
    return 0;
}
