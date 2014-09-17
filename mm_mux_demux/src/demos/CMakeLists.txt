##############################################################################
# Macro
##############################################################################

macro(${PROJECT_NAME}_add_demo name)
  set(target_name ${PROJECT_NAME}_${name})  # unique target names
  add_executable(${target_name} ${name}.cpp)
  set_target_properties(${target_name} PROPERTIES OUTPUT_NAME ${name})
  target_link_libraries(${target_name} ${PROJECT_NAME} ${catkin_LIBRARIES})
  install(TARGETS ${target_name} RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
endmacro()

##############################################################################
# Executables
##############################################################################

mm_mux_demux_add_demo(mm_pubsub)
mm_mux_demux_add_demo(nanomsg_pubsub)
mm_mux_demux_add_demo(nanomsg_pipeline)
mm_mux_demux_add_demo(nanomsg_services)
mm_mux_demux_add_demo(iproc)
