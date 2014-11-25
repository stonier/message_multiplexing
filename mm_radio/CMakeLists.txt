##############################################################################
# Cmake
##############################################################################

cmake_minimum_required(VERSION 2.8.3)
project(mm_radio)

##############################################################################
# Find Packages
##############################################################################

find_package(catkin REQUIRED COMPONENTS
    ecl_build
    ecl_command_line
    ecl_exceptions
    ecl_threads
    ecl_time
    ecl_utilities
    mm_core_msgs
    mm_messages
    nanomsg
)

##############################################################################
# Catkin
##############################################################################

catkin_package(
    INCLUDE_DIRS include
    LIBRARIES ${PROJECT_NAME}
    CATKIN_DEPENDS
        ecl_build
        ecl_command_line
        ecl_exceptions
        ecl_threads
        ecl_time
        ecl_utilities
        mm_core_msgs
        mm_messages
        nanomsg
)

##############################################################################
# Project Configuration
##############################################################################

# upgrade to c11 as soon as we are able.
ecl_enable_cxx11_compiler()
add_definitions(${catkin_DEFINITIONS})
include_directories(include ${catkin_INCLUDE_DIRS})

##############################################################################
# Sources
##############################################################################

add_subdirectory(include)
add_subdirectory(src)
#add_subdirectory(tests)

##############################################################################
# Installs
##############################################################################

#install(
#    PROGRAMS 
#        scripts/foo
#    DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
#    )

#install(DIRECTORY launch
#        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
#)

