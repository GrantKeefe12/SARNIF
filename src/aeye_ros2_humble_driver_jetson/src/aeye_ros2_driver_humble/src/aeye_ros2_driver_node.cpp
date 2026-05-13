#include "aeye_ros2_driver_humble/aeye_ros2_driver.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

  rclcpp::executors::MultiThreadedExecutor exec;
  rclcpp::NodeOptions options;

  auto aeye_ros2_driver = std::make_shared<aeye_ros2_driver::AEyeROS2Driver>(options);
  exec.add_node(aeye_ros2_driver->get_node_base_interface());

  exec.spin();

  rclcpp::shutdown();

  return 0;
}