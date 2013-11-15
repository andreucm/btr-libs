#edit the following line to add the librarie's header files
FIND_PATH(
        poseStateTime_INCLUDE_DIR 
        NAMES timeStamp.h quaternion.h vqState.h vqStateTs.h wState.h                
        PATHS /usr/include/pipeGuard /usr/local/include/pipeGuard        
)

FIND_LIBRARY(
        poseStateTime_LIBRARY
        NAMES poseStateTime
        PATHS /usr/lib /usr/local/lib /usr/local/lib/pipeGuard
) 

IF (poseStateTime_INCLUDE_DIR AND poseStateTime_LIBRARY)
   SET(poseStateTime_FOUND TRUE)
ENDIF (poseStateTime_INCLUDE_DIR AND poseStateTime_LIBRARY)

IF (poseStateTime_FOUND)
   IF (NOT poseStateTime_FIND_QUIETLY)
      MESSAGE(STATUS "Found poseStateTime: ${poseStateTime_LIBRARY}")
   ENDIF (NOT poseStateTime_FIND_QUIETLY)
ELSE (poseStateTime_FOUND)
   IF (poseStateTime_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find poseStateTime")
   ENDIF (poseStateTime_FIND_REQUIRED)
ENDIF (poseStateTime_FOUND)

