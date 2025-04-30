#ifndef __USR_H__
#define __USR_H__


#include <string>
using namespace std;

// 参考数据库，user的ORM类
class User {
public:
    User(int id_ = -1, string name_ = "", string pwd_ = "", string state_ = "offline")
        : id(id_) 
        , name(name_)
        , password(pwd_)
        , state(state_)
    {}

    void setId(int id) { this->id = id; }
    void setName(string name) { this->name = name; }
    void setPwd(string pwd) { this->password = pwd; }
    void setState(string state) { this->state = state; }

    int getId(void) { return this->id; }
    string getName(void) { return this->name; }
    string getPwd(void) { return this->password; }
    string getState(void) { return this->state; }

protected:
    int id;
    string name;
    string password;
    string state;
};

#endif
