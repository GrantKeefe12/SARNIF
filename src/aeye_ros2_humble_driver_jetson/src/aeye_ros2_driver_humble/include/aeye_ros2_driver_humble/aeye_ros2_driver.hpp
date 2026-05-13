#pragma once

#include <atomic>
#include <cstdio>
#include <iostream>
#include <string>
#include <thread>

#include <algorithm>
#include <unordered_map>
#include <vector>

#include "rclcpp/rclcpp.hpp"

#include "aeye_msgs_humble/srv/get_scan_patterns.hpp"
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <sensor_msgs/msg/point_field.hpp>
#include <sensor_msgs/point_cloud2_iterator.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_srvs/srv/trigger.hpp>

#include <tf2/LinearMath/Quaternion.h>
#ifdef USE_TF2_GEOMETRY_MSGS_HPP
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#else
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#endif
#include <tf2_ros/transform_broadcaster.h>

#include <spdlog/spdlog.h>

#include "AEyeSDKConfig.hpp"
#include "AEyeSDKStructures.hpp"
#include "PointCloudFrame.hpp"
#include "PointStructure.hpp"
#include "Sensor.hpp"

namespace aeye_ros2_driver {

class AEyeROS2Driver {
  public:
    AEyeROS2Driver(const rclcpp::NodeOptions &options);

    rclcpp::node_interfaces::NodeBaseInterface::SharedPtr
    get_node_base_interface() {
        return node_->get_node_base_interface();
    }

    void aeye_point_cloud_callback(aeye::PointCloudFrame &&point_cloud_frame);

  private:
    void handle_get_scan_patterns(
        const std::shared_ptr<aeye_msgs_humble::srv::GetScanPatterns::Request> request,
        std::shared_ptr<aeye_msgs_humble::srv::GetScanPatterns::Response> response);

    void setMultiEchoConfiguration(bool return_1_enabled, bool return_2_enabled);

    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_point_cloud_;

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr set_scan_pattern_sub_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr set_multiEcho_return_sub_;

    rclcpp::Service<aeye_msgs_humble::srv::GetScanPatterns>::SharedPtr get_scan_patterns_srv_;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr get_current_scan_pattern_srv_;

    rclcpp::Node::SharedPtr node_;
    std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;

    std::atomic<size_t> count_ = 0;
    const size_t max_frames_ = std::numeric_limits<size_t>::max();

    std::string nic_;
    std::string ip_;
    std::string sensor_name_;
    std::shared_ptr<aeye::Sensor> sensor_;

    std::unordered_map<std::string, int> scan_patterns_on_sensor_;

    const std::string frame_id_ = "aeye";
    const float intensity_factor_ = 65535.0;
};

} // namespace aeye_ros2_driver