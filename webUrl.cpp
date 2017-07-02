#include "webUrl.h"
#include <iostream>

    webUrl::webUrl(string url){
        int n,m;
        cout << url << "\n";
        if((n=url.find('?',1))!=string::npos){
            m_map.insert(pair<string,string>("type",url.substr(1,n-1)));
        }
        else{
            m_map.insert(pair<string,string>("type",url.substr(1,url.length()-1)));
        }
        m=n+1;
        while((n=url.find('&',m))!=string::npos){
            string ss=url.substr(m,n-m);
            int p,q;
            if((p=ss.find('=',0))!=string::npos){
                m_map.insert(pair<string,string>(ss.substr(0,p),ss.substr(p+1,(ss.length()-p))));
            }
            else{
                continue;
            }
            m=n+1;
        }
        int p;
            string ss=url.substr(m,url.length()-m);
            if((p=ss.find('=',0))!=string::npos){
                m_map.insert(pair<string,string>(ss.substr(0,p),ss.substr(p+1,ss.length()-p)));
            }
            m=n+1;
        cout<<getValue("command")<<endl;
    }

    string webUrl::getValue(string request){
         return m_map[request];
     }
