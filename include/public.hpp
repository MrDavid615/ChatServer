#ifndef __PUBLIC_HPP__
#define __PUBLIC_HPP__

/*
server与client的公共资源
*/

enum EnMsgType {
    LOGIN_MSG = 1,
    LOGIN_MSG_ACK,
    REG_MSG,
    REG_MSG_ACK,     // 注册响应消息
    ONE_CHAT_MSG,
    ADD_FRIEND_MSG, //  添加好友消息
    ADD_FRIEND_MSG_ACK,

    CREATE_GROUP_MSG,
    CREATE_GROUP_MSG_ACK,
    ADD_GROUP_MSG,
    ADD_GROUP_MSG_ACK,
    GROUP_CHAT_MSG,
};


/* REG_MSG {
"name" : "li si"
"password" : "123456"
}

{"msgid":3,"name":"li si","password":"123456"}
*/

/* LOGIN_MSG {
"id" : "2"
"password" : "123456"
}

{"msgid":1,"id":1,"password":"123456"}
{"msgid":1,"id":2,"password":"123456"}
*/

/* ONE_CHAT_MSG {
"id" : 1
"from" : "zhang san"
"toid" : 3
"msg" : "hello"
}

{"msgid":5,"id":2,"form":"li si","to":1,"msg":"hello world!"}
*/

/* ADD_FRIEND_MSG {
"id" : 1
"friendid" : 2
}

{"msgid":6,"id":1,"friendid":2}
*/

#endif
