
ADD_EXECUTABLE(quaternion_test quaternion_test.cpp)
TARGET_LINK_LIBRARIES(quaternion_test pose_state_time)

ADD_EXECUTABLE(rotation_test rotation_test.cpp)
TARGET_LINK_LIBRARIES(rotation_test pose_state_time)

ADD_EXECUTABLE(pose3d_test pose3d_test.cpp)
TARGET_LINK_LIBRARIES(pose3d_test pose_state_time)

# ADD_EXECUTABLE(vqState_test vqState_test.cpp)
# TARGET_LINK_LIBRARIES(vqState_test poseStateTime)
# 
# ADD_EXECUTABLE(vqStateTs_test vqStateTs_test.cpp)
# TARGET_LINK_LIBRARIES(vqStateTs_test poseStateTime)
# 
# ADD_EXECUTABLE(wState_test wState_test.cpp)
# TARGET_LINK_LIBRARIES(wState_test poseStateTime)
