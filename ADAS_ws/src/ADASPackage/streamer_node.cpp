
#include "streamer_node.hpp"
#include <rclcpp/rclcpp.hpp>


int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

 /* Initialize GStreamer */
  auto streamer_node = std::make_shared<Streamer>("lanedetect_image");
  rclcpp::spin(streamer_node);
  return 0;
}
