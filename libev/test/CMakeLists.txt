AUX_SOURCE_DIRECTORY(${PROJECT_SOURCE_DIR}/source/src}/   DIR_EV_SRC)
include_directories(${TOOL_DIR}/ ${PROJECT_EV_DIR}/include/)

add_executable(server  entry.cpp 
    ${DIR_EV_SRC})

target_link_libraries(server pthread ${EV_LIB_FILE})
