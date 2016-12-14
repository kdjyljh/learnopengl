add_subdirectory(gettingstart)
set(SRC_FILE utils.cpp ShaderHelper.cpp)

include_directories(${COMMON_INCLUDE_DIR})

add_library(utils SHARED ${SRC_FILE})

set_target_properties(utils PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
