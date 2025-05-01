#ifndef __CHATSERVICE_H__
#define __CHATSERVICE_H__

#include "json.hpp"
#include "public.hpp"
#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"

#include <mutex>
#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
using namespace std;
using namespace muduo;
using namespace muduo::net;

using json = nlohmann::json;
using MsgHandler = std::function<void(const TcpConnectionPtr& conn, json& js, Timestamp time)>;

// 提供业务代码，使用单例模式
class ChatService
{
public:
    // 获取单例对象的接口函数
    static ChatService* instance();

    // 登录业务
    void login(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 注销业务
    void loginOut(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 注册业务
    void reg(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 一对一聊天
    void oneChat(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 添加朋友
    void addFriend(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 创建群组
    void createGroup(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 加入群组
    void addGroup(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 群组聊天
    void groupChat(const TcpConnectionPtr& conn, json& js, Timestamp time);

    // 获取消息对应的处理器
    MsgHandler getHandler(int id);

    // 处理客户端异常退出
    void clientCloseException(const TcpConnectionPtr& conn);

    // 处理服务器异常退出
    void reset();

private:
    ChatService();  // 单例模式需要私有化构造函数

    // 存储消息id何对应的业务处理方法
    unordered_map<int, MsgHandler> _msgHandlerMap;

    // 存储在线用户的通信链接
    unordered_map<int, TcpConnectionPtr> _userConnMap;

    // 定义互斥锁，保证线程安全
    mutex _connMutex;

    // 数据操作类对象
    UserModel _userModel;
    OfflineMsgModel _offlineMsgModel;
    FriendModel _friendModel;
    GroupModel _groupModel;
};

#endif
