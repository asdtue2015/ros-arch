
#ifndef LaneDetectorSim_main_h
#define LaneDetectorSim_main_h
#include "Process_LaneDetectorSim.h"
#include <string>
#include <chrono>
#include <string>
#include <thread>
#include <memory>
#include "opencv2/highgui/highgui.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "../Aux/common.hpp"
#include <stdexcept>



/* Time */
extern const int    NUM_WINDOW_EWM      = 5;    //EWMA, EWVAR Init (times)
/* Size of Image */
extern const double COEF                = 1;
/* Multi-Image Show */
extern const int    WIN_COLS            = 3;
extern const int    WIN_ROWS            = 3;
extern const int    IMAGE_RECORD = 1;

/* Run applicaiton */

/* Record docs */
//extern const char   LANE_RECORD_FILE[]  = "./inputdata/outputdata/LaneFeatures_22-03-2014_13h05m12s.txt";
//extern const char   FILE_LANE_FEATURE[] = "./inputdata/outputdata/Sim_LaneFeatures_22-03-2014_13h05m12s.txt";
extern const char   LANE_RECORD_IMAGE[]    = "../inputdata/outputdata/lane_%d.png";
/* Data Source */
//extern const char   LANE_RAW_NAME[]     = "./inputdata/cropped_images/cropped_%d.png";
//extern const char   LANE_RAW_NAME[]     = "./inputdata/washington/lane_%d.png";
// extern const char   LANE_RAW_NAME[]     = "/home/lixp/Developer/KIT/2011_09_26/2011_09_26_drive_0015_sync/image_00/data/%010d.png";
// extern const char   LANE_RAW_NAME[]     = "/home/lixp/Developer/Data/LaneRaw_10-07-2013_18h30m21s/lane_%d.jpg";


extern const int    TH_KALMANFILTER     = 1; // originally 1


namespace LaneDetectorSim {



class Arguments

{

public:
	Arguments(int lanedetector, int startframe, int endframe, double yawangle,double pitchangle )

	{
		LANE_DETECTOR = lanedetector;
		StartFrame    = startframe;
		EndFrame      = endframe;
		YAW_ANGLE     = yawangle;
		PITCH_ANGLE   = pitchangle;
	}

	int LANE_DETECTOR = 1;
    int StartFrame = 1;
    int EndFrame = 61;
    double YAW_ANGLE = 0.0;
	double PITCH_ANGLE = 0.1;
};






	class LaneDetectNode : public rclcpp::Node
	{

	public:

	LaneDetectNode (
	    const std::string & input, const std::string & output, Arguments args,
	    const std::string & node_name = "lanedetect_node")
	  : Node(node_name, true)

	  {
			this->Args=args;
			auto qos = rmw_qos_profile_sensor_data;

		      execTime          = 0;  // Execute Time for Each Frame
			  lastLateralOffset = 0;
			  lateralOffset     = 0;    // Lateral Offset
			  detectLaneFlag    = -1;   // init state -> normal state 0
			  isChangeLane      = 0;    // Whether lane change happens
			  changeDone        = 0;    // Finish lane change
			  muWindowSize      = 5;    // Initial window size: 5 (sample)
			  sigmaWindowSize   = 5;    // Initial window size: 5 (sample)






			  InitlaneFeatures(laneFeatures);


		      // Create a publisher on the output topic.
			  pub_ = this->create_publisher<sensor_msgs::msg::Image>(output, qos);
			  std::weak_ptr<std::remove_pointer<decltype(pub_.get())>::type> captured_pub = pub_;

			  // Create a subscription on the input topic.
			  sub_ = this->create_subscription<sensor_msgs::msg::Image>(input, [this, captured_pub](sensor_msgs::msg::Image::UniquePtr msg)
					  	  {
				   	      	  auto pub_ptr = captured_pub.lock();
				   	      	  if (!pub_ptr) {return;}

				   	          // Create a cv::Mat from the image message (without copying).
				   	      	  cv::Mat cvMat( msg->width, msg->height,encoding2mat_type(msg->encoding), msg->data.data());

				   	      	  laneMat=cvMat;


				   	      	  if (Args.LANE_DETECTOR && !init)
				   	      	      {

				   	      		  	  	  std::cout << "/*************************************/" << std::endl;
				   	      				  std::cout << "xxxInput LANE_DETECTOR" << Args.LANE_DETECTOR << std::endl;
				   	      				  std::cout << "xxxInput StartFrame" << Args.StartFrame << std::endl;
				   	      				  std::cout << "xxxInput EndFrame" << Args.EndFrame << std::endl;
				   	      				  std::cout << "xxxInput YAW_ANGLE" << Args.YAW_ANGLE << std::endl;
				   	      				  std::cout << "xxxInput PITCH_ANGLE" << Args.PITCH_ANGLE << std::endl;
				   	      				  std::cout << "/*************************************/" << std::endl;

				   	      	         /* Lane detect and tracking */
				   	      	            LaneDetector::InitlaneDetectorConf(laneMat, laneDetectorConf, 2); // KIT 1, ESIEE 2
				   	      	            LaneDetector::InitLaneKalmanFilter(laneKalmanFilter, laneKalmanMeasureMat, laneKalmanIdx);
				   	      	            init=true; //poisonous
				   	      	      }


				   	      	   if (Args.LANE_DETECTOR)
				   	      	     {

				   	      	       	 startTime = (double)cv::getTickCount();

				   	      	         LaneDetectorSim::ProcessLaneImage(laneMat, laneDetectorConf, startTime, laneKalmanFilter, laneKalmanMeasureMat, laneKalmanIdx, hfLanes, lastHfLanes,
				   	      	         lastLateralOffset, lateralOffset, isChangeLane, detectLaneFlag,  idx, execTime, preHfLanes, changeDone, Args.YAW_ANGLE, Args.PITCH_ANGLE);
				   	      	     }
				   	      	    if(IMAGE_RECORD)
				   	      	      {
				   	      	          char *text = new char[100];
				   	      	          sprintf(text, LANE_RECORD_IMAGE, idx);
				   	      	          cv::imwrite(text, laneMat);
				   	      	          delete text;
				   	      	       }

				   	      	     char *text = new char[30];
				   	      	          cv::putText(laneMat, text, cv::Point(0, laneMat.rows-5), cv::FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(0,255,0));
				   	      	          cv::putText(laneMat, text, cv::Point(0, 60), cv::FONT_HERSHEY_SIMPLEX, 0.8, CV_RGB(0, 255, 0));
				   	      	          delete text;

				   	      	           //cv::imshow("Lane System", laneMat);
				   	      	           //cv::moveWindow("Lane System", 790, 30);
				   	      	           //cv::waitKey(1);



				   	      	  pub_ptr->publish(msg);    // Publish it along.

				   	    }, qos);


	  }

	private:
	  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr pub_;
	  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_;
	  cv::Mat laneMat;
	  cv::KalmanFilter laneKalmanFilter= cv::KalmanFilter(8, 8, 0);
	  cv::Mat laneKalmanMeasureMat= cv::Mat(8, 1, CV_32F, cv::Scalar::all(0));

	  /*More Sensible initialisation */
	  Arguments Args= Arguments(0,0,0,0,0);

	  int  idx;  //index for image sequence

	  double execTime;  // Execute Time for Each Frame
	   //double pastTime;


	  bool  init=false;


	 std::ofstream laneFeatureFile;

	 /* Parameters for Lane Detector */
	 LaneDetector::LaneDetectorConf laneDetectorConf;
	 std::vector<cv::Vec2f> hfLanes;
	 std::vector<cv::Vec2f> lastHfLanes;
	 std::vector<cv::Vec2f> preHfLanes;

	 std::vector<double> LATSDBaselineVec;
	 std::deque<LaneDetector::InfoCar> lateralOffsetDeque;
	 std::deque<LaneDetector::InfoCar> LANEXDeque;
	 std::deque<LaneDetector::InfoTLC> TLCDeque;

	 LaneDetector::LaneFeature laneFeatures;
	 double lastLateralOffset;
	 double lateralOffset;          // Lateral Offset
	 int    detectLaneFlag;        // init state -> normal state 0
	 int    isChangeLane;         // Whether lane change happens
	 int    changeDone;          // Finish lane change
	 int    muWindowSize;       // Initial window size: 5 (sample)
	 int    sigmaWindowSize;   // Initial window size: 5 (sample)


	 std::vector<float> samplingTime;
	 double startTime;
	 int    laneKalmanIdx;    //Marker of start kalmam



	};


}

#endif
