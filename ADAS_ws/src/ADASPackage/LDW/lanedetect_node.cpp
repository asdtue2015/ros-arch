
#include "lanedetect_node.hpp"
#include <gst/gst.h>
#include <rclcpp/rclcpp.hpp>

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto watermark_node =
    std::make_shared<WatermarkNode>("image", "watermarked_image", "Heel world!");
  rclcpp::spin(watermark_node);
  return 0;
}
