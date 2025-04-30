#include "friendmodel.hpp"
#include "db.h"

#include <muduo/base/Logging.h>

// 添加好友
void FriendModel::insert(int userid, int friendid) {
    char sql[1024] = {0};
    sprintf(sql, "insert into Friend(userid, friendid) values(%d, %d)", userid, friendid);

    // cout <<sql<<endl;
    MySQL mysql;
    if(mysql.connect()) {
        if(mysql.update(sql)) {
            return;
        }
    }

    LOG_ERROR << "Add friend fail";
    return;
}

// 返回好友列表
vector<User> FriendModel::query(int userid) {
    char sql[1024] = {0};
    // 多表联合查询
    // sprintf(sql, "select friendid from Friend");
    sprintf(sql, 
            "select a.id, a.name, a.state from User a inner join Friend b on b.friendid = a.id where b.userid = %d", 
            userid);

    vector<User> vec;
    MySQL mysql;
    if(mysql.connect()) {
        MYSQL_RES* res = mysql.query(sql);
        if(res != nullptr) {
            // 把所有离线消息返回
            MYSQL_ROW row ;
            while ( (row = mysql_fetch_row(res)) != nullptr ) {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(row[2]);
                vec.emplace_back(user);
            }

            mysql_free_result(res);
        }
    }
    
    return vec;
}