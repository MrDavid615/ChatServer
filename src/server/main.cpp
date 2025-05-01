#include "chatserver.hpp"
#include "chatservice.hpp"
#include <iostream>
#include <signal.h>
using namespace std;

// 处理服务器ctrl+c结束后，重置user的状态
void resetHandler(int) {
    ChatService::instance()->reset();
    exit(0);
}

int main() {
    signal(SIGINT, resetHandler);

    EventLoop loop;
    InetAddress addr("192.168.61.190", 6000);
    ChatServer server(&loop, addr, "ChatServer");

    server.start();
    loop.loop();

    return 0;
}
