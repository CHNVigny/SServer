#ifndef _WEBDEAL_H_
    #define _WEBDEAL_H_
    #define HDR_URI_SZ      1024
    #define HDR_METHOD_SZ   5
    #define HDR_VERSION_SZ  10

    #define MAX_BUFFER      1024

struct headers
{
    char version[HDR_VERSION_SZ];//版本号为10个字节
    char method[HDR_METHOD_SZ];//访问方式为5个字节，包括get，post，put等
    char uri[HDR_URI_SZ];//访问链接，即http：www.baidu.com/abcdefg，/后的那些数据
};

class webDeal{
	private:
		int sockID;
	public:
		webDeal(int socketID){
			this->sockID=socketID;
		}
		void sendheader(int client);  
		int start();
		void getHeader(struct headers *hdr);
};
#endif
