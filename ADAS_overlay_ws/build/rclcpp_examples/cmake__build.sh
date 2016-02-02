#!/usr/bin/env sh

if [ -n "$AMENT_TRACE_SETUP_FILES" ]; then
  echo ". \"/home/rameez/ADAS_overlay_ws/install/share/example_interfaces/local_setup.sh\""
fi
if [ -f "/home/rameez/ADAS_overlay_ws/install/share/example_interfaces/local_setup.sh" ]; then
  . "/home/rameez/ADAS_overlay_ws/install/share/example_interfaces/local_setup.sh"
fi

export CMAKE_PREFIX_PATH="$AMENT_PREFIX_PATH:$CMAKE_PREFIX_PATH"
