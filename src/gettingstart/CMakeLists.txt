include_directories(${COMMON_INCLUDE_DIR})

link_directories(${COMMON_LIBS_DIR})

set(GS_COMMON_LIBS utils ${COMMON_LIBS})


add_executable(createwindow createwindow.cpp)
target_link_libraries(createwindow ${COMMON_LIBS})

add_executable(triangle triangle.cpp)
target_link_libraries(triangle ${GS_COMMON_LIBS})

add_executable(shader shader.cpp)
target_link_libraries(shader ${GS_COMMON_LIBS})

add_executable(texture texture.cpp)
target_link_libraries(texture ${GS_COMMON_LIBS})

add_executable(transformation transformation.cpp)
target_link_libraries(transformation ${GS_COMMON_LIBS})

set_target_properties(createwindow triangle shader texture transformation
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_BINARY_DIR}/bin/)

#add_custom_target(shaderfile_copy ALL
#    "${CMAKE_COMMAND}" -E copy ${CMAKE_CURRENT_SOURCE_DIR}/vertex.sd ${CMAKE_CURRENT_SOURCE_DIR}/fragment.sd ${PROJECT_BINARY_DIR}/bin/gettingstart
#    )
