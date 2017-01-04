include_directories(${COMMON_INCLUDE_DIR})

link_directories(${COMMON_LIBS_DIR})

set(LT_COMMON_LIBS utils ${COMMON_LIBS})


add_executable(colors colors.cpp)
target_link_libraries(colors ${LT_COMMON_LIBS})

set_target_properties(
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/bin/)
