cmake_minimum_required(VERSION 3.0.2)
project(pkg4)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  control_msgs
  sensor_msgs
  actionlib
)

catkin_package()

include_directories(${catkin_INCLUDE_DIRS})

add_executable(prova4 src/prova4.cpp)
add_executable(prova4b src/prova4b.cpp)

target_link_libraries(prova4 
      ${catkin_LIBRARIES})

target_link_libraries(prova4b 
      ${catkin_LIBRARIES})
