// generated from rosidl_generator_cpp/resource/msg__traits.hpp.template
// generated code does not contain a copyright notice

#ifndef EXAMPLE_INTERFACES__MSG__LARGE_FIXED__TRAITS_HPP_
#define EXAMPLE_INTERFACES__MSG__LARGE_FIXED__TRAITS_HPP_

#include <stdint.h>

#include <array>
#include <string>
#include <vector>

namespace rosidl_generator_traits
{

#ifndef __ROSIDL_GENERATOR_CPP_TRAITS
#define __ROSIDL_GENERATOR_CPP_TRAITS

template<typename T>
struct has_fixed_size : std::false_type {};

#endif  // __ROSIDL_GENERATOR_CPP_TRAITS

#include "example_interfaces/msg/large_fixed__struct.hpp"


template<>
struct has_fixed_size<example_interfaces::msg::LargeFixed> :
std::integral_constant<bool, true> {};

}  // namespace rosidl_generator_traits

#endif  // EXAMPLE_INTERFACES__MSG__LARGE_FIXED__TRAITS_HPP_
