FIND_PATH(
      ball_detector_INCLUDE_DIR 
      NAMES ballDetector.h 
      PATHS /usr/local/include/btr-headers
)

FIND_LIBRARY(
      ball_detector_LIBRARY
      NAMES ball_detector
      PATHS /usr/local/include/btr-libs
) 

IF (ball_detector_INCLUDE_DIR AND ball_detector_LIBRARY)
   SET(ball_detector_FOUND TRUE)
ENDIF (ball_detector_INCLUDE_DIR AND ball_detector_LIBRARY)

IF (ball_detector_FOUND)
   IF (NOT ball_detector_FIND_QUIETLY)
      MESSAGE(STATUS "Found ball_detector library: ${ball_detector_LIBRARY}")
   ENDIF (NOT ball_detector_FIND_QUIETLY)
ELSE (ball_detector_FOUND)
   IF (ball_detector_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find ball_detector library")
   ENDIF (ball_detector_FIND_REQUIRED)
ENDIF (ball_detector_FOUND)

