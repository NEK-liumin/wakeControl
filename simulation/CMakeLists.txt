# CMakeList.txt: wakeControl 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.10)

# 如果支持，请为 MSVC 编译器启用热重载。

project ("simulation")
aux_source_directory(./ SRC)

# 将源代码添加到此项目的可执行文件。
set(LIBRARY_OUTPUT_PATH ${LIBPATH})
add_library(simulation STATIC ${SRC})

# TODO: 如有需要，请添加测试并安装目标。
