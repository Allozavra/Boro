#ifndef BORO_INTERFACES__VISIBILITY_CONTROL_H_
#define BORO_INTERFACES__VISIBILITY_CONTROL_H_
#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define BORO_INTERFACES_EXPORT __attribute__ ((dllexport))
    #define BORO_INTERFACES_IMPORT __attribute__ ((dllimport))
  #else
    #define BORO_INTERFACES_EXPORT __declspec(dllexport)
    #define BORO_INTERFACES_IMPORT __declspec(dllimport)
  #endif
  #ifdef BORO_INTERFACES_BUILDING_LIBRARY
    #define BORO_INTERFACES_PUBLIC BORO_INTERFACES_EXPORT
  #else
    #define BORO_INTERFACES_PUBLIC BORO_INTERFACES_IMPORT
  #endif
  #define BORO_INTERFACES_PUBLIC_TYPE BORO_INTERFACES_PUBLIC
  #define BORO_INTERFACES_LOCAL
#else
  #define BORO_INTERFACES_EXPORT __attribute__ ((visibility("default")))
  #define BORO_INTERFACES_IMPORT
  #if __GNUC__ >= 4
    #define BORO_INTERFACES_PUBLIC __attribute__ ((visibility("default")))
    #define BORO_INTERFACES_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define BORO_INTERFACES_PUBLIC
    #define BORO_INTERFACES_LOCAL
  #endif
  #define BORO_INTERFACES_PUBLIC_TYPE
#endif
#endif  // BORO_INTERFACES__VISIBILITY_CONTROL_H_
// Generated 15-Apr-2026 17:34:18
 