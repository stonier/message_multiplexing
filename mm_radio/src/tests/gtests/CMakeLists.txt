###############################################################################
# Test Macro
###############################################################################

macro(${PROJECT_NAME}_add_gtest test_name)
  set(target_name ${PROJECT_NAME}_test_${test_name})
  catkin_add_gtest(
      ${target_name}
      ${test_name}.cpp
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )
  # If we don't have gtest installed, then the target doesn't get made - check!
  if(TARGET ${target_name})
      target_link_libraries(${target_name} ${PROJECT_NAME} ${catkin_LIBRARIES})
  endif()
endmacro()

###############################################################################
# Google Tests
###############################################################################

mm_radio_add_gtest(radio)
