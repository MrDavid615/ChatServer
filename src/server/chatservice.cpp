#include "chatservice.hpp"
#include "public.hpp"
#include <muduo/base/Logging.h>
#include <string>
#include <vector>
using namespace muduo;
using namespace std;

ChatService* ChatService::instance() {
    static ChatService service;
    return &service;
}

MsgHandler ChatService::getHandler(int msgid) {
    auto it = _msgHandlerMap.find(msgid);
    if(it == _msgHandlerMap.end()) {
        // 返回一个默认处理器
        return [=](const TcpConnectionPtr& conn, json& js, Timestamp time)->void {
            LOG_ERROR << "msgid : " << msgid << " can not find handler!";
        };
    }
    else {
        return _msgHandlerMap[msgid];
    }
}

ChatService::ChatService() {
    this->_msgHandlerMap.emplace(LOGIN_MSG, std::bind(&ChatService::login, this, _1, _2, _3));
    this->_msgHandlerMap.emplace(LOGINOUT_MSG, std::bind(&ChatService::loginOut, this, _1, _2, _3));
    this->_msgHandlerMap.emplace(REG_MSG, std::bind(&ChatService::reg, this, _1, _2, _3));
    this->_msgHandlerMap.emplace(ONE_CHAT_MSG, std::bind(&ChatService::oneChat, this, _1, _2, _3));
    this->_msgHandlerMap.emplace(ADD_FRIEND_MSG, std::bind(&ChatService::addFriend, this, _1, _2, _3));
    this->_msgHandlerMap.emplace(CREATE_GROUP_MSG, std::bind(&ChatService::createGroup, this, _1, _2, _3));
    this->_msgHandlerMap.emplace(ADD_GROUP_MSG, std::bind(&ChatService::addGroup, this, _1, _2, _3));
    this->_msgHandlerMap.emplace(GROUP_CHAT_MSG, std::bind(&ChatService::groupChat, this, _1, _2, _3));
}


// 登录业务 ORM实现 业务操作的都是对象，不直接暴露数据库接口，数据库接口被封装
void ChatService::login(const TcpConnectionPtr& conn, json& js, Timestamp time) {
    int id = js["id"].get<int>();
    string pwd = js["password"];

    User user = _userModel.query(id);
    if(user.getId() != -1 && user.getPwd() == pwd) {
        if(user.getState() == "online") {
            // 禁止重复登录
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 2;
            response["errmsg"] = "用户已经登录";
            conn->send(response.dump());
        }
        else {
            // 登录成功，记录用户连接信息
            {   // 临界代码段
                lock_guard<mutex> lock(_connMutex);
                _userConnMap.emplace(user.getId(), conn);
            }

            // 更新用户状态
            user.setState("online");
            _userModel.updateState(user);
            json response;
            response["msgid"] = LOGIN_MSG_ACK;
            response["errno"] = 0;
            response["id"] = user.getId();
            response["name"] = user.getName();

            // 查询是否有离线消息
            vector<string> vec = _offlineMsgModel.query(user.getId());
            if(!vec.empty()) {
                response["offlinemsg"] = vec;
                // 删除所有离线消息
                _offlineMsgModel.remove(id);
            }

            // 查询用户的好友信息
            vector<User> userVec = _friendModel.query(user.getId());
            if(!userVec.empty()) {
                vector<string> vec2;
                for(auto user : userVec) {
                    json js;
                    js["id"] = user.getId();
                    js["name"] = user.getName();
                    js["state"] = user.getState();
                    vec2.push_back(js.dump());
                }
                response["friends"] = vec2;
            }

            // 查询用户所在群组信息
            vector<Group> groupVec = _groupModel.queryGroups(user.getId());
            if(!groupVec.empty()) {
                vector<string> vec3;
                for(auto group : groupVec) {
                    json js;
                    js["id"] = group.getId();
                    js["groupname"] = group.getName();
                    js["groupdesc"] = group.getDesc();
                    vec3.push_back(js.dump());
                }
                response["groups"] = vec3;
            }

            conn->send(response.dump());
        }
    }
    else {
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["errno"] = 1;
        response["errmsg"] = "用户名或密码错误";
        conn->send(response.dump());
    }
}

void ChatService::loginOut(const TcpConnectionPtr& conn, json& js, Timestamp time) {
    int userid = js["id"].get<int>();
    User user(userid);
    _userModel.updateState(user);
    {
        lock_guard<mutex> lock(_connMutex);
        _userConnMap.erase(userid);
    }
    // conn->connectDestroyed();
}

// 注册业务
void ChatService::reg(const TcpConnectionPtr& conn, json& js, Timestamp time) {
    // LOG_INFO << "Do reg service!!!";
    string name = js["name"];
    string pwd = js["password"];

    User user;
    user.setName(name);
    user.setPwd(pwd);
    bool state = _userModel.insert(user);

    if(state) {
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 0;
        response["id"] = user.getId();
        conn->send(response.dump());
    }
    else {
        json response;
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1;
        conn->send(response.dump());
    }
}

// 处理客户端异常退出
void ChatService::clientCloseException(const TcpConnectionPtr& conn) {
    int id = -1;

    // LOG_ERROR << "OK";

    {   // 临界区代码段
        lock_guard<mutex> lock(_connMutex);
        for(auto it = _userConnMap.begin(); it != _userConnMap.end(); it++) {
            if(it->second == conn) {
                id = it->first;
                _userConnMap.erase(it);
                break;
            }
        }
    }

    if(id != -1) {
        User user = _userModel.query(id);
        user.setState("offline");
        _userModel.updateState(user);
    }
}

// 处理服务器异常退出
void  ChatService::reset() {
    _userModel.resetState();
}

// 聊天服务
void ChatService::oneChat(const TcpConnectionPtr& conn, json& js, Timestamp time) {
    int toid = js["toid"].get<int>();

    {
        lock_guard<mutex> lock(_connMutex);
        auto it = _userConnMap.find(toid);
        if(it != _userConnMap.end()) {
            // toid 在线
            it->second->send(js.dump());
            return;
        }
    }

    // toid 不在线 存储离线消息
    _offlineMsgModel.insert(toid, js.dump());
}

// 添加朋友
void ChatService::addFriend(const TcpConnectionPtr& conn, json& js, Timestamp time) {
    int userid = js["id"].get<int>();
    int friendid = js["friendid"].get<int>();

    User user = _userModel.query(friendid);
    if(user.getId() == -1)  { 
        json response;
        response["msgid"] = ADD_FRIEND_MSG_ACK;
        response["errno"] = 3;
        response["errmsg"] = "user not exit";
        conn->send(response.dump());
    }
    else {
        _friendModel.insert(userid, friendid);
        json response;
        response["msgid"] = ADD_FRIEND_MSG_ACK;
        response["errno"] = 0;
        response["errmsg"] = "add friend success!";
        conn->send(response.dump());
    }
}


// 创建群组
void ChatService::createGroup(const TcpConnectionPtr& conn, json& js, Timestamp time) {
    int userid = js["id"].get<int>();
    string name = js["groupname"];
    string desc = js["groupdesc"];
    Group group(-1, name, desc);
    if(_groupModel.createGroup(group)) {
        _groupModel.addGroup(userid, group.getId(), "creator");
        json response;
        response["msgid"] = CREATE_GROUP_MSG_ACK;
        response["errno"] = 0;
        response["groupid"] = group.getId();
        response["errmsg"] = "create group success!";
        conn->send(response.dump());
    }
    else {
        json response;
        response["msgid"] = CREATE_GROUP_MSG_ACK;
        response["errno"] = 4;
        response["errmsg"] = "create group fail!";
        conn->send(response.dump());
    }
}

// 加入群组
void ChatService::addGroup(const TcpConnectionPtr& conn, json& js, Timestamp time) {
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    if(_groupModel.addGroup(userid, groupid, "normal")) {
        json response;
        response["msgid"] = ADD_GROUP_MSG_ACK;
        response["errno"] = 0;
        response["groupid"] = groupid;
        response["errmsg"] = "join group success!";
        conn->send(response.dump());
    }
    else {
        json response;
        response["msgid"] = ADD_GROUP_MSG_ACK;
        response["errno"] = 5;
        response["errmsg"] = "join group fail!";
    }
}

// 群组聊天
void ChatService::groupChat(const TcpConnectionPtr& conn, json& js, Timestamp time) {
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    vector<int> useridVec = _groupModel.queryGroupUsers(userid, groupid);

    lock_guard<mutex> lock(_connMutex);
    for(int id: useridVec) {
        auto it = _userConnMap.find(id);
        if(it != _userConnMap.end()) { it->second->send(js.dump()); }
        else { _offlineMsgModel.insert(id, js.dump()); }
    }
}
