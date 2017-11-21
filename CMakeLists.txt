cmake_minimum_required(VERSION 2.8.3)
project(viscap_pixhawk)

find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  mavros
  mavros_msgs
  nav_msgs
  roscpp
  sensor_msgs
)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES viscap_pixhawk
#  CATKIN_DEPENDS geometry_msgs mavros mavros_msgs nav_msgs roscpp sensor_msgs
#  DEPENDS system_lib
)

include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(takeoff_landing src/takeoff_landing.cpp)
add_dependencies(takeoff_landing ${catkin_EXPORTED_TARGETS})
target_link_libraries(takeoff_landing ${catkin_LIBRARIES})

add_executable(change_mode src/change_mode.cpp)
add_dependencies(change_mode ${catkin_EXPORTED_TARGETS})
target_link_libraries(change_mode ${catkin_LIBRARIES})
