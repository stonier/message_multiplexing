##############################################################################
# Macro
##############################################################################

macro(${PROJECT_NAME}_add_demo name)
  set(target_name ${PROJECT_NAME}_${name})  # unique target names
  add_executable(${target_name} ${name}.cpp)
  set_target_properties(${target_name} PROPERTIES OUTPUT_NAME ${name})
  target_link_libraries(${target_name} ${PROJECT_NAME} ${catkin_LIBRARIES})
endmacro()

##############################################################################
# Executables
##############################################################################

mm_radio_add_demo(mm_radio)
mm_radio_add_demo(nanomsg_radio)
