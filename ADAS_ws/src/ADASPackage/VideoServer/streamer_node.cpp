
#include "streamer_node.hpp"
#include <rclcpp/rclcpp.hpp>


int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

 /* Initialize GStreamer */

  const gchar *nano_str;
  guint major, minor, micro, nano;
  gst_init (&argc, &argv);
  gst_version (&major, &minor, &micro, &nano);
  if (nano == 1)
  nano_str = "(CVS)";
  else if (nano == 2)
  nano_str = "(Prerelease)";
  else
  nano_str = "";
  printf ("This program is linked against GStreamer %d.%d.%d %s\n",
  major, minor, micro, nano_str);

  auto streamer_node = std::make_shared<Streamer>("lanedetect_image");
  rclcpp::spin(streamer_node);
  return 0;
}
