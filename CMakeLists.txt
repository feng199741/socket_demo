#指定CMake最低版本
cmake_minimum_required(VERSION 3.10)

#项目名称
project(socket_demo)

#设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#可执行文件的输出路径
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)

#添加可执行文件
add_executable(socket_demo send_PointCloud/send_pointcloud.cpp)

#链接库
if (UNIX)
    target_link_libraries(socket_demo PRIVATE pthread)
endif()