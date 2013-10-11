cmake_minimum_required(VERSION 2.6)

# locate the necessary dependencies, by FIND_PACKAGE or by manual setting if no Find module exists
FIND_PACKAGE(OpenCV REQUIRED)
SET (ptgrey_INCLUDE_DIRS "/usr/include/flycapture")
SET (ptgrey_LIBRARIES "flycapture")

# add the necessary include directories
INCLUDE_DIRECTORIES(. ${OpenCV_INCLUDE_DIRS} ${ptgrey_INCLUDE_DIRS})

# set application source files
SET(SRCS bflyCamera.cpp)

# set application source headers
SET(HEADERS bflyCamera.h)

# create the shared library
ADD_LIBRARY(bfly_camera SHARED ${SRCS})

# link necessary libraries
TARGET_LINK_LIBRARIES(bfly_camera ${OpenCV_LIBS} ${ptgrey_LIBRARIES})

# install library
INSTALL(TARGETS bfly_camera
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/btr-libs
        ARCHIVE DESTINATION lib/btr-libs)

# install headers
INSTALL(FILES ${HEADERS} DESTINATION include/btr-headers)

#install Find*.cmake Module
INSTALL(FILES ../Findbfly_camera.cmake DESTINATION ${CMAKE_ROOT}/Modules/)

#continue to examples folder
ADD_SUBDIRECTORY(examples)
