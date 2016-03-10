
#include "camera_node.hpp"

#include <rclcpp/rclcpp.hpp>

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto camera_node = std::make_shared<CameraNode>("image");
  rclcpp::spin(camera_node);
  return 0;
}
