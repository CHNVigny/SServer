#ifndef WEB_URL_H_
#define WEB_URL 
 #include <map>
 #include <string>
#include <stdlib.h>
 using namespace std;
 
 class webUrl {
 public:
     webUrl(string url);
     string getValue(string request);
 private:
     string _url;
     map<string,string> m_map;
 };
 #endif
