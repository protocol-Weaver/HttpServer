#include <iostream>
#include  "HttpServer.h"
using namespace std;
using namespace http;

int main()
{
    TcpServer server = TcpServer("0.0.0.0",8080);
    return 0;
}