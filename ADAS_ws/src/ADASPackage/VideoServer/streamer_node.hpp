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
#include <X11/Xlib.h>


using namespace std;

typedef void(*StreamerCallback)();
typedef void(*StreamerDataCallback)(guint8* d, gssize s);


typedef struct {
	GMainLoop *loop;
	GstElement *pipeline, *source, *parse, *scale, *capsfilter, *conv, *mux, *sink, *encoder, *decoder, *payload;
	guint sourceid;
	GstBus *bus;
	GThread *m_loop_thread;
	guint bus_watch_id;

	gboolean stop;

	gboolean resize, have_data;
	guint in_framerate, in_width, in_height, out_width, out_height;
	gchar* format = "BGR";
	GstBuffer* buffer;
	gint rotation;

	GMutex mutex;
	GCond cond;

	FILE* outfile;

	StreamerCallback ready_callback, input_callback, eos_callback;
	StreamerDataCallback output_callback;

} gst_app_t;



static gst_app_t app;



gboolean streamer_init()
{
	GError* e;
	gboolean res;

	res = gst_init_check(NULL, NULL, &e);
	if (!res) {
		g_printerr("%s", e->message);
	}

	g_mutex_init(&app.mutex);
	return res;
}




static void main_loop_run(gpointer data)
{
	g_debug("Streamer thread started\n");
	g_main_loop_run(app.loop);
	g_debug("Streamer returned, stopping playback\n");
}


static void cb_enough_data(GstAppSrc *src, gpointer user_data)
{
    g_print("In %s\n", __func__);
}



static void cb_need_Data(GstElement *appsrc, guint unused_size, gpointer user_data)
{

	static GstClockTime timestamp = 0;
	GstBuffer *buffer = NULL;
	GstFlowReturn ret;
	gboolean resize;


	g_mutex_lock(&app.mutex);

	if (!app.have_data)
	 {
	    g_debug("Wait for data... ");
	    g_cond_wait(&app.cond, &app.mutex);
	    g_debug("Done.\n");
      }

	else
	{
	    g_debug("Have data\n");
	}

	buffer = gst_buffer_copy(app.buffer);
	app.have_data = FALSE;
	resize = app.resize;
	app.resize = FALSE;
	g_mutex_unlock(&app.mutex);

	if (!buffer)
		return;

	if (resize)
	{
	  std::cout<<app.format<<endl;
		g_warning("Streamer resizing to %dx%d", app.in_width, app.in_height);
		
		
		g_object_set(G_OBJECT(app.source), "caps",
		gst_caps_new_simple("video/x-raw",
			"format", G_TYPE_STRING, app.format,
			"width", G_TYPE_INT, app.in_width,
			"height", G_TYPE_INT, app.in_height,
			"framerate", GST_TYPE_FRACTION, app.in_framerate, 1,
			NULL), NULL);


	}

	if (app.stop)
	{
		gst_app_src_end_of_stream((GstAppSrc*)appsrc);
		return;
	}


	GST_BUFFER_PTS(buffer) = timestamp;
	//GST_BUFFER_DURATION(buffer) = gst_util_uint64_scale_int(1, GST_MSECOND, 10);
	GST_BUFFER_DURATION(buffer) = gst_util_uint64_scale_int(1, GST_SECOND, 30);
	//GST_BUFFER_DURATION(buffer) = 200000;
	timestamp += GST_BUFFER_DURATION(buffer);

	ret = gst_app_src_push_buffer((GstAppSrc*)appsrc, buffer);

	if (ret != GST_FLOW_OK) {
		g_main_loop_quit(app.loop);
	}
	g_debug("written\n");
}

/* An older version
static void  cb_need_data (GstAppSrc *appsrc,guint usize, gpointer Gimage)
{
	       GstBuffer *buffer;
		   GstFlowReturn ret;
		   GImage* Gframe = (GImage *) Gimage;
		   static GstClockTime timestamp = 0;
		   //static gboolean white = FALSE;

		  //guint  size = 385 * 288 * 2;


		   //cv::Mat image(320, 240, CV_8UC3, cv::Scalar(0,0,0));
		   //std::vector<uchar>
		   //buffer = gst_buffer_new_allocate (NULL, Gframe->sizeInBytes, NULL);

		   //gst_buffer_map (buffer, &map, GST_MAP_WRITE);
		   //memcpy( (guchar *)map.data, Gframe->data,  gst_buffer_get_size( buffer ) );


	    //bool encoding = cv::imencode(".jpg", image, buffer, CV_IMWRITE_JPEG_QUALITY,100);


		   buffer =gst_buffer_new_wrapped (Gframe->data,Gframe->sizeInBytes);
		   //buffer = gst_buffer_new_allocate (NULL, size, NULL);
		   //gst_buffer_memset (buffer, 0, white ? 0xff : 0x0, size);

		   //white = !white;

		   GST_BUFFER_PTS (buffer) = timestamp;
		   //GST_BUFFER_DURATION (buffer) = gst_util_uint64_scale_int (1, GST_SECOND, 1);
		   //GST_BUFFER_DURATION (buffer) = 2000000;
		   //GST_BUFFER_DURATION (buffer) = 900000000;

		   GST_BUFFER_DURATION(buffer) = gst_util_uint64_scale_int(1, GST_MSECOND, 1);


		   //GST_BUFFER_DURATION (buffer)=GST_CLOCK_TIME_NONE;
		   timestamp += GST_BUFFER_DURATION (buffer);

		    g_signal_emit_by_name (appsrc, "push-buffer", buffer, &ret);

		    if (ret != GST_FLOW_OK)
		    {
		    	const char* str= "LiveCamera";
		    	g_print ("Fault in Pipeline flow: %s\n", str);
		    }
}
*/

void streamer_feed_sync(guint w, guint h, gssize size, gchar* format, guint8* frame)
{
 
  

	if (w != app.in_width || h != app.in_height || app.format != format)
	{
		app.resize = TRUE;
		app.in_width = w;
		app.in_height = h;
		app.format = format;
		
	}

	if (app.resize)
	{
		if (app.buffer)
			gst_buffer_unref(app.buffer);
		app.buffer = gst_buffer_new_and_alloc(size);
	}

	gst_buffer_fill(app.buffer, 0, frame, size);
}

void streamer_feed(guint w, guint h, gssize size, gchar * format, guint8* frame) {



	g_mutex_lock(&app.mutex);

	streamer_feed_sync(w, h, size, format, frame);
	app.have_data = TRUE;

	g_mutex_unlock(&app.mutex);
	g_cond_signal(&app.cond);
}



static gboolean bus_call(GstBus     *bus, GstMessage *msg,  gpointer    data)
{
	GMainLoop *loop = (GMainLoop *)data;
	GstState old_state, new_state;

	switch (GST_MESSAGE_TYPE(msg)) {
	case GST_MESSAGE_STATE_CHANGED:
		gst_message_parse_state_changed (msg, &old_state, &new_state, NULL);

		if(msg->src == GST_OBJECT(app.pipeline) && new_state == GST_STATE_READY && old_state == GST_STATE_NULL) {
			g_debug ("Element %s changed state from %s to %s.\n",
				GST_OBJECT_NAME (msg->src),
				gst_element_state_get_name (old_state),
				gst_element_state_get_name (new_state));
			if(app.ready_callback) {
				app.ready_callback();
			}
		}
		break;
	case GST_MESSAGE_EOS:
		g_warning("End of stream\n");
		g_main_loop_quit(loop);
		break;

	case GST_MESSAGE_ERROR: {
		gchar  *debug;
		GError *error;

		gst_message_parse_error(msg, &error, &debug);
		// g_free(debug);

		g_printerr("Error: %s\n", error->message);
		g_printerr("Debug: %s\n", debug);
		g_error_free(error);

		g_main_loop_quit(loop);
		break;
	}
	default:
		break;
	}

	return TRUE;
}



gboolean streamer_run(guint in_framerate, guint out_width, guint out_height) {


	g_cond_init(&app.cond);

	app.loop = g_main_loop_new(NULL, FALSE);
	app.in_width = 0;
	app.in_height = 0;
	app.out_width = out_width;
	app.out_height = out_height;
	app.in_framerate = in_framerate;
	app.resize = FALSE;
	app.have_data = FALSE;

	app.pipeline = gst_pipeline_new("video-streamer");
	app.source = gst_element_factory_make("appsrc", "app-src");
	//app.source = gst_element_factory_make("videotestsrc", "app-src");
	app.scale = gst_element_factory_make("videoscale", "video-scale");
	app.capsfilter = gst_element_factory_make("capsfilter", "caps-filter");
	app.conv      = gst_element_factory_make ("videoconvert", "conv");


	app.encoder = gst_element_factory_make("avenc_mpeg4", "Mpeg4-encoder");
	app.payload = gst_element_factory_make("rtpmp4vpay", "payload");


	//app.encoder = gst_element_factory_make("jpegenc", "jpeg-encoder");
	//app.payload = gst_element_factory_make("rtpjpegpay", "payload");

	app.sink   =  gst_element_factory_make ("udpsink", "udp-sink");
	//app.sink   =  gst_element_factory_make ("autovideosink", "video-sink");


	g_assert(app.pipeline);
	g_assert(app.source);
	g_assert(app.scale);
	g_assert(app.capsfilter);
	g_assert(app.conv);

	app.bus = gst_pipeline_get_bus(GST_PIPELINE(app.pipeline));
	app.bus_watch_id = gst_bus_add_watch(app.bus, bus_call, app.loop);
	gst_object_unref(app.bus);





	 g_object_set(G_OBJECT(app.sink), "host", "131.155.222.8", NULL);
	 g_object_set(G_OBJECT(app.sink), "port", 5000, NULL);





	 /*Payload settings in case of MPEG4 */
	g_object_set(G_OBJECT(app.payload),"config-interval", 60, NULL);


	/* Encoder settings in case of MPEG4 */
	 g_object_set(G_OBJECT(app.encoder), "bitrate", 4000000, NULL);


	g_object_set(G_OBJECT(app.source), "caps",
		gst_caps_new_simple	("video/x-raw",
							 "format", G_TYPE_STRING, app.format,
							 "width", G_TYPE_INT, out_width,
							 "height", G_TYPE_INT, out_height,
							 "framerate", GST_TYPE_FRACTION, in_framerate, 1, NULL), NULL);


	//g_object_set(G_OBJECT(app.source), "stream-type", 0, "format", GST_FORMAT_TIME, NULL);
	 g_object_set (G_OBJECT (app.source),"stream-type", 0,"is-live", TRUE,"format", GST_FORMAT_TIME, NULL);

	/*g_object_set(G_OBJECT(app.source), "caps",
	gst_caps_new_simple	("video/x-raw",
						 "format", G_TYPE_STRING, "BGR",
						 "width", G_TYPE_INT, out_width,
						 "height", G_TYPE_INT, out_height,
						 "framerate", GST_TYPE_FRACTION, in_framerate, 1, NULL), NULL);*/




	 g_signal_connect(app.source, "need-data", G_CALLBACK(cb_need_Data), NULL);
	 g_object_set(G_OBJECT(app.sink), "port", 5000, NULL);


	gst_bin_add_many(GST_BIN(app.pipeline), app.source, app.conv, app.encoder, app.payload,app.sink, NULL);



	/*g_object_set(G_OBJECT(app.capsfilter), "caps",
		gst_caps_new_simple("video/x-raw",
		"format", G_TYPE_STRING, "RGB",
		"width", G_TYPE_INT, out_width,
		"height", G_TYPE_INT, out_height,
		"framerate", GST_TYPE_FRACTION, in_framerate, 1,
		NULL), NULL);
*/


	bool ret= gst_element_link_many(app.source, app.conv, app.encoder, app.payload,app.sink, NULL);

	if(!ret)
	{
		g_printerr("ERROR: linking failed\n");
		return FALSE;
	}

	gst_element_set_state(app.pipeline, GST_STATE_PLAYING);
	g_debug("Running...\n");

	if ((app.m_loop_thread = g_thread_new("mainloop", (GThreadFunc)main_loop_run, NULL)) == NULL){
		g_printerr("ERROR: cannot start loop thread\n");
		return FALSE;
	}

	return TRUE;
}




void streamer_stop() {

	g_mutex_lock(&app.mutex);
	app.have_data = FALSE;
	g_mutex_unlock(&app.mutex);
	g_cond_signal(&app.cond);

	gst_element_set_state(app.pipeline, GST_STATE_NULL);

	g_debug("Deleting pipeline\n");
	gst_object_unref(GST_OBJECT(app.pipeline));
	g_source_remove(app.bus_watch_id);
	g_main_loop_unref(app.loop);
}



class GImage
{

public:
	size_t sizeInBytes=0;
	guint height, width=0;
	guchar *data=NULL;
	guchar*  Gst_encoding=NULL;
};





// Node which receives sensor_msgs/Image messages and renders them using OpenCV.
class Streamer : public rclcpp::Node
{

private:
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_;


    cv::Mat frame_;
    //static GMainLoop *loop;
    bool VidSource_initialised=false;
    GImage* Gframe= new GImage();
    GstElement *pipeline, *source, *appsrc, *encoder, *decoder, *payload, *conv, *myudpsink, *videosink, *queue1, *queue2;
    GstBus *bus;




public: explicit Streamer( const std::string & input, const std::string & node_name = "streamer_node")
  	  	  	  	 : Node(node_name, true)
{

  
  
	int w=1024, h=600, fps = 0;

	auto qos = rmw_qos_profile_sensor_data;



	g_log_set_handler(G_LOG_DOMAIN, G_LOG_LEVEL_DEBUG, g_log_default_handler, NULL);
	XInitThreads();
	streamer_init();
	
	
    if (streamer_run(fps, w / 2, h/ 2))
 {
	/*Initialise Gstreamer */

	//rameez	  gst_init (NULL, NULL);

		    // Create a subscription on the input topic.
		    sub_ = this->create_subscription<sensor_msgs::msg::Image>(input, [node_name, this, app](const sensor_msgs::msg::Image::SharedPtr msg)

		{
		  
		  
		  

		  
		  
		  //Create a cv::Mat from the image message (without copying).
		   cv::Mat frame_(msg->width, msg->height,encoding2mat_type(msg->encoding), msg->data.data());
		   this->frame_= frame_;
		    
		   
		   std::string encoding = mat_type2GSTencoding(frame_.type());


		    gchar * format = new char[encoding.size() + 1];
		  		  std::copy(encoding.begin(), encoding.end(), format);
		  		  format[encoding.size()] = '\0'; // don't forget the terminating 0

		  //cv::imshow("Lane System", frame_);
		  // cv::waitKey(1);
		   
		   
		cout<<format<<std::endl;
									      
									      
									      
		  streamer_feed(frame_.cols, frame_.rows,frame_.step[0] * frame_.rows, "BGR",   frame_.data);
		  
		  delete[] format;
		  
		 }, qos);

 }
 
   // g_usleep(1000*1000*10);
   // streamer_stop();
}

};


#endif  // IMAGE_PIPELINE__IMAGE_VIEW_NODE_HPP_
