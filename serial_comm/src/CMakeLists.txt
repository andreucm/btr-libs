cmake_minimum_required(VERSION 2.6)

# required include directorires
INCLUDE_DIRECTORIES(.)

# set application source files
SET(SRCS serialComm.cpp)

# set application source headers
SET(HEADERS serialComm.h)

# create the shared library
ADD_LIBRARY(serial_comm SHARED ${SRCS})

# install library
INSTALL(TARGETS serial_comm
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/btr-libs
        ARCHIVE DESTINATION lib/btr-libs)

# install headers
INSTALL(FILES ${HEADERS} DESTINATION include/btr-headers)

#install Find*.cmake Module
INSTALL(FILES ../Findserial_comm.cmake DESTINATION ${CMAKE_ROOT}/Modules/)

#continue to examples folder
ADD_SUBDIRECTORY(examples)
