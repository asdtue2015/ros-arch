#include <rclcpp/rclcpp.hpp>
#include <memory>

#include "InputStream/camera_node.hpp"
#include "VideoServer/streamer_node.hpp"
#include "LDW/lanedetect_node.hpp"

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::executors::SingleThreadedExecutor executor;

  // Connect the nodes as a pipeline: camera_node -> lanedetect_node-> streamer_node
  auto camera_node = std::make_shared<CameraNode>("image");
  auto watermark_node =
    std::make_shared<WatermarkNode>("image", "watermarked_image", "Heel world!");
  auto image_view_node = std::make_shared<ImageViewNode>("watermarked_image");

  executor.add_node(camera_node);
  executor.add_node(watermark_node);
  executor.add_node(image_view_node);

  executor.spin();
  return 0;
}
