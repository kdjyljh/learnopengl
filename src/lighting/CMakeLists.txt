include_directories(${COMMON_INCLUDE_DIR})

link_directories(${COMMON_LIBS_DIR})

set(LT_COMMON_LIBS utils ${COMMON_LIBS})


add_executable(colors colors.cpp)
target_link_libraries(colors ${LT_COMMON_LIBS})

add_executable(basiclighting basiclighting.cpp)
target_link_libraries(basiclighting ${LT_COMMON_LIBS})

add_executable(testbl testbl.cpp)
target_link_libraries(testbl ${LT_COMMON_LIBS})

add_executable(materials materials.cpp)
target_link_libraries(materials ${LT_COMMON_LIBS})

set_target_properties(colors basiclighting testbl materials
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/bin/)
