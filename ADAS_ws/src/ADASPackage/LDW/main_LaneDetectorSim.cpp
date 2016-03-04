
#include"main_LaneDetectorSim.h"
using namespace::LaneDetectorSim;


int main(int argc, char ** argv)
{

	rclcpp::init(argc, argv);

	Arguments args= Arguments(1,1,61,0.0,0.1);

	//LDW ldw_node;
	//auto lanedetect_node =
	 //   std::make_shared<LDW>("image", "lanedetect_image", args);

	//rclcpp::spin(lanedetect_node);

  return 0;
}
