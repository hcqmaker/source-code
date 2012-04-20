cmake_minimum_required(VERSION 2.8)
project(libev_test)

#set binary tree type(output)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

#find header file(include)
set(LOCAL_INCLUDE_DIR /usr/local/include)

#lib
set(LOCAL_LIB_DIR /usr/local/lib)

#link lib
link_directories(${LOCAL_LIB_DIR})

#find library of static and shared
#find_library(EV_LIB_FILE names libev libevview  libevdocument paths /usr/lib/ /usr/local/lib/)
find_library(EV_LIB_FILE NAMES ev PATHS /usr/local/lib/)

#set build type
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -Wall)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -Wall)
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb") 
set(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} -O0 -Wall -g -ggdb") 
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} -O3 -Wall")


#project dir
set(PROJECT_EV_DIR ${PROJECT_SOURCE_DIR}/source/)
set(TOOL_DIR ${PROJECT_SOURCE_DIR}/include/)

#message cmake subdirectory
add_subdirectory(source)
add_subdirectory(test)
