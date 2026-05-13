#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/imgproc/imgproc.hpp>

class BayerToRGBNode : public rclcpp::Node {
public:
    BayerToRGBNode() : Node("bayer_to_rgb_node") {
        const auto input_topic = this->declare_parameter<std::string>(
            "input_topic", "/arena_camera_node/images");
        const auto output_topic = this->declare_parameter<std::string>(
            "output_topic", "rgb_image");
        bayer_pattern_ = this->declare_parameter<std::string>("bayer_pattern", "bggr");
        output_encoding_ = this->declare_parameter<std::string>("output_encoding", "rgb8");

        auto sub_qos = rclcpp::SensorDataQoS();
        auto pub_qos = rclcpp::QoS(rclcpp::KeepLast(10)).reliable();
        image_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
            input_topic, sub_qos,
            std::bind(&BayerToRGBNode::imageCallback, this, std::placeholders::_1));
        
        image_pub_ = this->create_publisher<sensor_msgs::msg::Image>(output_topic, pub_qos);
        RCLCPP_INFO(this->get_logger(),
                    "Subscribing: %s | Publishing: %s | bayer_pattern: %s | output_encoding: %s",
                    input_topic.c_str(), output_topic.c_str(),
                    bayer_pattern_.c_str(), output_encoding_.c_str());
    }

private:
    int getDebayerCode() const {
        const bool as_rgb = (output_encoding_ == "rgb8");

        if (bayer_pattern_ == "rggb") {
            return as_rgb ? cv::COLOR_BayerRG2RGB : cv::COLOR_BayerRG2BGR;
        }
        if (bayer_pattern_ == "bggr") {
            return as_rgb ? cv::COLOR_BayerBG2RGB : cv::COLOR_BayerBG2BGR;
        }
        if (bayer_pattern_ == "grbg") {
            return as_rgb ? cv::COLOR_BayerGR2RGB : cv::COLOR_BayerGR2BGR;
        }
        if (bayer_pattern_ == "gbrg") {
            return as_rgb ? cv::COLOR_BayerGB2RGB : cv::COLOR_BayerGB2BGR;
        }

        return as_rgb ? cv::COLOR_BayerBG2RGB : cv::COLOR_BayerBG2BGR;
    }

    void imageCallback(const sensor_msgs::msg::Image::SharedPtr msg) {
        try {
            // Convert the ROS image message to an OpenCV image
            cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, "bayer_rggb8");
            
            cv::Mat color_image;
            cv::cvtColor(cv_ptr->image, color_image, getDebayerCode());
            
            auto rgb_msg = cv_bridge::CvImage(msg->header, output_encoding_, color_image).toImageMsg();
            image_pub_->publish(*rgb_msg);
        } catch (cv_bridge::Exception &e) {
            RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
        }
    }

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_sub_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr image_pub_;
    std::string bayer_pattern_;
    std::string output_encoding_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<BayerToRGBNode>());
    rclcpp::shutdown();
    return 0;
}
