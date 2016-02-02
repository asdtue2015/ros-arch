// generated from rosidl_typesupport_introspection_c/resource/msg__type_support.c.template


#include "example_interfaces/srv/add_two_ints__request__introspection_type_support.h"

// providing offsetof()
#include <stddef.h>

#include <rosidl_generator_c/message_type_support.h>

#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"

#include "example_interfaces/srv/add_two_ints__request__struct.h"

static const rosidl_typesupport_introspection_c__MessageMember example_interfaces__srv__rosidl_typesupport_introspection_c__AddTwoInts_Request_message_member_array[2] = {
  {
    "a",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT64,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(example_interfaces__srv__AddTwoInts_Request, a),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "b",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT64,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(example_interfaces__srv__AddTwoInts_Request, b),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers example_interfaces__srv__rosidl_typesupport_introspection_c__AddTwoInts_Request_message_members = {
  "example_interfaces",  // package name
  "AddTwoInts_Request",  // message name
  2,  // number of fields
  example_interfaces__srv__rosidl_typesupport_introspection_c__AddTwoInts_Request_message_member_array  // message members
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t example_interfaces__srv__rosidl_typesupport_introspection_c__AddTwoInts_Request_message_type_support_handle = {
  0,
  &example_interfaces__srv__rosidl_typesupport_introspection_c__AddTwoInts_Request_message_members
};

const rosidl_message_type_support_t *
rosidl_get_message_type_support__example_interfaces__msg__AddTwoInts_Request()
{
  if (!example_interfaces__srv__rosidl_typesupport_introspection_c__AddTwoInts_Request_message_type_support_handle.typesupport_identifier) {
    example_interfaces__srv__rosidl_typesupport_introspection_c__AddTwoInts_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &example_interfaces__srv__rosidl_typesupport_introspection_c__AddTwoInts_Request_message_type_support_handle;
}
