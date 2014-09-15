##############################################################################
# Subdirectories
##############################################################################

#add_subdirectory(lib)

##############################################################################
# Executables
##############################################################################

add_executable(mm_pubsub mm_pubsub.cpp)
target_link_libraries(mm_pubsub ${PROJECT_NAME})

add_executable(nanomsg_pubsub nanomsg_pubsub.cpp)
target_link_libraries(nanomsg_pubsub ${catkin_LIBRARIES})
add_executable(nanomsg_pipeline nanomsg_pipeline.cpp)
target_link_libraries(nanomsg_pipeline ${catkin_LIBRARIES})
add_executable(nanomsg_services nanomsg_services.cpp)
target_link_libraries(nanomsg_services ${catkin_LIBRARIES})

add_executable(iproc iproc.cpp)
target_link_libraries(iproc ${catkin_LIBRARIES})

