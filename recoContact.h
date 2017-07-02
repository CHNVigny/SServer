#ifndef RECOCONTACT_H_
#define RECOCONTACT_H_
#include <errno.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "equipment.h"

#define SCANINS "YZ-RECOSCAN\0"
#define SWITCHONINS "AT+YZSWITCH=1,on,201705011455"
#define SWITCHOFFINS "AT+YZSWITCH=1,off,201705011455"
#define GETINFOINS "AT+YZOUT\r\n"
#define RESULTOK "+OK"
#define RESULTERROR "+ERROR"

#define grantChar 5
#define grantLocalPort 49999
#define grantDstPort 49999
#define ScanDstPort 48899
#define dataSize 128
using namespace std;

	//int grantsocket,retimes=0;

	struct UDPGrantInfo{
		int port;
		struct sockaddr_in addr;
		char data;
	};

	struct UDPScanInfo{
		int port;
		struct sockaddr_in addr;
		char data[dataSize];
	};

	struct TCPinfo{
		int num;
		int port;
		struct sockaddr_in addr;
		char data[dataSize];
	};
class recoContact{
private:
    char UDPGrantAddress[20]="192.168.253.255";
    int UDPGrantCount=3,UDPGrantBeginNum=89,UDPGrantEndNum=86;
public:
	struct UDPGrantInfo UGI;
    void Grant(int grantsocket);
    int UDPSend(int grantsocket,int len);
    int RecoScan(struct smartSwitch *head, char addr[]);
    int RecoGrant();
    int recordReco(char buf[],struct smartSwitch *ss,struct smartSwitch *head);
    int contral(char ins[],struct smartSwitch *ss);
    int recordReco1(char buf[],struct smartSwitch *ss);
    int RecoScan1(struct smartSwitch *head,char addr[]);
    struct smartSwitch* getSmartSwitch(char *sn, struct smartSwitch *ss);
};
#endif




	
