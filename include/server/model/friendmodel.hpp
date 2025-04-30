#ifndef __FRIENDMODEL_H__
#define __FRIENDMODEL_H__

#include "user.hpp"
#include <vector>

// 提供好友信息
class FriendModel {
public:
    // 添加好友
    void insert(int userid, int friendid);

    // 返回好友列表
    vector<User> query(int userid);
};


#endif
