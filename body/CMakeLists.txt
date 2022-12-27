cmake_minimum_required(VERSION 3.21)
project(body LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

# From https://github.com/grpc/grpc/blob/master/examples/cpp/cmake/common.cmake
option(USE_SYSTEM_GRPC "Use system installed gRPC" OFF)
if(USE_SYSTEM_GRPC)
  # This branch assumes that gRPC and all its dependencies are already installed
  # on this system, so they can be located by find_package().

  # Find Protobuf installation
  # Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
  set(protobuf_MODULE_COMPATIBLE TRUE)
  find_package(Protobuf CONFIG REQUIRED)
  message(STATUS "Using protobuf ${Protobuf_VERSION}")

  set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
  set(_REFLECTION gRPC::grpc++_reflection)
  if(CMAKE_CROSSCOMPILING)
    find_program(_PROTOBUF_PROTOC protoc)
  else()
    set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
  endif()

  # Find gRPC installation
  # Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
  find_package(gRPC CONFIG REQUIRED)
  message(STATUS "Using gRPC ${gRPC_VERSION}")

  set(_GRPC_GRPCPP gRPC::grpc++)
  if(CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
  else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
  endif()
else()
  # Build gRPC using FetchContent
  message(STATUS "Using gRPC via add_subdirectory (FetchContent).")
  include(FetchContent)
  FetchContent_Declare(
    gRPC
    GIT_REPOSITORY https://github.com/grpc/grpc
    GIT_TAG        v1.43.0
  )
  set(FETCHCONTENT_QUIET OFF)
  FetchContent_MakeAvailable(gRPC)

  # Since FetchContent uses add_subdirectory under the hood, we can use
  # the grpc targets directly from this build.
  set(_PROTOBUF_LIBPROTOBUF libprotobuf)
  set(_REFLECTION grpc++_reflection)
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protoc>)
  set(_GRPC_GRPCPP grpc++)
  if(CMAKE_CROSSCOMPILING)
    find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
  else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:grpc_cpp_plugin>)
  endif()
endif()

find_package(qibuild)
find_package(Threads)

# Need to have BrainCore checked out in same directory as NaoBody
get_filename_component(tsr_proto "../../BrainCore/thesocialrobot/thesocialrobot.proto" ABSOLUTE)
get_filename_component(tsr_proto_path "${tsr_proto}" PATH)

# Generated sources
set(tsr_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/thesocialrobot.pb.cc")
set(tsr_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/thesocialrobot.pb.h")
set(tsr_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/thesocialrobot.grpc.pb.cc")
set(tsr_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/thesocialrobot.grpc.pb.h")
add_custom_command(
      OUTPUT "${tsr_proto_srcs}" "${tsr_proto_hdrs}" "${tsr_grpc_srcs}" "${tsr_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${tsr_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${tsr_proto}"
      DEPENDS "${tsr_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# tsr_grpc_proto
add_library(tsr_grpc_proto
  ${tsr_grpc_srcs}
  ${tsr_grpc_hdrs}
  ${tsr_proto_srcs}
  ${tsr_proto_hdrs})
target_link_libraries(tsr_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})

# Create a executable named body
# with the source file: main.cpp
qi_create_bin(body "main.cpp")
target_link_libraries(body
    tsr_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})

# Add a simple test:
qi_create_test(test_body "test.cpp")
