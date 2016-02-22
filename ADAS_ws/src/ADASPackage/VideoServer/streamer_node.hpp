// Copyright 2015 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef IMAGE_PIPELINE__IMAGE_VIEW_NODE_HPP_
#define IMAGE_PIPELINE__IMAGE_VIEW_NODE_HPP_
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <glib.h>
#include <gio/gio.h>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "../Aux/common.hpp"




class GImage
{

public:
	size_t sizeInBytes=0;
	guint height, width=0;
	guchar *data=NULL;
	guchar*  Gst_encoding=NULL;
};


static gboolean  bus_call (GstBus  *bus, GstMessage *msg, gpointer  data)
{
  //GMainLoop *loop = (GMainLoop *) data;

  switch (GST_MESSAGE_TYPE (msg)) {

    case GST_MESSAGE_EOS:
      g_print ("End of stream\n");
      break;

    case GST_MESSAGE_ERROR: {
      gchar  *debug;
      GError *error;

      gst_message_parse_error (msg, &error, &debug);
      g_free (debug);

      g_printerr ("Error: %s\n", error->message);
      g_error_free (error);
      break;
    }
    default:
      break;
  }

  return TRUE;
}


static void  cb_need_data (GstAppSrc *appsrc,guint usize, gpointer Gimage)
{
	   GstBuffer *buffer;
	   GstFlowReturn ret;
	   GImage* Gframe = (GImage *) Gimage;
	   static GstClockTime timestamp = 0;
	   //static gboolean white = FALSE;

	  //guint  size = 385 * 288 * 2;

	  //buffer = gst_buffer_new_allocate (NULL, Gframe->sizeInBytes, NULL);

	   //gst_buffer_map (buffer, &map, GST_MAP_WRITE);
	   //memcpy( (guchar *)map.data, Gframe->data,  gst_buffer_get_size( buffer ) );

	   buffer =gst_buffer_new_wrapped (Gframe->data,Gframe->sizeInBytes);
	   //buffer = gst_buffer_new_allocate (NULL, size, NULL);
	   //gst_buffer_memset (buffer, 0, white ? 0xff : 0x0, size);

	   //white = !white;

	   GST_BUFFER_PTS (buffer) = timestamp;
	   //GST_BUFFER_DURATION (buffer) = gst_util_uint64_scale_int (1, GST_SECOND, 1);
	   GST_BUFFER_DURATION (buffer) = 2000000;
	   //GST_BUFFER_DURATION (buffer)=GST_CLOCK_TIME_NONE;
	   timestamp += GST_BUFFER_DURATION (buffer);

	    g_signal_emit_by_name (appsrc, "push-buffer", buffer, &ret);

	    if (ret != GST_FLOW_OK)
	    {
	    	const char* str= "LiveCamera";
	    	g_print ("Fault in Pipeline flow: %s\n", str);
	    }
}

static void cb_enough_data(GstAppSrc *src, gpointer user_data)
{
    g_print("In %s\n", __func__);
}





// Node which receives sensor_msgs/Image messages and renders them using OpenCV.
class Streamer : public rclcpp::Node
{

private:
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_;


    cv::Mat frame_;
    //static GMainLoop *loop;
    bool VidSource_initialised=false;
    GImage* Gframe= new GImage();
    GstElement *pipeline, *source, *appsrc, *encoder, *decoder, *payload, *conv, *myudpsink, *videosink;
    GstBus *bus;




public: explicit Streamer( const std::string & input, const std::string & node_name = "streamer_node")
  	  	  	  	 : Node(node_name, true)
{

	auto qos = rmw_qos_profile_sensor_data;

	/*Initialise Gstreamer */
		   gst_init (NULL, NULL);

    // Create a subscription on the input topic.
    sub_ = this->create_subscription<sensor_msgs::msg::Image>(input, [node_name, this](const sensor_msgs::msg::Image::SharedPtr msg)

{
  //Create a cv::Mat from the image message (without copying).
   cv::Mat frame_(msg->width, msg->height,encoding2mat_type(msg->encoding), msg->data.data());
   this->frame_=frame_;
   Gframe->data=(guchar *)frame_.data;
   if (! VidSource_initialised)


   {
    //Get frame properties
	       Gframe->sizeInBytes = frame_.step[0] * frame_.rows;
	  	   Gframe->height = (guint)frame_.rows;
	  	   Gframe->width  = (guint)frame_.cols;
	  	   Gframe->data   = (guchar *)frame_.data;
	  	   Gframe->Gst_encoding = (guchar*)mat_type2GSTencoding(frame_.type());

	 //setup pipeline
	    pipeline  = gst_pipeline_new ("pipeline");
	    appsrc    = gst_element_factory_make ("appsrc", "source");
	    source    = gst_element_factory_make ("videotestsrc", "source");

	    encoder = gst_element_factory_make("avenc_mpeg4", "Mpeg4_encoder");
	    decoder = gst_element_factory_make("avdec_mpeg4", "Mpeg4_decoder");
	    //encoder = gst_element_factory_make("jpegenc", "encoder");
	    //decoder = gst_element_factory_make("jpegdec", "decoder");

	    //payload = gst_element_factory_make("rtpjpegpay", "payload");
	    payload = gst_element_factory_make("rtpmp4vpay", "payload");

	    conv      = gst_element_factory_make ("videoconvert", "conv");
	    videosink = gst_element_factory_make ("autovideosink", "videosink");
	    myudpsink   =  gst_element_factory_make ("udpsink", "myudpsink");

	    if (!pipeline || !appsrc || !encoder || !payload || !myudpsink)
	    {
	        g_printerr ("One element could not be created. Exiting.\n");
	        return -1;
	    }


	  /*Payload settings in case of MPEG4 */
	    g_object_set(G_OBJECT(payload),"config-interval", 60, NULL);



	  /*Setup appsrc*/
	    g_object_set (G_OBJECT (appsrc), "caps",
	    gst_caps_new_simple ("video/x-raw", "format", G_TYPE_STRING, Gframe->Gst_encoding, "width", G_TYPE_INT, Gframe->width, "height", G_TYPE_INT, Gframe->height, "framerate", GST_TYPE_FRACTION, 0, 1,NULL), NULL);
	    //gst_caps_new_simple ("video/x-raw", "format", G_TYPE_STRING, "RGB16", "width", G_TYPE_INT, 384, "height", G_TYPE_INT, 288, "pixel-aspect-ratio", GST_TYPE_FRACTION, 1, 1,"framerate", GST_TYPE_FRACTION, 0, 1,NULL), NULL);
	    g_object_set (G_OBJECT (appsrc),"stream-type", 0,"is-live", TRUE,"format", GST_FORMAT_TIME, NULL);
	    //g_object_set (G_OBJECT (appsrc),"stream-type", 0,"format", GST_FORMAT_TIME, NULL);


	    g_signal_connect (appsrc, "need-data", G_CALLBACK (cb_need_data), Gframe);
	    g_signal_connect (appsrc, "enough-data", G_CALLBACK (cb_enough_data), Gframe);


	    /*SetUp UDP-sink*/
	    	    g_object_set(G_OBJECT(myudpsink), "host", "131.155.222.12", NULL);
	    	    g_object_set(G_OBJECT(myudpsink), "port", 5000, NULL);

	   /*Connect the pipeline elements */
	    //gst_bin_add_many (GST_BIN (pipeline), appsrc, conv, videosink, NULL);
	    //gst_element_link_many (appsrc, conv, videosink, NULL);


	     gst_bin_add_many (GST_BIN (pipeline), appsrc, conv, encoder, payload, myudpsink, NULL);
	     gst_element_link_many (appsrc, conv, encoder, payload, myudpsink, NULL);



		/* play */
		         const char* str= "LiveCamera";
		         g_print ("Now playing: %s\n", str);
		         gst_element_set_state (pipeline, GST_STATE_PLAYING);


       VidSource_initialised = true;

        }


   	   	/* Handle bus messages */
		GstMessage *gstmsg;
  	    bus = gst_element_get_bus (pipeline);
  	    gstmsg= gst_bus_pop (bus);

  		while (gstmsg)
  		{
  			  /* Call your bus message handler */
  	      bus_call (bus, gstmsg, NULL);
  		  gst_message_unref (gstmsg);
  		  gstmsg= gst_bus_pop (bus);
  		}

   // Show the image.
   //CvMat c_mat = frame_;
   //cvShowImage(node_name.c_str(), &c_mat);


// char key = cv::waitKey(1);    // Look for key presses.
// if (key == 27 /* ESC */ || key == 'q')
//	   {
//		 rclcpp::shutdown();
//	   }

//   if (key == ' ')
//	   {    // If <space> then pause until another <space>.
//		 key = '\0';
//		 while (key != ' ') {
//		   key = cv::waitKey(1);
//	   }
//   }
 }, qos);
}



};





#endif  // IMAGE_PIPELINE__IMAGE_VIEW_NODE_HPP_
