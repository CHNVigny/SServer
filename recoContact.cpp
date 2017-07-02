#include <iostream>
#include "recoContact.h"
void recoContact::Grant(int grantsocket){

	int secret[]={4,0,8,2,4,0,8,2};
    int end=340;
    int i,j=0;
    int num76=204;
	while(j<20){
	    for(i=0;i<num76;i++){
	    	usleep(10000);
	    	UDPSend(grantsocket,76);
	    }
	    
	    printf("76\n");
	    for(i=0;i<UDPGrantCount;i++){
	    	usleep(10000);
	    	UDPSend(grantsocket,UDPGrantBeginNum);
	    }
	    printf("89\n");
        
	    int serectLen=sizeof(secret)/sizeof(int);
	    for(i=0;i<serectLen;i++){
	    	usleep(10000);
	    	UDPSend(grantsocket,secret[i]+124);
	    }
	    printf("mima\n");
       
		for(i=0;i<UDPGrantCount;i++){
			usleep(10000);
	    	UDPSend(grantsocket,UDPGrantEndNum);
	    }
	    printf("86\n");
		for(i=0;i<UDPGrantCount;i++){
			usleep(10000);
	    	UDPSend(grantsocket,end);
	    }
	    printf("340\n");
	    j++;
	}
//	retimes++;
//	alarm(1);
}
int recoContact::RecoScan1(struct smartSwitch *head,char addr[]){
    struct UDPScanInfo scan;
    int so_broadcast=1;
    int scanSocket= socket(AF_INET, SOCK_DGRAM, 0);//申请一个udp网络套接字
    setsockopt(scanSocket,SOL_SOCKET,SO_BROADCAST,&so_broadcast,sizeof(so_broadcast));
    if (scanSocket != -1) {
        printf("Create a socket with fd: %d\n", scanSocket);
    } else {
        fprintf(stderr, "Fail to create a socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in localAddr;//声明地址
    
    localAddr.sin_family = AF_INET;//ipv4
    localAddr.sin_port = htons(48899);//绑定本地端口
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);//本地地址绑定
    memset(localAddr.sin_zero, '\0', sizeof localAddr.sin_zero);
    if (bind(scanSocket, (const struct sockaddr *) &localAddr, sizeof localAddr) == 0) {
        printf("Bind socket to 48899   hello\n");
    } else {
        fprintf(stderr, "Fail to bind the socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
   
    struct sockaddr_in srv_addr;
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(48899);
    srv_addr.sin_addr.s_addr = inet_addr("192.168.253.255");//"192.168.43.255");
    memset(srv_addr.sin_zero, '\0', sizeof srv_addr.sin_zero);
    
    char buf1[]=SCANINS;
    printf("%s\n",buf1);
    //fgets(buf, 512, stdin);
    
    //buf[strlen(buf) - 1] = 0;
    //int add=sizeof(scan.addr);
    ssize_t send_size = sendto(scanSocket, buf1, strlen(buf1), 0,
                               (const struct sockaddr *) &(srv_addr),
                               sizeof srv_addr);




    /*scan.port=ScanDstPort;//
    scan.addr.sin_family = AF_INET;
    scan.addr.sin_port = htons(48899);
    scan.addr.sin_addr.s_addr = htonl("192.168.43.255");//UDPGrantAddress);
    memset(scan.addr.sin_zero, '\0', sizeof scan.addr.sin_zero);
    char ins[]=SCANINS;
    printf("%s\n",ins);
    ssize_t send_size = sendto(scanSocket, ins,strlen(ins), 0,
                               (const struct sockaddr *) &(scan.addr),
                               sizeof scan.addr);*/
    if(send_size>0){
        printf("%s\n",buf1);
    }
    else{
        printf("%d  %s\n",send_size,buf1 );
    }
    
    char buf[512];
    struct sockaddr_in client_addr;
    socklen_t client_addrlen=sizeof(client_addr);
    bzero(&client_addr, sizeof(struct sockaddr_in));  
    client_addr.sin_family = AF_INET;  
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
    client_addr.sin_port = htons(48899);  
    ssize_t recv_size =
        recvfrom(scanSocket, buf, 512, 0, (struct sockaddr *) &client_addr,
                 &client_addrlen);
    printf("%s\n",buf);
    recv_size =
        recvfrom(scanSocket, buf, 512, 0, (struct sockaddr *) &client_addr,
                 &client_addrlen);
    printf("%s\n",buf);
    close(scanSocket);
}


int recoContact::RecoScan(struct smartSwitch *head,char addr[]){
    cout << "scaning..." <<endl;
	struct UDPScanInfo scan;
	int so_broadcast=1;
	int scanSocket= socket(AF_INET, SOCK_DGRAM, 0);//申请一个udp网络套接字
	setsockopt(scanSocket,SOL_SOCKET,SO_BROADCAST,&so_broadcast,sizeof(so_broadcast));//设置允许广播
	if (scanSocket != -1) {
        printf("Create a socket with fd: %d\n", scanSocket);
        cout << "11111111" <<endl;
    } else {
        fprintf(stderr, "Fail to create a socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in localAddr;//声明地址
    scan.port=ScanDstPort;
    localAddr.sin_family = AF_INET;//ipv4
    localAddr.sin_port = htons(48899);//绑定本地端口
    localAddr.sin_addr.s_addr =  htonl(INADDR_ANY);//本地地址绑定
    memset(localAddr.sin_zero, '\0', sizeof localAddr.sin_zero);
    if (bind(scanSocket, (const struct sockaddr *) &localAddr, sizeof localAddr) == 0) {
        printf("Bind socket to 48899   hello\n");
    } else {
        fprintf(stderr, "Fail to bind the socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    //printf("%s\n", addr);
    scan.addr.sin_family = AF_INET;
    scan.addr.sin_port = htons(48899);
    scan.addr.sin_addr.s_addr =  inet_addr(addr);//UDPGrantAddress);
    memset(scan.addr.sin_zero, '\0', sizeof scan.addr.sin_zero);
    char ins[]=SCANINS;
    //printf("sending:%s\n",ins);
    ssize_t send_size;
    for (int i=0;i<3;i++){
	send_size = sendto(scanSocket, ins,strlen(ins), 0,
                               (const struct sockaddr *) &(scan.addr),
                               sizeof scan.addr);
	}
	if(send_size>0){
		//printf("send:ins%s\n",ins);
	}
	else{
		printf("%d  %s\n",send_size,ins );
	}
	
	char buf[512];
	struct sockaddr_in client_addr;
	int client_addrlen=sizeof(client_addr);
    bzero(&client_addr, sizeof(struct sockaddr_in));  
    client_addr.sin_family = AF_INET;  
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
    client_addr.sin_port = htons(48899);
    int time=0;
    struct smartSwitch *p=head,*q;
    struct timeval tv_out;
    tv_out.tv_sec = 3;//等待10秒
    tv_out.tv_usec = 0;
    socklen_t lennnn=sizeof client_addr;
    setsockopt(scanSocket,SOL_SOCKET,SO_RCVTIMEO,&tv_out, sizeof(tv_out));
    while(1){
		ssize_t recv_size =
			recvfrom(scanSocket, buf, 512, 0, (struct sockaddr *) &client_addr,
					 &lennnn);
		
		if(recv_size == -1 && errno == EAGAIN)
			break;
		q=(struct smartSwitch *)malloc(sizeof(struct smartSwitch));
		printf("%s\n", buf);
		if(time == 0){
			if(this->recordReco(buf, p, head)==0){
				time=1;
			}
		}
		else{
			if(this->recordReco(buf, q, head)==0){
				p->next=q;
				p=q;
			}
		}
    //printf("t::%d\n",time );
	//memset(buf,"\0",512);
	}
	close(scanSocket);
    return 1;
}



int recoContact::RecoGrant(){
    int so_broadcast=1;
	int grantsocket= socket(AF_INET, SOCK_DGRAM, 0);//申请一个udp网络套接字
	if (grantsocket != -1) {
        printf("Create a socket with fd: %d\n", grantsocket);
    } else {
        fprintf(stderr, "Fail to create a socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in localAddr;//声明地址
    localAddr.sin_family = AF_INET;//ipv4
    localAddr.sin_port = htons(grantLocalPort);//绑定本地端口
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);//本地地址绑定
    memset(localAddr.sin_zero, '\0', sizeof localAddr.sin_zero);
    setsockopt(grantsocket,SOL_SOCKET,SO_BROADCAST,&so_broadcast,sizeof(so_broadcast));//设置允许广播
    if (bind(grantsocket, (const struct sockaddr *) &localAddr, sizeof localAddr) == 0) {
        printf("Bind socket to 49999\n");
    } else {
        fprintf(stderr, "Fail to bind the socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
	//signal(SIGALRM,Grant);	//闹钟警报
	UGI.port=grantLocalPort;//
    UGI.addr.sin_family = AF_INET;
    UGI.addr.sin_port = htons(grantDstPort);
    UGI.addr.sin_addr.s_addr = inet_addr(this->UDPGrantAddress);
    memset(UGI.addr.sin_zero, '\0', sizeof UGI.addr.sin_zero);
    //此处需要计算得到密码，与密码之后的结束值，即之前的340
	Grant(grantsocket);
    char buf[512];
    struct sockaddr_in client_addr;
    socklen_t client_addrlen;
	memset(buf, 0, sizeof buf);
	int time=0;
	struct timeval tv_out;
	tv_out.tv_sec = 10;//等待10秒
    tv_out.tv_usec = 0;
	setsockopt(grantsocket,SOL_SOCKET,SO_RCVTIMEO,&tv_out, sizeof(tv_out));
	while(1){
	    ssize_t recv_size =
	    recvfrom(grantsocket, buf, 512, 0, (struct sockaddr *) &client_addr,
	             &client_addrlen);
		if(recv_size==-1&&errno==EAGAIN){
			//pthread_exit(ret);
			close(grantsocket);
			return -1;
		}
	    if(recv_size==25){
	    	time++;
	    	printf("shoudao 25\n");
	    }
	    if(time>=5){
			//pthread_exit(ret);
			close(grantsocket);
	    	return 0;
	    }
	}
}


//建立函数进行发送UDP报文，参数分别为socket，udp传输信息，长度len，即传输信息
int recoContact::UDPSend(int grantsocket,int len){
	char* data;
	data=(char*)malloc(len);
	memset(data, 5, len);
	ssize_t send_size = sendto(grantsocket, data, len, 0,
                               (const struct sockaddr *) &(UGI.addr),
                               sizeof UGI.addr);
	if(send_size>0)return 0;
	else return -1;
}


int recoContact::recordReco(char buf[], struct smartSwitch *ss, struct smartSwitch *head){
//printf("buf  %s\n",buf );
    char* delim=",";//分隔符字符串
    char* ip=strtok(buf,delim);//第一次调用strtok
    char *mac,*sn;
    char* flag;
	struct in_addr addr; 
	if(ip==NULL)
		return -1;
    int isIp = inet_pton(AF_INET, ip, &addr);  
    if(isIp<=0){
    	return -1;
    }
    mac=strtok(NULL,delim);
    if(mac==NULL||strlen(mac)!=12){
    	return -1;
    }
   //printf("ip  %s  mac  %s\n",ip,mac );
    sn=strtok(NULL,delim);
    //printf("sn  %s\n",sn);
    if(sn==NULL||strlen(sn)>12){
    	return -1;
    }
    //printf("sn  %s\n",sn);

    ss->addr.sin_family = AF_INET;//ipv4
    ss->addr.sin_port = htons(8899);//绑定本地端口
    //printf("%s\n",ip);
    ss->addr.sin_addr.s_addr = inet_addr(ip);//本地地址绑定
    memset(ss->addr.sin_zero, '\0', sizeof ss->addr.sin_zero);

    strcpy(ss->mac, mac);
    strcpy(ss->sn, sn);

    ip=strtok(NULL,delim);
    //printf("flag  %s",ip);
    if(ip==NULL||strlen(ip)>1){
    	return -1;
    }
    //printf("flag  %s",ip);
    ip=strtok(NULL,delim);
    if(ip==NULL||strlen(ip)>1||(ip[0]!='0'&&ip[0]!='1')){
    	return -1;
    }
    //printf("flag  %s",ip);
    if(ip[0]=='1')ss->state=1;
    else ss->state=0;
    ss->next=NULL;
    struct smartSwitch *p=head;
    while(p!=NULL){
    	if(strcmp(p->sn,sn)==0){
    		return -1;
    	}
    	p=p->next;
    }
    
    return 0;
}



int recoContact::contral(char ins[],struct smartSwitch *ss){
    int s = socket(AF_INET, SOCK_STREAM, 0);
    char buf[100];
    if (s != -1) {
        printf("Create a socket with fd: %d\n", s);
    } else {
        fprintf(stderr, "Fail to create a socket: %s\n", strerror(errno));
        //exit(EXIT_FAILURE);
        return 0;
    }
    memset(ss->addr.sin_zero, '\0', sizeof ss->addr.sin_zero);
    if (connect(s, (const struct sockaddr *) &(ss->addr), sizeof ss->addr) ==0) {
        printf("Connect to the server\n");
    } else {
        fprintf(stderr, "Fail to connect to the server: %s\n",
                strerror(errno));
        //exit(EXIT_FAILURE);
        return 0;
    }
    ssize_t send_size = send( s,ins, strlen(ins), 0);
    ssize_t recv_size = recv(s,buf, 100, 0);
    buf[recv_size]='\0';
    if(recv_size>10){
        if(this->recordReco1(buf, ss)==0)
            return 0;
        else
            return -1;
    }
    else{
        if(ins[3]=='N'||ins[3]=='n'){
            if(strcmp(buf, "+ok")==0){
                ss->state=1;
                return 0;
            }
            else{
                return -1;
            }
        }
        else if(ins[3]=='F'||ins[3]=='f'){
            if(strcmp(buf,"+ok")==0){
                ss->state=0;
                return 0;
            }
            else{
                return -1;
            }
        }
        else{
            return -1;
        }
        
    }
    
}


//1118-1896-8545-7524-8021-7867

int recoContact::recordReco1(char buf[],struct smartSwitch *ss){

    //printf("info  %s\n",buf );
    char* delim="=";//·Ö¸ô·û×Ö·û´®
    char* I=strtok(buf,delim);//µÚÒ»´Îµ÷ÓÃstrtok
    //printf("%s\n",I);
    I=strtok(NULL,delim);
    //printf("%s\n",I);
    delim=",";
    I=strtok(I,delim);
    char *U,*P,*E;
    int tmp;
    //printf("%s\n",I);
    tmp=atoi(I);
    if(tmp==0){
        return -1;
    }
    ss->I=tmp;
    U=strtok(NULL,delim);
    tmp=atoi(U);
    if(tmp==0){
        return -1;
    }
    ss->U=tmp;
    
    P=strtok(NULL,delim);
    tmp=atoi(P);
    if(tmp==0){
        return -1;
    }
    P=strtok(NULL,delim);
    tmp=atoi(P);
    if(tmp==0){
        return -1;
    }
    ss->P=tmp;

    P=strtok(NULL,delim);

    tmp=atoi(P);
   
    P=strtok(NULL,delim);
    //printf("%s\n",P );
    tmp=atoi(P);
    if(tmp==0){
        return -1;
    }
    ss->E=tmp;

    return 0;
}

struct smartSwitch *getSmartSwitch(char *sn, struct smartSwitch *ss){
    struct smartSwitch *head;
    for (head = ss; ;head = head->next)
    {
        if(strcmp(head->sn, sn) == 0){
            return head;
        }
    }
}
