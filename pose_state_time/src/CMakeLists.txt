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
      pose3d.cpp
      vqState.cpp
#       vqStateTs.cpp
#       wState.cpp           
)

SET(HEADERS 
      timeStamp.h
      angle.h
      quaternion.h
      rotation.h
      pose3d.h
      vqState.h
#       vqStateTs.h        
#       wState.h
)

# create the shared library
ADD_LIBRARY(pose_state_time SHARED ${SRCS})

#install 
INSTALL(TARGETS pose_state_time
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/btr-libs
        ARCHIVE DESTINATION lib/btr-libs)

INSTALL(FILES ${HEADERS} DESTINATION include/btr-headers)

INSTALL(FILES ../Findpose_state_time.cmake DESTINATION ${CMAKE_ROOT}/Modules/)

ADD_SUBDIRECTORY(examples)
