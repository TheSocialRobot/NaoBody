cmake_minimum_required(VERSION 3.21)
project(body LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(qibuild)
find_package(Protobuf REQUIRED)
find_package(GRPC REQUIRED)
find_package(Threads)

# Need to have BrainCore checked out in same directory as NaoBody
file(GLOB PROTO_FILES "../../BrainCore/thesocialrobot/*.proto")
add_library(coreproto ${PROTO_FILES})
target_link_libraries(coreproto
        PUBLIC
        protobuf::libprotobuf
        gRPC::grpc
        gRPC::grpc++
        )
target_include_directories(coreproto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)
protobuf_generate(TARGET coreproto LANGUAGE cpp)
protobuf_generate(TARGET coreproto LANGUAGE grpc GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}")


# Create a executable named body
# with the source file: main.cpp
qi_create_bin(body "main.cpp")
target_link_libraries(body PRIVATE coreproto ${CONAN_LIBS})
target_include_directories(body PRIVATE ${CMAKE_BINARY_DIR})

# Add a simple test:
qi_create_test(test_body "test.cpp")
