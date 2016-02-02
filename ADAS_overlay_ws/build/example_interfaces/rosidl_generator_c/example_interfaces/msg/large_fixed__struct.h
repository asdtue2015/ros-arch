// generated from rosidl_generator_c/resource/msg__struct.h.template
// generated code does not contain a copyright notice

#ifndef EXAMPLE_INTERFACES__MSG__LARGE_FIXED__STRUCT_H_
#define EXAMPLE_INTERFACES__MSG__LARGE_FIXED__STRUCT_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/// Struct of message example_interfaces/LargeFixed
typedef struct example_interfaces__msg__LargeFixed
{
  int32_t data[255];
} example_interfaces__msg__LargeFixed;

/// Struct for an array of messages
typedef struct example_interfaces__msg__LargeFixed__Array
{
  example_interfaces__msg__LargeFixed * data;
  size_t size;
  size_t capacity;
} example_interfaces__msg__LargeFixed__Array;

#endif  // EXAMPLE_INTERFACES__MSG__LARGE_FIXED__STRUCT_H_
