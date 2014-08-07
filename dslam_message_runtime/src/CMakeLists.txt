##############################################################################
# Subdirectories
##############################################################################

add_subdirectory(lib)
add_executable(pubsub pubsub.cpp)
target_link_libraries(pubsub ${PROJECT_NAME})
add_executable(sample sample.cpp)
target_link_libraries(sample ${catkin_LIBRARIES})