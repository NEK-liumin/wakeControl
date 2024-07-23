# CMakeList.txt: wakeControl 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("usr")
aux_source_directory(./ SRC)

include_directories(${PROJECT_SOURCE_DIR}/matrixOperation)
include_directories(${PROJECT_SOURCE_DIR}/model)
include_directories(${PROJECT_SOURCE_DIR}/solver)
include_directories(${PROJECT_SOURCE_DIR}/main)


# 将源代码添加到此项目的可执行文件。
set(LIBRARY_OUTPUT_PATH ${LIBPATH})
add_library(usr ${SRC})

# TODO: 如有需要，请添加测试并安装目标。
