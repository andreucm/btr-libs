# Test for camera acquisition
ADD_EXECUTABLE(serialCommTest serialCommTest.cpp)
ADD_EXECUTABLE(likaIF51_test likaIF51_test.cpp)
TARGET_LINK_LIBRARIES(serialCommTest serial_comm)
TARGET_LINK_LIBRARIES(likaIF51_test serial_comm)
