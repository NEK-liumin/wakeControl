﻿cmake_minimum_required (VERSION 3.10)

project ("figure")
aux_source_directory(./ SRC)

link_directories(${LIBPATH})
link_libraries(model)
link_libraries(matrixOperation)
link_libraries(solver)
link_libraries(usr)
link_libraries(turbCloud)
link_libraries(input)
link_libraries(tecplot)
link_libraries(simulation)
link_libraries(run)

set(EXECUTABLE_OUTPUT_PATH ${WAKECONTROL}/bin)
add_executable (figure ${SRC})