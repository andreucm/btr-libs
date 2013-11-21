#edit the following line to add the librarie's header files
FIND_PATH(
        pose_state_time_INCLUDE_DIRS
        NAMES timeStamp.h quaternion.h rotation.h                
        PATHS /usr/local/include/btr-headers
)

FIND_LIBRARY(
        pose_state_time_LIBRARIES
        NAMES pose_state_time
        PATHS /usr/local/lib/btr-libs
) 

IF (pose_state_time_INCLUDE_DIRS AND pose_state_time_LIBRARIES)
   SET(pose_state_time_FOUND TRUE)
ENDIF (pose_state_time_INCLUDE_DIRS AND pose_state_time_LIBRARIES)

IF (pose_state_time_FOUND)
   IF (NOT pose_state_time_FIND_QUIETLY)
      MESSAGE(STATUS "Found pose_state_time: ${pose_state_time_LIBRARIES}")
   ENDIF (NOT pose_state_time_FIND_QUIETLY)
ELSE (pose_state_time_FOUND)
   IF (pose_state_time_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find pose_state_time")
   ENDIF (pose_state_time_FIND_REQUIRED)
ENDIF (pose_state_time_FOUND)

