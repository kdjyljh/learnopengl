cmake_minimum_required(VERSION 2.6)

add_compile_options(-std=c++0x)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -fPIC -Wall -g2 -ggdb")  
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

project (learnopengl)

set(COMMON_LIBS GLEW glfw3 GL X11 Xi Xrandr Xxf86vm Xinerama Xcursor rt m pthread dl)
set(COMMON_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(COMMON_LIBS_DIR ${PROJECT_BANARY_DIR}/lib)
set(COMMON_BIN_DIR ${PROJECT_BANARY_DIR}/bin)
add_subdirectory(src)

add_custom_target(scripts_copy ALL
    "${CMAKE_COMMAND}" -E copy ${CMAKE_SOURCE_DIR}/scripts/env_set.sh ${CMAKE_BINARY_DIR}
    )
