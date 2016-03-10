#include <rclcpp/rclcpp.hpp>
#include <memory>

#include "InputStream/camera_node.hpp"
#include "VideoServer/streamer_node.hpp"
#include "LDW/main_LaneDetectorSim.h"
#include "FCW/detector.hpp"


using namespace LaneDetectorSim;

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::executors::SingleThreadedExecutor executor;
  rclcpp::executors::MultiThreadedExecutor mexecutor;


  Arguments args_LDW= Arguments(1,1,61,0.0,0.1);
  Args args_FCW = Args();


  // Connect the nodes as a pipeline: camera_node -> lanedetect_node-> streamer_node
  auto camera_node = std::make_shared<CameraNode>("image");
  auto lanedetect_node = std::make_shared<LaneDetectNode>("image", "lanedetect_image", args_LDW);
  auto detector_node = std::make_shared<App>("image", "detector_image", args_FCW);
  auto streamer_node =   std::make_shared<Streamer>("detector_image");

  executor.add_node(camera_node);
  executor.add_node(lanedetect_node);
  executor.add_node(detector_node);
  executor.add_node(streamer_node);

  executor.spin();
  return 0;
}
