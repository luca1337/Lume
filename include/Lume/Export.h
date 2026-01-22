#pragma once

#if defined(_WIN32) || defined(__CYGWIN__)
  #ifdef RENDER_CORE_SHARED
    #ifdef RENDER_CORE_BUILD_DLL
      #define RENDER_API __declspec(dllexport)
    #else
      #define RENDER_API __declspec(dllimport)
    #endif
  #else
    #define RENDER_API
  #endif
#else
  #ifdef RENDER_CORE_SHARED
    #define RENDER_API __attribute__((visibility("default")))
  #else
    #define RENDER_API
  #endif
#endif
