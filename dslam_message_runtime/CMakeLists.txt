##############################################################################
# Cmake
##############################################################################

cmake_minimum_required(VERSION 2.8.3)
project(dslam_message_runtime)

##############################################################################
# Find Packages
##############################################################################

find_package(catkin REQUIRED COMPONENTS
    ecl_build
    ecl_threads
    ecl_time
    ecl_utilities
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
        ecl_threads
        ecl_time
        ecl_utilities
        nanomsg
)

##############################################################################
# Project Configuration
##############################################################################

ecl_enable_cxx0x_compiler()
add_definitions(${catkin_DEFINITIONS})
include_directories(include ${catkin_INCLUDE_DIRS})

##############################################################################
# Sources
##############################################################################

add_subdirectory(src)

##############################################################################
# Installs
##############################################################################

#install(
#    PROGRAMS 
#        scripts/dslam_image_capture
#    DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
#    )

#install(DIRECTORY launch
#        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
#)

