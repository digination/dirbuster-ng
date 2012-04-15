cmake_minimum_required(VERSION 2.6)

project(dirbuster-ng)

SET (CMAKE_C_FLAGS "-g -fPIE")

add_definitions(-DDBNG_PREFIX=\"${CMAKE_INSTALL_PREFIX}\")

add_executable(
	dirbuster-ng
	src/dirbuster-ng.c
	src/dirbuster-ng.h
	src/dirbuster-ng_config.h
	src/queue.c
	src/queue.h
        src/utils.c
        src/utils.h
        src/common.h
        src/uagents.h
        src/uagents.c
        )

find_library (PTHREAD pthread) 
find_library (CURL curl)


target_link_libraries(
        dirbuster-ng
        pthread
        curl
)

install (TARGETS dirbuster-ng DESTINATION bin)
