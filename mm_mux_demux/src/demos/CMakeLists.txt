##############################################################################
# Subdirectories
##############################################################################

#add_subdirectory(lib)

##############################################################################
# Executables
##############################################################################

add_executable(pubsub pubsub.cpp)
target_link_libraries(pubsub ${PROJECT_NAME})

add_executable(sample sample.cpp)
target_link_libraries(sample ${catkin_LIBRARIES})
add_executable(iproc iproc.cpp)
target_link_libraries(iproc ${catkin_LIBRARIES})

