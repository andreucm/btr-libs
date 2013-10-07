#edit the following line to add the librarie's header files
FIND_PATH(
        bfly_camera_INCLUDE_DIRS 
        NAMES bflyCamera.h
        PATHS /usr/local/include/btr_headers
)

FIND_LIBRARY(
        bfly_camera_LIBRARIES
        NAMES bfly_camera
        PATHS /usr/local/lib/btr_libs
) 

IF (bfly_camera_INCLUDE_DIR AND bfly_camera_LIBRARY)
   SET(bfly_camera_FOUND TRUE)
ENDIF (bfly_camera_INCLUDE_DIR AND bfly_camera_LIBRARY)

IF (bfly_camera_FOUND)
   IF (NOT bfly_camera_FIND_QUIETLY)
      MESSAGE(STATUS "Found bfly_camera: ${bfly_camera_LIBRARIES}")
   ENDIF (NOT bfly_camera_FIND_QUIETLY)
ELSE (bfly_camera_FOUND)
   IF (bfly_camera_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find bfly_camera")
   ENDIF (bfly_camera_FIND_REQUIRED)
ENDIF (bfly_camera_FOUND)
