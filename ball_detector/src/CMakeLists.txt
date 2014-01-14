cmake_minimum_required(VERSION 2.6)

# locate the necessary dependencies
FIND_PACKAGE(OpenCV REQUIRED)

# add the necessary include directories
INCLUDE_DIRECTORIES(. ${OpenCV_INCLUDE_DIR})

# application source files
SET(sources 
	ballDetector.cpp
)

# application header files
SET(headers 
	ballDetector.h
)

# create the executable file
ADD_LIBRARY(ball_detector SHARED ${sources})

# link necessary libraries
TARGET_LINK_LIBRARIES(ball_detector ${OpenCV_LIBS})

# install library
INSTALL(TARGETS ball_detector
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/btr-libs
        ARCHIVE DESTINATION lib/btr-libs)

# install headers
INSTALL(FILES ${HEADERS} DESTINATION include/btr-headers)

#install Find*.cmake Module
INSTALL(FILES ../Findball_detector.cmake DESTINATION ${CMAKE_ROOT}/Modules/)

ADD_SUBDIRECTORY(examples)
