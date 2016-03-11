
#ifndef IMAGE_PIPELINE__CAMERA_NODE_HPP_
#define IMAGE_PIPELINE__CAMERA_NODE_HPP_

#include <chrono>
#include <ctime>
#include <sstream>
#include <string>
#include <thread>
#include <memory>
#include "opencv2/highgui/highgui.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "../Aux/common.hpp"


using namespace std::chrono;

class SourceFolder
{

public:

	std::string Source_string;
	double fps= 10;
	char Img_Path[256];
	int	LANE_DETECTOR = 1;
	int	LANE_ANALYZER = 1;
	int	SEND_DATA = 0;
	int	DATA_RECORD = 1;
	int	StartFrame = 1; // FRAME_START
	int Idx =StartFrame; //current_Index
	int EndFrame = 611; // FRAME_END
    double YAW_ANGLE = 0; // Yaw - X
    double PITCH_ANGLE = 0.1; // pitch - Y
};



enum class InputMode {Camera, Folder, Video};

// Node which captures images from a camera using OpenCV and publishes them.
// Images are annotated with this process's id as well as the message's ptr.
class CameraNode : public rclcpp::Node
{



public:

  CameraNode(const std::string & output, const std::string & node_name = "camera_node", int device = 0, int width = 320, int height = 240) : Node(node_name, true), canceled_(false)

{

// Create a publisher on the output topic.
  pub_ = this->create_publisher<sensor_msgs::msg::Image>(output, rmw_qos_profile_sensor_data);
  this->node_name = node_name;

if (input== InputMode::Camera)
{

    // Initialize OpenCV
    cap_.open(device);
    cap_.set(CV_CAP_PROP_FRAME_WIDTH, static_cast<double>(width));
    cap_.set(CV_CAP_PROP_FRAME_HEIGHT, static_cast<double>(height));

    if (!cap_.isOpened())
    {
      throw std::runtime_error("Could not open video stream!");
    }

      thread_ = std::thread(std::bind(&CameraNode::loop_CameraSrc, this));
}

else if (input== InputMode::Folder)
{


            src_folder.Source_string= "../inputdata/cropped/cropped_%d.png";

			std::cout << "/*************************************/" << std::endl;
			std::cout << "Input LANE_DETECTOR= " << src_folder.LANE_DETECTOR << std::endl;
			std::cout << "Input LANE_ANALYZER= " << src_folder.LANE_ANALYZER << std::endl;
			std::cout << "Input SEND_DATA = " << src_folder.SEND_DATA << std::endl;
			std::cout << "Input DATA_RECROD =" << src_folder.DATA_RECORD << std::endl;
			std::cout << "Input StartFrame =" << src_folder.StartFrame << std::endl;
			std::cout << "Input EndFrame =" << src_folder.EndFrame << std::endl;
			std::cout << "Input YAW_ANGLE =" << src_folder.YAW_ANGLE << std::endl;
			std::cout << "Input PITCH_ANGLE =" << src_folder.PITCH_ANGLE << std::endl;
			std::cout << "/*************************************/" << std::endl;
			thread_ = std::thread(std::bind(&CameraNode::loop_FolderSrc, this));

}


else if (input== InputMode::Video)
{


}

else
	throw std::runtime_error("The input mode for the program is not recognized....shutting down");

}






  virtual ~CameraNode()
  {
    // Make sure to join the thread on shutdown.
    canceled_.store(true);
    if (thread_.joinable()) {
      thread_.join();
    }
  }



  void loop_CameraSrc()
  {
    // While running...
    while (rclcpp::ok() && !canceled_.load()) {
      // Capture a frame from OpenCV.
      cap_ >> frame_;
      if (frame_.empty()) {
        continue;
      }
      // Create a new unique_ptr to an Image message for storage.
      sensor_msgs::msg::Image::UniquePtr msg(new sensor_msgs::msg::Image());

      // Pack the OpenCV image into the ROS image.
      set_now(msg->header.stamp);
      msg->header.frame_id = "camera_frame";
      msg->height = frame_.cols;
      msg->width = frame_.rows;
      msg->encoding = mat_type2encoding(frame_.type());
      msg->is_bigendian = false;
      msg->step = static_cast<sensor_msgs::msg::Image::_step_type>(frame_.step);
      msg->data.assign(frame_.datastart, frame_.dataend);
      pub_->publish(msg);  // Publish.

    }
  }


  void loop_FolderSrc()
    {

	  std::chrono::time_point<std::chrono::system_clock> start, end;

      // While running...
      while (rclcpp::ok() && !canceled_.load() && src_folder.Idx <= src_folder.EndFrame)
      {

    	auto start = high_resolution_clock::now();

    	//double startTime = (double)cv::getTickCount();
    	sprintf(src_folder.Img_Path, src_folder.Source_string.c_str() , src_folder.Idx);
    	frame_ = cv::imread(src_folder.Img_Path);
    	//frame_ = cv::imread("fish.jpg");
    	//cv::imshow(node_name, frame_);

        if (frame_.empty())
          continue;

        // Create a new unique_ptr to an Image message for storage.
        sensor_msgs::msg::Image::UniquePtr msg(new sensor_msgs::msg::Image());

        // Pack the OpenCV image into the ROS image.
        set_now(msg->header.stamp);
        msg->header.frame_id = src_folder.Img_Path;
        msg->height = frame_.cols;
        msg->width = frame_.rows;
        msg->encoding = mat_type2encoding(frame_.type());
        msg->is_bigendian = false;
        msg->step = static_cast<sensor_msgs::msg::Image::_step_type>(frame_.step);
        msg->data.assign(frame_.datastart, frame_.dataend);


        //wait for target duration
        auto end = high_resolution_clock::now();
        std::chrono::duration<double, std::milli> duration = end-start;


        while (duration.count() < (1.0/src_folder.fps)*1000)
        {

        	//std::cout<<duration.count() << std::endl;
        	end = high_resolution_clock::now();
        	duration = end- start;
        }


        pub_->publish(msg);  // Publish.
        src_folder.Idx++;

      }

    }


private:
  InputMode input = InputMode::Folder;

  std::chrono::high_resolution_clock Clock;
  std::string node_name;
  SourceFolder src_folder= SourceFolder();
  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr pub_;
  std::thread thread_;
  std::atomic<bool> canceled_;
  cv::VideoCapture cap_;
  cv::Mat frame_;
};

#endif  // IMAGE_PIPELINE__CAMERA_NODE_HPP_
