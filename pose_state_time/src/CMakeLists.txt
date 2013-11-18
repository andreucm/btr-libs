# minimum cmake version
cmake_minimum_required(VERSION 2.6)

# add the necessary include directories
INCLUDE_DIRECTORIES(.)

# application source files
SET(SRCS
      timeStamp.cpp
      angle.cpp
      quaternion.cpp
      rotation.cpp
#       vqState.cpp
#       vqStateTs.cpp
#       wState.cpp           
)

SET(HEADERS 
      timeStamp.h
      angle.h
      quaternion.h
      rotation.h
#       vqState.h
#       vqStateTs.h        
#       wState.h
)

# create the shared library
ADD_LIBRARY(poseStateTime SHARED ${SRCS})

#install 
INSTALL(TARGETS poseStateTime
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/pipeGuard
        ARCHIVE DESTINATION lib/pipeGuard)

INSTALL(FILES ${HEADERS} DESTINATION include/pipeGuard)

INSTALL(FILES ../FindposeStateTime.cmake DESTINATION ${CMAKE_ROOT}/Modules/)

ADD_SUBDIRECTORY(examples)
