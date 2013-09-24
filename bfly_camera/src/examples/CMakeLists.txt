# Test for camera constraint
ADD_EXECUTABLE(bflyAcquisition main.cpp)
TARGET_LINK_LIBRARIES(bflyAcquisition bfly_camera ${OpenCV_LIBS})
