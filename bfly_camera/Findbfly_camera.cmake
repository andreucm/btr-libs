FIND_PATH(
      bfly_camera_INCLUDE_DIRS 
      NAMES bflyCamera.h
      PATHS /usr/local/include/btr-headers
)

FIND_LIBRARY(
      bfly_camera_LIBRARIES
      NAMES bfly_camera
      PATHS /usr/local/lib/btr-libs
) 

IF (bfly_camera_INCLUDE_DIRS AND bfly_camera_LIBRARIES)
   SET(bfly_camera_FOUND TRUE)
ENDIF (bfly_camera_INCLUDE_DIRS AND bfly_camera_LIBRARIES)

IF (bfly_camera_FOUND)
   IF (NOT bfly_camera_FIND_QUIETLY)
      MESSAGE(STATUS "Found bfly_camera library: ${bfly_camera_LIBRARIES}")
   ENDIF (NOT bfly_camera_FIND_QUIETLY)
ELSE (bfly_camera_FOUND)
   IF (bfly_camera_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find bfly_camera library")
   ENDIF (bfly_camera_FIND_REQUIRED)
ENDIF (bfly_camera_FOUND)

