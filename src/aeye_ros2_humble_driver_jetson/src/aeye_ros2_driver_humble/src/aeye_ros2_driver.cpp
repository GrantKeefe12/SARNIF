#include <stdexcept>
#include <cstdio>
#include <memory>
#include <string>
#include <sstream>

#include "aeye_ros2_driver_humble/aeye_ros2_driver.hpp"

namespace aeye_ros2_driver {

std::string getNIC(const std::string& ipAddress) {
    std::string command = "ip -o route get " + ipAddress + " | awk '{for(i=1;i<=NF;i++) if ($i==\"dev\") print $(i+1)}'";
    std::string result;
    char buffer[128];

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }

    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }

    result.erase(result.find_last_not_of(" \n\r\t") + 1);

    return result;
}

AEyeROS2Driver::AEyeROS2Driver(const rclcpp::NodeOptions &options)
    : node_(std::make_shared<rclcpp::Node>("aeye_ros2_driver_humble", options)),
      tf_broadcaster_(std::make_shared<tf2_ros::TransformBroadcaster>(node_)) {

    ip_ = node_->declare_parameter<std::string>("sensor_ip", "10.10.10.108");
    sensor_name_ = node_->declare_parameter<std::string>("sensor_name", "Apollo_B0");

    RCLCPP_INFO(node_->get_logger(), "Using IP: %s, Sensor Name: %s", ip_.c_str(), sensor_name_.c_str());

    bool auto_network_configuration = node_->declare_parameter<bool>("auto_network_configuration", true);

    RCLCPP_INFO(node_->get_logger(), "Automatic Network Configuration: %s",
                auto_network_configuration ? "enabled" : "disabled");

    bool return_1_enabled = node_->declare_parameter<bool>("return_1", true);
    bool return_2_enabled = node_->declare_parameter<bool>("return_2", true);

    RCLCPP_INFO(node_->get_logger(), "Multi-echo returns - Return 1: %s, Return 2: %s",
                return_1_enabled ? "enabled" : "disabled", return_2_enabled ? "enabled" : "disabled");

    try {
        nic_ = getNIC(ip_);
        if (nic_.empty()) {
            RCLCPP_ERROR(node_->get_logger(), "NIC for IP address %s not found!", ip_.c_str());
            throw std::runtime_error("NIC retrieval failed");
        }
        RCLCPP_INFO(node_->get_logger(), "NIC for IP %s is %s", ip_.c_str(), nic_.c_str());
    } catch (const std::exception& e) {
        RCLCPP_FATAL(node_->get_logger(), "Error retrieving NIC: %s", e.what());
        throw;
    }

    aeye::DataPortConfig ports;
    if (ip_ == "127.0.0.1") {
        ports.points = 7101;
    }

    sensor_ = std::make_shared<aeye::Sensor>(
        aeye::SensorConfig{}, ports, nic_, ip_, sensor_name_);

    pub_point_cloud_ = node_->create_publisher<sensor_msgs::msg::PointCloud2>(
        "/aeye_point_cloud", 10);

    set_scan_pattern_sub_ = node_->create_subscription<std_msgs::msg::String>(
        "set_scan_pattern", 10,
        [this](const std_msgs::msg::String::SharedPtr msg) {
            auto it = scan_patterns_on_sensor_.find(msg->data);
            if (it != scan_patterns_on_sensor_.end()) {
                sensor_->sendShotlistFileNumber(it->second);
                RCLCPP_INFO(node_->get_logger(), "Scan pattern updated: %s", msg->data.c_str());
            } else {
                RCLCPP_WARN(node_->get_logger(), "Invalid scan pattern: %s", msg->data.c_str());
            }
        });

    set_multiEcho_return_sub_ = node_->create_subscription<std_msgs::msg::String>(
        "set_multiEcho_return", 10,
        [this](const std_msgs::msg::String::SharedPtr msg) {
            std::string config = msg->data;
            bool return_1_fromPanel = config.find("return1:true") != std::string::npos;
            bool return_2_fromPanel = config.find("return2:true") != std::string::npos;
            setMultiEchoConfiguration(return_1_fromPanel, return_2_fromPanel);
        });

    sensor_->setOnPointCloudFrameCallback(
        std::bind(&AEyeROS2Driver::aeye_point_cloud_callback, this,
                  std::placeholders::_1));
    
    try {
        sensor_->connect(auto_network_configuration);
        RCLCPP_INFO(node_->get_logger(), "Successfully connected to sensor at %s", ip_.c_str());

        setMultiEchoConfiguration(return_1_enabled, return_2_enabled);

        int size = static_cast<int>(sensor_->getScanPatternFileNames().size());
        for (int i = 0; i < size; i++) {
            std::string shotlist_name = sensor_->getShotlistName(i);
            scan_patterns_on_sensor_.insert({shotlist_name, i});
        }
    } catch (const std::exception& e) {
        RCLCPP_ERROR(node_->get_logger(), "Failed to connect to sensor at %s: %s", ip_.c_str(), e.what());
        RCLCPP_WARN(node_->get_logger(), "Make sure the fake sensor is running if using 127.0.0.1");
        RCLCPP_WARN(node_->get_logger(), "Run: ./run_fake_sensor.sh start");
        throw;
    }

    get_scan_patterns_srv_ = node_->create_service<aeye_msgs_humble::srv::GetScanPatterns>(
        "get_scan_patterns",
        std::bind(&AEyeROS2Driver::handle_get_scan_patterns, this,
                  std::placeholders::_1, std::placeholders::_2));

    get_current_scan_pattern_srv_ = node_->create_service<std_srvs::srv::Trigger>(
        "get_current_scan_pattern",
        [this](const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
               std::shared_ptr<std_srvs::srv::Trigger::Response> response) {
            response->success = true;
            response->message = sensor_->getShotlistName(0);
        });

    try {
        auto shotlist_index = 0;
        auto name = sensor_->getShotlistName(shotlist_index);
        printf("Running with scan pattern: %s.\n", name.c_str());
        sensor_->sendShotlistFileNumber(shotlist_index);
    } catch (const std::exception& e) {
        RCLCPP_WARN(node_->get_logger(), "Could not set scan pattern: %s", e.what());
        RCLCPP_WARN(node_->get_logger(), "Continuing without scan pattern (fake sensor mode)");
    }

    try {
        sensor_->start();
    } catch (const std::exception& e) {
        RCLCPP_ERROR(node_->get_logger(), "Failed to start sensor: %s", e.what());
        if (ip_ == "127.0.0.1") {
            RCLCPP_WARN(node_->get_logger(), "Note: The fake sensor may have limited functionality");
            RCLCPP_WARN(node_->get_logger(), "Continuing anyway for visualization testing");
        } else {
            throw;
        }
    }
}

void AEyeROS2Driver::handle_get_scan_patterns(
    const std::shared_ptr<aeye_msgs_humble::srv::GetScanPatterns::Request> request,
    std::shared_ptr<aeye_msgs_humble::srv::GetScanPatterns::Response> response) {

    for (const auto &[name, index] : scan_patterns_on_sensor_) {
        response->scan_patterns.push_back(name);
    }
}

void AEyeROS2Driver::setMultiEchoConfiguration(bool return_1, bool return_2) {
    uint8_t multiEchoSelection = 0;
    if (return_1) multiEchoSelection |= 0b0001u;
    if (return_2) multiEchoSelection |= 0b0010u;

    try {
        sensor_->sendNumMultiEchoes(multiEchoSelection);
        RCLCPP_INFO(node_->get_logger(), "Multi-echo configuration updated: Return1=%s, Return2=%s",
                   return_1 ? "enabled" : "disabled", return_2 ? "enabled" : "disabled");
    } catch (const std::exception& e) {
        RCLCPP_ERROR(node_->get_logger(), "Failed to update multi-echo config: %s", e.what());
    }
}

void AEyeROS2Driver::aeye_point_cloud_callback(
    aeye::PointCloudFrame &&point_cloud_frame) {
    const int frame_id = point_cloud_frame.getFrameId();
    if (frame_id == 0)
        return;

    std::vector<aeye::AEyePoint> points = point_cloud_frame.getPoints();
    unsigned int N = points.size();
    if (N > 0) {
        auto ros_point_cloud = sensor_msgs::msg::PointCloud2();
        ros_point_cloud.height = 1;
        ros_point_cloud.width = N;
        ros_point_cloud.header.frame_id = frame_id_;
        ros_point_cloud.header.stamp = node_->get_clock()->now();
        sensor_msgs::PointCloud2Modifier modifier(ros_point_cloud);

        modifier.setPointCloud2Fields(8,
            "azimuth", 1, sensor_msgs::msg::PointField::FLOAT32,
            "elevation", 1, sensor_msgs::msg::PointField::FLOAT32,
            "range", 1, sensor_msgs::msg::PointField::FLOAT32,
            "x", 1, sensor_msgs::msg::PointField::FLOAT32,
            "y", 1, sensor_msgs::msg::PointField::FLOAT32,
            "z", 1, sensor_msgs::msg::PointField::FLOAT32,
            "reflectivity", 1, sensor_msgs::msg::PointField::FLOAT32,
            "echo_index", 1, sensor_msgs::msg::PointField::UINT8
        );

        sensor_msgs::PointCloud2Iterator<float> azimuth_it(ros_point_cloud, "azimuth");
        sensor_msgs::PointCloud2Iterator<float> elevation_it(ros_point_cloud, "elevation");
        sensor_msgs::PointCloud2Iterator<float> range_it(ros_point_cloud, "range");
        sensor_msgs::PointCloud2Iterator<float> x_it(ros_point_cloud, "x");
        sensor_msgs::PointCloud2Iterator<float> y_it(ros_point_cloud, "y");
        sensor_msgs::PointCloud2Iterator<float> z_it(ros_point_cloud, "z");
        sensor_msgs::PointCloud2Iterator<float> intensity_type_data_it(ros_point_cloud, "reflectivity");
        sensor_msgs::PointCloud2Iterator<uint8_t> echo_index_it(ros_point_cloud, "echo_index");

        geometry_msgs::msg::TransformStamped transform_stamped;
        transform_stamped.header.stamp = ros_point_cloud.header.stamp;
        transform_stamped.header.frame_id = frame_id_;
        transform_stamped.child_frame_id = "aeye_child";

        auto pose = sensor_->getPose();
        transform_stamped.transform.translation.x = pose.x;
        transform_stamped.transform.translation.y = pose.y;
        transform_stamped.transform.translation.z = pose.z;
        tf2::Quaternion q;
        q.setRPY(pose.roll, pose.pitch, pose.yaw);
        transform_stamped.transform.rotation = tf2::toMsg(q);

        for (const auto &point : points) {
            *azimuth_it = point.pointReturnSphericalCoords.azimuth;
            *elevation_it = point.pointReturnSphericalCoords.elevation;
            *range_it = point.pointReturnSphericalCoords.radius;
            *x_it = point.pointReturnCartesianCoords.x;
            *y_it = point.pointReturnCartesianCoords.y;
            *z_it = point.pointReturnCartesianCoords.z;
            *intensity_type_data_it = point.pointReturnIntensity.intensity_type_data;
            *echo_index_it = static_cast<uint8_t>(point.pointReturnType.point_return_mask & 0b11u);

            ++azimuth_it;
            ++elevation_it;
            ++range_it;
            ++x_it;
            ++y_it;
            ++z_it;
            ++intensity_type_data_it;
            ++echo_index_it;
        }

        tf_broadcaster_->sendTransform(transform_stamped);
        pub_point_cloud_->publish(std::move(ros_point_cloud));
    }
}

} // namespace aeye_ros2_driver