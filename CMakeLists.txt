cmake_minimum_required(VERSION 2.8)

add_compile_options(-std=c++0x)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -fPIC -Wall -g2 -ggdb")  
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

project (learnopengl)

set(COMMON_LIBS GLEW glfw3 GL X11 Xi Xrandr Xxf86vm Xinerama Xcursor rt m pthread dl SOIL)
set(COMMON_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/include)
set(COMMON_LIBS_DIR ${PROJECT_BINARY_DIR}/lib ${PROJECT_SOURCE_DIR}/lib)
set(COMMON_BIN_DIR ${PROJECT_BINARY_DIR}/bin)
set(LEANOPENGL_RUN_DIR ${PROJECT_BINARY_DIR})

add_subdirectory(src)

configure_file(
    "${PROJECT_SOURCE_DIR}/modules/environment.cmake.in"
    "${PROJECT_BINARY_DIR}/include/environment.cmake.h"
    )

#add_custom_target(file_copy ALL
#    "${CMAKE_COMMAND}" -E copy ${CMAKE_SOURCE_DIR}/scripts/env_set.sh ${CMAKE_BINARY_DIR}
#    "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
#    file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
#    )

#execute_process(COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/scripts/env_set.sh ${CMAKE_BINARY_DIR})

file(COPY ${CMAKE_SOURCE_DIR}/scripts/env_set.sh DESTINATION ${CMAKE_BINARY_DIR})
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
