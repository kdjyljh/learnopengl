include_directories(${COMMON_INCLUDE_DIR})

link_directories(${COMMON_LIBS_DIR})

add_executable(createwindow createwindow.cpp)
target_link_libraries(createwindow ${COMMON_LIBS})
add_executable(triangle triangle.cpp)
target_link_libraries(triangle utils ${COMMON_LIBS})
add_executable(shader shader.cpp)
target_link_libraries(shader utils ${COMMON_LIBS})

set_target_properties(createwindow triangle shader PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/bin/gettingstart)
