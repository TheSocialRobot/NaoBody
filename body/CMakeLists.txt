cmake_minimum_required(VERSION 2.8)
project(body)

find_package(qibuild)

# Create a executable named body
# with the source file: main.cpp
qi_create_bin(body "main.cpp")

# Add a simple test:
qi_create_test(test_body "test.cpp")
