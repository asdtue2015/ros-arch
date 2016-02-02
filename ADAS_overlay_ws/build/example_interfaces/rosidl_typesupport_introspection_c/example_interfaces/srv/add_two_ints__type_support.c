// generated from rosidl_typesupport_introspection_c/resource/srv__type_support.c.template

#ifndef EXAMPLE_INTERFACES__SRV__ADD_TWO_INTS__TYPE_SUPPORT_H_
#define EXAMPLE_INTERFACES__SRV__ADD_TWO_INTS__TYPE_SUPPORT_H_

#include <rosidl_generator_c/service_type_support.h>

#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

#include "example_interfaces/srv/add_two_ints__request__struct.h"
#include "example_interfaces/srv/add_two_ints__response__struct.h"
#include "example_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers example_interfaces__srv__rosidl_typesupport_introspection_c__AddTwoInts_service_members = {
  "example_interfaces",  // package name
  "AddTwoInts",  // service name
  // these two fields are initialized below on the first access
  // see get_service_type_support_handle_introspection<example_interfaces::AddTwoInts>()
  NULL,  // request message
  // example_interfaces__srv__rosidl_typesupport_introspection_c__AddTwoInts_Request_message_type_support_handle,
  NULL  // response message
  // example_interfaces__srv__rosidl_typesupport_introspection_c__AddTwoInts_Request_message_type_support_handle
};

static const rosidl_service_type_support_t example_interfaces__srv__rosidl_typesupport_introspection_c__AddTwoInts_service_type_support_handle = {
  rosidl_typesupport_introspection_c__identifier,
  &example_interfaces__srv__rosidl_typesupport_introspection_c__AddTwoInts_service_members
};

const rosidl_service_type_support_t *
rosidl_get_service_type_support__example_interfaces__srv____AddTwoInts()
{
  return &example_interfaces__srv__rosidl_typesupport_introspection_c__AddTwoInts_service_type_support_handle;
}

#endif  // EXAMPLE_INTERFACES__SRV__ADD_TWO_INTS__TYPE_SUPPORT_H_
