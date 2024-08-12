cmake_minimum_required (VERSION 3.8)

project ("template")
aux_source_directory(./ SRC)

link_directories(${LIBPATH})
link_libraries(model)
link_libraries(matrixOperation)
link_libraries(solver)
link_libraries(usr)
link_libraries(turbCloud)
link_libraries(input)

set(EXECUTABLE_OUTPUT_PATH ${WAKECONTROL}/bin)
add_executable (template ${SRC})