
#ifndef IMAGE_PIPELINE__WATERMARK_NODE_HPP_
#define IMAGE_PIPELINE__WATERMARK_NODE_HPP_

#include <string>
#include <chrono>
#include <string>
#include <thread>
#include <memory>
#include "opencv2/highgui/highgui.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "../Aux/common.hpp"
/*Node that receives an image,performs lane detection and publishes it again. */

class LaneDetectNode : public rclcpp::Node
{
public:
	LaneDetectNode(
    const std::string & input, const std::string & output, const std::string & text,
    const std::string & node_name = "lanedetect_node")
  : Node(node_name, true)

  {

	auto qos = rmw_qos_profile_sensor_data;



    // Create a publisher on the input topic.
    pub_ = this->create_publisher<sensor_msgs::msg::Image>(output, qos);
    std::weak_ptr<std::remove_pointer<decltype(pub_.get())>::type> captured_pub = pub_;



    // Create a subscription on the output topic.
    sub_ = this->create_subscription<sensor_msgs::msg::Image>(
      input, [captured_pub, text](sensor_msgs::msg::Image::UniquePtr msg) {
      auto pub_ptr = captured_pub.lock();
      if (!pub_ptr) {
        return;
      }
      // Create a cv::Mat from the image message (without copying).
      cv::Mat cv_mat(
        msg->width, msg->height,
        encoding2mat_type(msg->encoding),
        msg->data.data());
      // Annotate the image with the pid, pointer address, and the watermark text.
      std::stringstream ss;
      ss << "pid: " << GETPID() << ", ptr: " << msg.get() << " " << text;
      draw_on_image(cv_mat, ss.str(), 40);
      pub_ptr->publish(msg);    // Publish it along.
    }, qos);
  }

private:
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr pub_;
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_;

  cv::VideoCapture cap_;
  cv::Mat frame_;
};

#endif  // IMAGE_PIPELINE__WATERMARK_NODE_HPP_
