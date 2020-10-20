# CMakeList.txt: NetWork 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。
add_executable (SelectSrv "selectSrv.cpp")
add_executable (SelectClient "selectclient.cpp")
add_executable (PollSrv "pollsrv.cpp")
add_executable (PollClient "pollclient.cpp")

add_executable (EpollSrv "epollsrv.cpp")
# TODO: 如有需要，请添加测试并安装目标。
