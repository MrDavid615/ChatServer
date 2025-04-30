#ifndef __GROUPUSER_H__
#define __GROUPUSER_H__

#include "user.hpp"

class GroupUser : public User {
public:
    void setRole(string role_) { role = role_; }
    string getRole() { return role; }

private:
    string role;
};

#endif
