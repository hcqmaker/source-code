include_directories(${LOCAL_INCLUDE_DIR})
include_directories(${TOOL_DIR})
include_directories(${PROJECT_EV_DIR}/libev)

include_directories(${PROJECT_EV_DIR}/include)
FILE(GLOB_RECURSE EV_SRC_FILE ${PROJECT_EV_DIR}/src/*.cpp)

add_executable(server  EXCLUDE_FROM_ALL entry.cpp 
    ${EV_SRC_FILE})

target_link_libraries(server pthread ${EV_LIB_FILE})
