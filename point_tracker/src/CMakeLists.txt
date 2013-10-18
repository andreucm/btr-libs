cmake_minimum_required(VERSION 2.6)

# locate the necessary dependencies
FIND_PACKAGE(OpenCV REQUIRED)

# add the necessary include directories
INCLUDE_DIRECTORIES(. ${OpenCV_INCLUDE_DIR})

# application source files
SET(sources 
	trackStamped.cpp
	pointTracker.cpp 
)

# application header files
SET(headers
	trackStamped.h
	pointTracker.h
)

# create the shared library
ADD_LIBRARY(point_tracker SHARED ${sources})

# link necessary libraries
TARGET_LINK_LIBRARIES(point_tracker ${OpenCV_LIBS})

#install 
INSTALL(TARGETS point_tracker
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/btr-libs
        ARCHIVE DESTINATION lib/btr-libs)

INSTALL(FILES ${headers} DESTINATION include/btr-headers)

INSTALL(FILES ../Findpoint_tracker.cmake DESTINATION ${CMAKE_ROOT}/Modules/)

ADD_SUBDIRECTORY(examples)
