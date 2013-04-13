
#
# CMakeLists.txt
#
# fU9ANg (bb.newlife@gmail.com)
# 

cmake_minimum_required (VERSION 2.8)
project (evFTPSrv)

SET (CMAKE_CXX_FLAGS_DEBUG " -O0 -Wall -Werror -g -ggdb -D_ONLY_TEST")
SET (CMAKE_C_FLAGS_DEBUG   " -O0 -Wall -Werror -g -ggdb -D_ONLY_TEST")
SET (CMAKE_BUILD_TYPE Debug)

SET (CMAKE_CXX_COMPILER   "ccache")
SET (CMAKE_CXX_COMPILER_ARG1 "g++")
SET (CMAKE_C_COMPILER     "ccache")
SET (CMAKE_C_COMPILER_ARG1   "gcc")

INCLUDE_DIRECTORIES (
    .
)

SET (SRC_LIST
    d2EvLoopTask.cpp
    d2FTPTask.cpp
    d2MemBlock.cpp
    d2SendTask.cpp
    d2SingleFactory.cpp
    d2ThreadPool.cpp
    Main.cpp
)

SET (EXTRA_LIBS
   -lpthread
   -lrt
   -lev
)

add_executable (evFTPSrv ${SRC_LIST})
target_link_libraries (evFTPSrv ${EXTRA_LIBS})
