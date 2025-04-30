#ifndef __GROUPMODEL_H__
#define __GROUPMODEL_H__

#include "group.hpp"
#include <string>
#include <vector>

class GroupModel {
public:
    // 创建群组
    bool createGroup(Group &group);
    // 加入群主
    void addGroup(int userid, int groupid, string role);
    // 查询用户所在的群组信息
    vector<Group> queryGroups(int userid);
    // 查询群组合内的用户
    vector<int> queryGroupUsers(int userid, int groupid);
};

#endif
