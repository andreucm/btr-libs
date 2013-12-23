FIND_PATH(
        serial_comm_INCLUDE_DIRS 
        NAMES serialComm.h
        PATHS /usr/local/include/btr-headers
)

FIND_LIBRARY(
        serial_comm_LIBRARIES
        NAMES serial_comm
        PATHS /usr/local/lib/btr-libs
) 

IF (serial_comm_INCLUDE_DIRS AND serial_comm_LIBRARIES)
   SET(serial_comm_FOUND TRUE)
ENDIF (serial_comm_INCLUDE_DIRS AND serial_comm_LIBRARIES)

IF (serial_comm_FOUND)
   IF (NOT serial_comm_FIND_QUIETLY)
      MESSAGE(STATUS "Found serial_comm library: ${serial_comm_LIBRARIES}")
   ENDIF (NOT serial_comm_FIND_QUIETLY)
ELSE (serial_comm_FOUND)
   IF (serial_comm_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find serial_comm library")
   ENDIF (serial_comm_FIND_REQUIRED)
ENDIF (serial_comm_FOUND)

