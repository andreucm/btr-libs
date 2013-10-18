FIND_PATH(
	point_tracker_INCLUDE_DIRS 
	NAMES trackStamped.h pointTracker.h
	PATHS /usr/local/include/btr-headers
)

FIND_LIBRARY(
        point_tracker_LIBRARIES
        NAMES point_tracker
        PATHS /usr/local/lib/btr-libs
) 

IF (point_tracker_INCLUDE_DIRS AND point_tracker_LIBRARIES)
   SET(point_tracker_FOUND TRUE)
ENDIF (point_tracker_INCLUDE_DIRS AND point_tracker_LIBRARIES)

IF (point_tracker_FOUND)
   IF (NOT point_tracker_FIND_QUIETLY)
      MESSAGE(STATUS "Found point_tracker library: ${point_tracker_LIBRARY}")
   ENDIF (NOT point_tracker_FIND_QUIETLY)
ELSE (point_tracker_FOUND)
   IF (point_tracker_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find point_tracker library")
   ENDIF (point_tracker_FIND_REQUIRED)
ENDIF (point_tracker_FOUND)

