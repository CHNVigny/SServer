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
#define ISspace(x) isspace((int)(x))
#define SERVER_STRING "Server: jdbhttpd/0.1.0\r\n"


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
    char buf[1024];
    int numchars;
    char method[255];
    char url[255];
    char path[512];
    size_t i, j;
    //struct stat st;
    //struct header hd;
    int cgi = 0;      /* becomes true if server decides this is a CGI program */
    char *query_string = NULL;
    memset(hdr, 0, sizeof(struct headers));

    /*得到请求的第一行*/
    numchars = get_line(sockID, buf, sizeof(buf));
    i = 0; j = 0;
    /*把客户端的请求方法存到 method 数组*/
    while (!ISspace(buf[j]) && (i < sizeof(method) - 1))
    {
        hdr->method[i] = buf[j];
        i++; j++;
    }
    hdr->method[i] = '\0';

    /*如果既不是 GET 又不是 POST 则无法处理 */
    if (strcasecmp(hd->method, "GET") && strcasecmp(hd->method, "POST"))
    {
        unimplemented(sockID);
        return;
    }

    /* POST 的时候开启 cgi */
    if (strcasecmp(hdr->method, "POST") == 0)
        cgi = 1;

    /*读取 url 地址*/
    i = 0;
    while (ISspace(buf[j]) && (j < sizeof(buf)))
        j++;
    while (!ISspace(buf[j]) && (i < sizeof(url) - 1) && (j < sizeof(buf)))
    {
        /*存下 url */
        hdr->uri[i] = buf[j];
        i++; j++;
    }
    hdr->uri[i] = '\0';

    //printf("HTTP version: %s\n", hdr->version);
    printf("HTTP request: %s\n", hdr->method);
    printf("HTTP uri:     %s\n", hdr->uri);
}


void webDeal::bad_request(int client)
{
    char buf[1024];

    /*回应客户端错误的 HTTP 请求 */
    sprintf(buf, "HTTP/1.0 400 BAD REQUEST\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "Content-type: text/html\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "<P>Your browser sent a bad request, ");
    send(client, buf, sizeof(buf), 0);
    sprintf(buf, "such as a POST without a Content-Length.\r\n");
    send(client, buf, sizeof(buf), 0);
}


void webDeal::unimplemented(int client)
{
    char buf[1024];

    /* HTTP method 不被支持*/
    sprintf(buf, "HTTP/1.0 501 Method Not Implemented\r\n");
    send(client, buf, strlen(buf), 0);
    /*服务器信息*/
    sprintf(buf, SERVER_STRING);
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "Content-Type: text/html\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<HTML><HEAD><TITLE>Method Not Implemented\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</TITLE></HEAD>\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "<BODY><P>HTTP request method not supported.\r\n");
    send(client, buf, strlen(buf), 0);
    sprintf(buf, "</BODY></HTML>\r\n");
    send(client, buf, strlen(buf), 0);
}


int webDeal::get_line(int sock, char *buf, int size)
{
    int i = 0;
    char c = '\0';
    int n;

    /*把终止条件统一为 \n 换行符，标准化 buf 数组*/
    while ((i < size - 1) && (c != '\n'))
    {
        /*一次仅接收一个字节*/
        n = recv(sock, &c, 1, 0);
        /* DEBUG printf("%02X\n", c); */
        if (n > 0)
        {
            /*收到 \r 则继续接收下个字节，因为换行符可能是 \r\n */
            if (c == '\r')
            {
                /*使用 MSG_PEEK 标志使下一次读取依然可以得到这次读取的内容，可认为接收窗口不滑动*/
                n = recv(sock, &c, 1, MSG_PEEK);
                /* DEBUG printf("%02X\n", c); */
                /*但如果是换行符则把它吸收掉*/
                if ((n > 0) && (c == '\n'))
                    recv(sock, &c, 1, 0);
                else
                    c = '\n';
            }
            /*存到缓冲区*/
            buf[i] = c;
            i++;
        }
        else
            c = '\n';
    }
    buf[i] = '\0';

    /*返回 buf 数组大小*/
    return(i);
}

