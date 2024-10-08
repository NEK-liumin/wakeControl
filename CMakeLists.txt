﻿# CMakeList.txt: wakeControl 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

# 设置全局编译选项
if(WIN32)
    add_compile_definitions(ENCODING_UTF8_BOM=1)
else()
    add_compile_definitions(ENCODING_UTF8_BOM=0)
endif()


cmake_minimum_required (VERSION 3.10)

project ("base")
#当前目录，即根目录

set(CMAKE_CXX_STANDARD 17)

set(WAKECONTROL ${PROJECT_SOURCE_DIR})
set(LIBPATH ${PROJECT_SOURCE_DIR}/lib)
#main函数所在目录
#aux_source_directory(${PROJECT_SOURCE_DIR}/wakeControl SRC)

#所有头文件
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/matrixOperation)
include_directories(${PROJECT_SOURCE_DIR}/model)
include_directories(${PROJECT_SOURCE_DIR}/solver)
include_directories(${PROJECT_SOURCE_DIR}/QP)
include_directories(${PROJECT_SOURCE_DIR}/main)
include_directories(${PROJECT_SOURCE_DIR}/GP)
include_directories(${PROJECT_SOURCE_DIR}/usr)
include_directories(${PROJECT_SOURCE_DIR}/turbCloud)
include_directories(${PROJECT_SOURCE_DIR}/input)
include_directories(${PROJECT_SOURCE_DIR}/template)
include_directories(${PROJECT_SOURCE_DIR}/tecplot)
include_directories(${PROJECT_SOURCE_DIR}/simulation)
include_directories(${PROJECT_SOURCE_DIR}/run)
include_directories(${PROJECT_SOURCE_DIR}/figure)
include_directories(${PROJECT_SOURCE_DIR}/statistics)

#子目录
add_subdirectory(matrixOperation)
add_subdirectory(model)
add_subdirectory(QP)
add_subdirectory(GP)
add_subdirectory(solver)
add_subdirectory(usr)
add_subdirectory(turbCloud)
add_subdirectory(input)
add_subdirectory(wakeControl)
add_subdirectory(template)
add_subdirectory(tecplot)
add_subdirectory(simulation)
add_subdirectory(run)
add_subdirectory(figure)
add_subdirectory(statistics)

# 将源代码添加到此项目的可执行文件。
#set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#add_executable (wakeControl ${SRC})