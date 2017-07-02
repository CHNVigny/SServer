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

std::string int_to_str(int n)
{
	stringstream ss;
	string s;
	ss<<n;
    ss>>s;
    return s;
}

int main()
{
	string str;
	int n = 123;
	str = int_to_str(123);
	std::cout << str << std::endl;
}