FIND_PATH(
      faramotics_INCLUDE_DIRS
      NAMES sceneRender.h groundRender.h floorGrid.h rangeSector.h rangeImage.h rangeScan2D.h dynamicSceneRender.h
      PATHS /usr/local/include/btr-headers
)

FIND_LIBRARY(
      faramotics_LIBRARIES
      NAMES faramotics
      PATHS /usr/local/lib/btr-libs
)

IF (faramotics_INCLUDE_DIRS AND faramotics_LIBRARIES)
   SET(faramotics_FOUND TRUE)
ENDIF (faramotics_INCLUDE_DIRS AND faramotics_LIBRARIES)

IF (faramotics_FOUND)
   IF (NOT faramotics_FIND_QUIETLY)
      MESSAGE(STATUS "Found faramotics library: ${faramotics_LIBRARIES}")
   ENDIF (NOT faramotics_FIND_QUIETLY)
ELSE (faramotics_FOUND)
   IF (faramotics_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find faramotics library")
   ENDIF (faramotics_FIND_REQUIRED)
ENDIF (faramotics_FOUND)

