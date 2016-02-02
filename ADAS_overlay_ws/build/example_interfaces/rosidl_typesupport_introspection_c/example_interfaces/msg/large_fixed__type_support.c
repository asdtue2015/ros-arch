// generated from rosidl_typesupport_introspection_c/resource/msg__type_support.c.template


#include "example_interfaces/msg/large_fixed__introspection_type_support.h"

// providing offsetof()
#include <stddef.h>

#include <rosidl_generator_c/message_type_support.h>

#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"

#include "example_interfaces/msg/large_fixed__struct.h"

static const rosidl_typesupport_introspection_c__MessageMember example_interfaces__msg__rosidl_typesupport_introspection_c__LargeFixed_message_member_array[1] = {
  {
    "data",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    255,  // array size
    false,  // is upper bound
    offsetof(example_interfaces__msg__LargeFixed, data),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers example_interfaces__msg__rosidl_typesupport_introspection_c__LargeFixed_message_members = {
  "example_interfaces",  // package name
  "LargeFixed",  // message name
  1,  // number of fields
  example_interfaces__msg__rosidl_typesupport_introspection_c__LargeFixed_message_member_array  // message members
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t example_interfaces__msg__rosidl_typesupport_introspection_c__LargeFixed_message_type_support_handle = {
  0,
  &example_interfaces__msg__rosidl_typesupport_introspection_c__LargeFixed_message_members
};

const rosidl_message_type_support_t *
rosidl_get_message_type_support__example_interfaces__msg__LargeFixed()
{
  if (!example_interfaces__msg__rosidl_typesupport_introspection_c__LargeFixed_message_type_support_handle.typesupport_identifier) {
    example_interfaces__msg__rosidl_typesupport_introspection_c__LargeFixed_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &example_interfaces__msg__rosidl_typesupport_introspection_c__LargeFixed_message_type_support_handle;
}
