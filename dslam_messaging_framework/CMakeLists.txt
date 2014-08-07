##############################################################################
# Cmake
##############################################################################

cmake_minimum_required(VERSION 2.8.3)
project(dslam_ecto_bridge)

##############################################################################
# Find Packages
##############################################################################

find_package(OpenCV REQUIRED)
find_package(catkin REQUIRED COMPONENTS
    dslam_camera
    dslam_common
    dslam_ecto_utils
    dslam_ecto_vision
    dslam_slam
    dslam_rosmsg_conversions
    ecl_streams
    ecl_threads
    ecl_time
    ecto
    ecto_ros
    gsocket
    rostime
    sophus
    )

##############################################################################
# Catkin
##############################################################################

catkin_package(
    LIBRARIES ${PROJECT_NAME}
    CATKIN_DEPENDS
        dslam_camera
        dslam_common
        dslam_ecto_utils
        dslam_ecto_vision
        dslam_slam
        dslam_rosmsg_conversions
        ecl_streams
        ecl_threads
        ecl_time
        ecto
        ecto_ros
        gsocket
        rostime
        sophus
)
    
##############################################################################
# Project Configuration
##############################################################################

catkin_python_setup()
dslam_desktop_setup()
add_definitions(${catkin_DEFINITIONS} ${OpenCV_DEFINITIONS})
include_directories(include ${catkin_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})

##############################################################################
# Sources
##############################################################################

add_subdirectory(src)

##############################################################################
# Installs
##############################################################################

install(
    PROGRAMS 
        scripts/dslam_image_capture
    DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
    )

install(DIRECTORY launch
        DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

