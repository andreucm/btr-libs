FIND_PATH(
      ball_detector_INCLUDE_DIRS 
      NAMES ballDetector.h 
      PATHS /usr/local/include/btr-headers
)

FIND_LIBRARY(
      ball_detector_LIBRARIES
      NAMES ball_detector
      PATHS /usr/local/lib/btr-libs
) 



IF (ball_detector_INCLUDE_DIRS)
      MESSAGE(STATUS "------------ Found ball_detector_INCLUDE_DIRS: ${ball_detector_INCLUDE_DIRS}")
ELSE (ball_detector_INCLUDE_DIRS)
      MESSAGE(STATUS "------------ NOT Found ball_detector_INCLUDE_DIRS: ${ball_detector_INCLUDE_DIRS}")
ENDIF (ball_detector_INCLUDE_DIRS)



IF (ball_detector_INCLUDE_DIRS AND ball_detector_LIBRARIES)
   SET(ball_detector_FOUND TRUE)
ENDIF (ball_detector_INCLUDE_DIRS AND ball_detector_LIBRARIES)

IF (ball_detector_FOUND)
   IF (NOT ball_detector_FIND_QUIETLY)
      MESSAGE(STATUS "Found ball_detector library: ${ball_detector_LIBRARIES}")
   ENDIF (NOT ball_detector_FIND_QUIETLY)
ELSE (ball_detector_FOUND)
   IF (ball_detector_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find ball_detector library")
   ENDIF (ball_detector_FIND_REQUIRED)
ENDIF (ball_detector_FOUND)

