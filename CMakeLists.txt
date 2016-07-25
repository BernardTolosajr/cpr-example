cmake_minimum_required(VERSION 2.8.7)
project(cpr-example)

set(EXAMPLE_LIBRARIES example CACHE INTERNAL "")
set(EXAMPLE_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE INTERNAL "")

if(IOS)
  message(STATUS "Using libcurl for iOS")

  set(IOS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build_ios")
  set(CURL_INCLUDE_DIRS "${IOS_SOURCE_DIR}/curl-ios/include")
  set(CURL_LIBRARIES "${IOS_SOURCE_DIR}/curl-ios/lib/libcurl.a")
  set(USE_SYSTEM_CURL OFF CACHE BOOL "Use the system curl for faster builds")

  set(CPR_INCLUDE_DIRS "${IOS_SOURCE_DIR}/cpr/include")
  set(CPR_LIBRARIES "${IOS_SOURCE_DIR}/cpr/lib/libcpr.a")


  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Binaries)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -Wno-return-type-c-linkage -Wno-unneeded-internal-declaration")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
  set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")

   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-strict-aliasing")

   set(ZLIB_INCLUDE_DIRS "${CMAKE_IOS_SDK_ROOT}/usr/include")
   set(ZLIB_LIBRARIES "${CMAKE_IOS_SDK_ROOT}/usr/lib/libz.tbd")

   message(STATUS "ZLIB_LIBRARIES ${ZLIB_INCLUDE_DIRS}")
   message(STATUS "ZLIB_INCLUDE_DIRS ${ZLIB_LIBRARIES}")

   find_library(SECURITY Security "/")
   find_library(COREFOUNDATION CoreFoundation "/")
   message(STATUS "found xcode sucurity framework ${SECURITY}")
   set(EXTRALINKS ${COREFOUNDATION} ${SECURITY})
endif()

if(NOT ${CMAKE_VERSION} LESS 3.2)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
else()
    message(STATUS "Checking compiler flags for C++11 support.")
    # Set C++11 support flags for various compilers
    include(CheckCXXCompilerFlag)
    check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
    check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
    if(COMPILER_SUPPORTS_CXX11)
        message(STATUS "C++11 is supported.")
        if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
        endif()
    elseif(COMPILER_SUPPORTS_CXX0X)
        message(STATUS "C++0x is supported.")
        if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++")
        else()
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
        endif()
    else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    endif()
endif()

add_subdirectory(opt)

message(STATUS "Using CPP_INCLUDE_DIRS: ${CPR_INCLUDE_DIRS}.")
message(STATUS "Using CPP_LIBRARIES: ${CPR_LIBRARIES}.")
message(STATUS "Using EXAMPLE DIR: ${EXAMPLE_INCLUDE_DIRS}.")
message(STATUS "Using EXAMPLE LIBRARY: ${EXAMPLE_LIBRARIES}.")

include_directories(${EXAMPLE_INCLUDE_DIRS}
  ${ZLIB_INCLUDE_DIRS}
  ${CPR_INCLUDE_DIRS}
  ${JSON_INCLUDE_DIRS})

add_library(${EXAMPLE_LIBRARIES} example.cpp)

target_link_libraries(${EXAMPLE_LIBRARIES} ${CPR_LIBRARIES})
target_link_libraries(${EXAMPLE_LIBRARIES}
  ${CPR_LIBRARIES}
  ${ZLIB_LIBRARIES}
  ${EXTRALINKS})
