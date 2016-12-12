add_executable(createwindow createwindow.cpp)
target_link_libraries(createwindow "${COMMON_LIBS}")
add_executable(triangle triangle.cpp)
target_link_libraries(triangle ${COMMON_LIBS})
