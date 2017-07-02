#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <memory.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cassert>
#include <sstream>
#include "webDeal.h"
#include "webUrl.h"
#include "equipment.h"
#include "recoContact.h"
#include "json.h"

using namespace std;

void webDeal::sendheader(int client)  
{  
    char buf[100];
    //(void)filename;  /* could use filename to determine file type */  
  
    /*正常的 HTTP header */  
    strcpy(buf, "HTTP/1.0 200 OK\r\n");  
    send(client, buf, strlen(buf), 0);  
    /*服务器信息*/  
    strcpy(buf, "Server: jdbhttpd/0.1.0\r\n");  
    send(client, buf, strlen(buf), 0);  
    sprintf(buf, "Content-Type: text/html\r\n");  
    send(client, buf, strlen(buf), 0);  
    strcpy(buf, "\r\n");  
    send(client, buf, strlen(buf), 0);  
}  
  

std::string int_to_str(int n)
{
	stringstream ss;
	string s;
	ss<<n;
    ss>>s;
    return s;
}

int webDeal::start(){
	struct headers hdr;
	getHeader(&hdr);
	string uri=hdr.uri;
	webUrl url=webUrl(uri);
	string type=url.getValue("type");
	cout<<uri<<endl;
    int result = -1;
    Json::FastWriter writer;
	if(type=="reco"){
        //ip/reco?command=on
        recoContact reco;
        struct smartSwitch ss;
        int i=reco.RecoScan(&ss,"192.168.2.255");
        //cout << "i:" << i << endl;
        //cout << "ip:" << ss.addr.sin_addr.s_addr << endl;
        string command = url.getValue("command");
        if (command == "switch"){
        	//std::cout << "json_str" << std::endl;
            if(ss.state == 0){
                result = reco.contral(SWITCHONINS, &ss);
            }
            else if(ss.state == 1){
                result = reco.contral(SWITCHOFFINS, &ss);
            }
            Json::Value response;
            if(result == -1){
            	response["result"] = "success";
            }
            else{
            	response["result"] = "error";
            }
            std::string json_str = writer.write(response);
            int len = json_str.length();
            const char *json = json_str.c_str();
            std::cout << json_str << std::endl;
            printf("%s\n", json);
			this->sendheader(sockID);
            send(sockID, json,strlen(json),0);
        }
        /*
        else if(command == "off"){
        	//cout << "commond off" <<endl;
            
            //cout << "result:" << result <<endl;
            Json::Value response;
            if(result == 0){
            	response["result"] = "success";
            }
            else{
            	response["result"] = "error";
            }
            std::string json_str = writer.write(response);
            int len = json_str.length();
            const char *json = json_str.c_str();
            std::cout << json_str << std::endl;
            printf("%s\n", json);
            send(sockID, json, len, 0);
        }
        */
        else if(command == "state"){
            result = reco.contral(GETINFOINS, &ss);
	    std::cout << result << endl;
            Json::Value response;
            if(result == 0){
            	response["result"] = "success";
            	int I = ss.I;
            	int U = ss.U;
            	int P = ss.P;
            	int E = ss.E;
                int state = ss.state;
            	response["I"] = I;
          	response["U"] = U;
          	response["P"] = P;
          	response["E"] = E;
                response["state"] = state;
            }
            else{
            	response["result"] = "error";
            }
            std::string json_str = writer.write(response);
            int len = json_str.length();
            const char *json = json_str.c_str();
            std::cout << json_str << std::endl;
            printf("%s\n", json);
	    this->sendheader(sockID);
            send(sockID, json, len, 0);
        }
        else{
            result = -1;
        }

	}
	else if(type == "camera"){
		send(sockID, "camera", 6, 0);
	}
	else if(type == "state"){
		send(sockID, "state", 5, 0);
	}
	else if(type == "device")//get device list
	{
		send(sockID, "device", 6, 0);
	}
	else{
		send(sockID, "device", 6, 0);
	}
}
void webDeal::getHeader(struct headers *hdr)
{
    int i = 0, j = 0;
    char c = '\n';

    memset(hdr, 0, sizeof(struct headers));
	//printf("memset\n");
    //getcwd(hdr->uri, HDR_URI_SZ);

    for(i = 0; (i < HDR_METHOD_SZ) && (recv(sockID, &c, 1, 0) > 0) && (c != ' ' && c != '\n'); i++)
		//每次取得一个字节，分别进行赋值，根据http协议进行处理，首先是访问方式，一般为get，之后是访问链接长度，最后为访问版本号
    {
        if(c != '\r')
        {
            hdr->method[i] = c;
        }
    }
    for(i = 0, j = i; (i < HDR_URI_SZ) && (recv(sockID, &c, 1, 0) > 0) && (c != ' ' && c != '\n'); i++)
    {
        if(c != '\r')
        {
            hdr->uri[i] = c;
        }
    }

    for(i = 0; (i < HDR_VERSION_SZ) && (recv(sockID, &c, 1, 0) > 0) && (c != ' ' && c != '\n'); i++)
    {
        if(c != '\r')
        {
            hdr->version[i] = c;
        }
    }

    printf("HTTP version: %s\n", hdr->version);
    printf("HTTP request: %s\n", hdr->method);
    printf("HTTP uri:     %s\n", hdr->uri);
}

