// generated from rosidl_generator_c/resource/rosidl_generator_c__visibility_control.h.in

#ifndef example_interfaces__MSG__ROSIDL_GENERATOR_C__VISIBILITY_CONTROL__H_
#define example_interfaces__MSG__ROSIDL_GENERATOR_C__VISIBILITY_CONTROL__H_

#if __cplusplus
extern "C"
{
#endif

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define ROSIDL_GENERATOR_C_EXPORT_example_interfaces \
      __attribute__ ((dllexport))
    #define ROSIDL_GENERATOR_C_IMPORT_example_interfaces \
      __attribute__ ((dllimport))
  #else
    #define ROSIDL_GENERATOR_C_EXPORT_example_interfaces \
      __declspec(dllexport)
    #define ROSIDL_GENERATOR_C_IMPORT_example_interfaces \
      __declspec(dllimport)
  #endif
  #ifdef ROSIDL_GENERATOR_C_BUILDING_DLL_example_interfaces
    #define ROSIDL_GENERATOR_C_PUBLIC_example_interfaces \
      ROSIDL_GENERATOR_C_EXPORT_example_interfaces
  #else
    #define ROSIDL_GENERATOR_C_PUBLIC_example_interfaces \
      ROSIDL_GENERATOR_C_IMPORT_example_interfaces
  #endif
#else
  #define ROSIDL_GENERATOR_C_EXPORT_example_interfaces \
    __attribute__ ((visibility("default")))
  #define ROSIDL_GENERATOR_C_IMPORT_example_interfaces
  #if __GNUC__ >= 4
    #define ROSIDL_GENERATOR_C_PUBLIC_example_interfaces \
      __attribute__ ((visibility("default")))
  #else
    #define ROSIDL_GENERATOR_C_PUBLIC_example_interfaces
  #endif
#endif

#if __cplusplus
}
#endif

#endif  // example_interfaces__MSG__ROSIDL_GENERATOR_C__VISIBILITY_CONTROL__H_
