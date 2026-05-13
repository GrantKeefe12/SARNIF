cmake_minimum_required(VERSION 3.10)
project(aeye_ros2_driver_humble)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(NOT_MSVC $<NOT:$<CXX_COMPILER_ID:MSVC>>)

set(AEYE_SDK_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../sdk_build")
set(AEyeSensorSDK_DIR "${AEYE_SDK_ROOT}/lib/cmake/libAEyeSensorSDK")
find_package(AEyeSensorSDK REQUIRED)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)
find_package(aeye_msgs_humble REQUIRED)

find_path(TF2_GEOMETRY_MSGS_HPP_PATH
  NAMES tf2_geometry_msgs.hpp
  PATHS ${tf2_geometry_msgs_INCLUDE_DIRS}
  PATH_SUFFIXES tf2_geometry_msgs
  NO_DEFAULT_PATH
)

if(TF2_GEOMETRY_MSGS_HPP_PATH)
  message(STATUS "Found tf2_geometry_msgs.hpp - using modern header")
  add_compile_definitions(USE_TF2_GEOMETRY_MSGS_HPP)
else()
  message(STATUS "tf2_geometry_msgs.hpp not found - using legacy header")
endif()

find_package(fmt QUIET)
if(fmt_FOUND)
  message(STATUS "Found fmt library - linking for compatibility")
  set(FMT_LIBRARIES fmt::fmt)
  set(FMT_DEPENDENCIES fmt)
else()
  message(STATUS "fmt library not found - proceeding without it")
  set(FMT_LIBRARIES "")
  set(FMT_DEPENDENCIES "")
endif()

add_library(
  aeye_ros2_driver_humble SHARED
  src/aeye_ros2_driver.cpp
)

target_include_directories(aeye_ros2_driver_humble PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  ${Python3_INCLUDE_DIRS}
  ${pybind11_INCLUDE_DIRS}
  ${AEYE_SDK_ROOT}/include
  /usr/local/include
)

target_compile_definitions(aeye_ros2_driver_humble
  PRIVATE "COMPOSITION_BUILDING_DLL"
)

ament_target_dependencies(
  aeye_ros2_driver_humble
  rclcpp
  rclcpp_components
  std_msgs
  std_srvs
  sensor_msgs
  tf2_ros
  tf2_geometry_msgs
  geometry_msgs
  Python3
  pybind11
  aeye_msgs_humble
  ${FMT_DEPENDENCIES}
)

target_link_libraries(aeye_ros2_driver_humble
  ${Python3_LIBRARIES}
  ${AEYE_SDK_ROOT}/lib/libAEyeSensorSDK.a
  ${FMT_LIBRARIES}
)

rclcpp_components_register_nodes(aeye_ros2_driver_humble "aeye_ros2_driver::AEyeROS2Driver")
set(node_plugins "${node_plugins}aeye_ros2_driver::AEyeROS2Driver;$<TARGET_FILE:aeye_ros2_driver_humble>\n")

add_executable(aeye_ros2_driver_humble_node src/aeye_ros2_driver_node.cpp)

target_include_directories(aeye_ros2_driver_humble_node PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
  ${Python3_INCLUDE_DIRS}
  ${pybind11_INCLUDE_DIRS}
  ${AEYE_SDK_ROOT}/include
  /usr/local/include
)

target_link_libraries(aeye_ros2_driver_humble_node
  aeye_ros2_driver_humble
  ${Python3_LIBRARIES}
  ${AEYE_SDK_ROOT}/lib/libAEyeSensorSDK.a
  ${FMT_LIBRARIES}
)

ament_target_dependencies(aeye_ros2_driver_humble_node
  rclcpp
  Python3
  pybind11
  aeye_msgs_humble
  ${FMT_DEPENDENCIES}
)

install(TARGETS
  aeye_ros2_driver_humble_node
  DESTINATION lib/${PROJECT_NAME}
)

install(TARGETS
  aeye_ros2_driver_humble
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

install(
  DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch
)

install(
  DIRECTORY rviz/
  DESTINATION share/${PROJECT_NAME}/rviz
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()