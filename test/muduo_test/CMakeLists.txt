cmake_minimum_required(VERSION 3.0)
project(main)

# g++ -o server -g muduo_test.cpp -I/usr/include -L/usr/lib -Imuduo_ner -Imuduo_base

# 配置编译选项
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -g)

# 配置头文件搜索路径
# include_directories()
# 配置库文件搜索路径
# link_directories()

# 设置需要编辑的源文件列表
set(SRC_LIST muduo_test.cpp)

# 把.指定路径下所有源文件名字放入变量SRC_LIST中
aux_source_directory(. SRC_LIST)

# 设置可执行文件存储路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# set(LIB_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# 生成可执行文件
add_executable(server ${SRC_LIST})

# 需要链接的库
# 表示server这个目标文件需要链接muduo_net muduo_base pthread这三个库
target_link_libraries(server muduo_net muduo_base pthread)
