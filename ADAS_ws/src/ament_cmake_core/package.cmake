set(_AMENT_PACKAGE_NAME "adas_pipeline")
set(adas_pipeline_VERSION "0.0.0")
set(adas_pipeline_MAINTAINER "Rameez Ismail <r.ismail@tue.nl>, Andreas <AndeasEmail@tue.nl>")
set(adas_pipeline_BUILD_DEPENDS "libopencv-dev" "rclcpp" "rmw_implementation_cmake" "sensor_msgs" "std_msgs")
set(adas_pipeline_BUILDTOOL_DEPENDS "ament_cmake")
set(adas_pipeline_BUILD_EXPORT_DEPENDS )
set(adas_pipeline_BUILDTOOL_EXPORT_DEPENDS )
set(adas_pipeline_EXEC_DEPENDS "libopencv-dev" "rclcpp" "sensor_msgs")
set(adas_pipeline_TEST_DEPENDS "ament_cmake_nose" "ament_lint_auto" "ament_lint_common" "launch" "launch_testing")
set(adas_pipeline_DEPRECATED "")
set(adas_pipeline_EXPORT_TAGS)
list(APPEND adas_pipeline_EXPORT_TAGS "<build_type>ament_cmake</build_type>")