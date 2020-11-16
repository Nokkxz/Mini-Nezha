cmake_minimum_required(VERSION 3.0.0)


STRING( REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} ) 

message(The current folder is ${CURRENT_FOLDER})
project(${CURRENT_FOLDER} VERSION 0.1.0)

include(CTest)
enable_testing()


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

aux_source_directory(./src DIR_SRCS)
include_directories(./inc)
add_executable(${CURRENT_FOLDER} ${DIR_SRCS})

target_link_libraries(${CURRENT_FOLDER} ${OpenCV_LIBS})

set(CMAKE_BUILD_TYPE "Debug")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
