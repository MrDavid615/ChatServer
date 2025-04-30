#include "json.hpp"
using json = nlohmann::json;

#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

string func1() {
    json js;
    js["msg_typ"] = 2;
    js["from"] = "zhang san";
    js["to"] = "li si";
    js["msg"] = "hello, what are you doing now?";

    string sendBuf = js.dump();
    cout << js << endl;
    cout << sendBuf << endl;
    return sendBuf;
}

string func2() {
    json js;
    js["id"] = {1, 2, 3, 4, 5};
    return js.dump();
}

void func3() {
    string recvBuf = func1();
    json jsbuf = json::parse(recvBuf);
    cout << jsbuf["msg_typ"] << endl;
    cout << jsbuf["from"] << endl;
    cout << jsbuf["to"] << endl;
    cout << jsbuf["msg"] << endl;
}

void func4() {
    string recvBuf = func2();
    json jsBuf = json::parse(recvBuf);
    cout << jsBuf["id"] << endl;
    auto arr = jsBuf["id"];
    cout << arr[2] << endl;
}

int main() {
    func4();
    return 0;
}

