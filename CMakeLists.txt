# CMakeList.txt: wakeControl 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("wakeControl")
#当前目录，即根目录
set(WAKECONTROL ${PROJECT_SOURCE_DIR})
set(LIBPATH ${PROJECT_SOURCE_DIR}/lib)
#main函数所在目录
aux_source_directory(${PROJECT_SOURCE_DIR}/main SRC)

#所有头文件
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/matrixOperation)
include_directories(${PROJECT_SOURCE_DIR}/model)
include_directories(${PROJECT_SOURCE_DIR}/solver)
include_directories(${PROJECT_SOURCE_DIR}/main)
#子目录
add_subdirectory(matrixOperation)
add_subdirectory(model)
add_subdirectory(QP)
add_subdirectory(solver)

link_directories(${LIBPATH})
link_libraries(model)
link_libraries(matrixOperation)
# 将源代码添加到此项目的可执行文件。
add_executable (wakeControl ${SRC})