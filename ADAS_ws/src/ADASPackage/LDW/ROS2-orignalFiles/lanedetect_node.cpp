
#include "lanedetect_node.hpp"
#include <rclcpp/rclcpp.hpp>

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto lanedetect_node =
    std::make_shared<LaneDetectNode>("image", "lanedetect_image", "Hello World");
  rclcpp::spin(lanedetect_node);
  return 0;
}
