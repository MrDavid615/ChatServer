#ifndef __GROUP_H__
#define __GROUP_H__

#include "groupuser.hpp"
#include <string>
#include <vector>
using namespace std;

class Group {
public:
    Group(int _id = -1, string _name = "", string _desc = "") 
            : id(_id)
            , name(_name)
            , desc(_desc) {}

    void setId(int id_) { id = id_; }
    void setName(string name_) { name = name_; }
    void setDesc(string desc_) { desc = desc_; }

    int getId() { return id; }
    string getName() { return name; }
    string getDesc() { return desc; }
    vector<GroupUser>& getUsers() { return user; }

private:
    int id;
    string name;
    string desc;
    vector<GroupUser> user;
};


#endif
