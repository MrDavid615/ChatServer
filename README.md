# ChatServer
在linux环境下基于Muduo网络库开发的，可以工作于nginx tcp负载均衡环境下的集群聊天服务器与客户端源码

main分支：单机版本的聊天服务器
MultiSer分支：集群版本的聊天服务器，基于Nginx与Redis

需要配置Redis与Nginx环境，对于Nginx环境
Nginx：实现负载均衡功能，提高系统并发能力
Redis：用于多服务器间的消息通信，基于发布订阅功能

工程目录介绍
一级文件夹：ChatServer/~
include：头文件
src：源文件
test：sample案例文件，包含json序列化与muduo服务器设计
thirdparty：第三方库

二级文件夹：ChatServer/src/~ 或 ChatServer/include/~
client：客户端相关文件
server：服务器相关文件



个人CSDN博客：https://blog.csdn.net/Mr_Hanc_Tiskor?spm=1000.2115.3001.5343
