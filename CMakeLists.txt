cmake_minimum_required(VERSION 3.10)
project(NetworkProtocolAnalyzer C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Protocol library (C)
add_library(protocol STATIC simulator/protocol.c)
target_include_directories(protocol PUBLIC simulator)

# Simulator executable (C)
add_executable(device_sim simulator/device.c)
target_link_libraries(device_sim PRIVATE protocol)

# Dissector library (C++)
add_library(dissector_core dissector/dissector_core.cpp)
target_include_directories(dissector_core PUBLIC simulator dissector)

# Packet analyzer tool (C++)
add_executable(packet_analyzer dissector/packet_analyzer.cpp)
target_link_libraries(packet_analyzer PRIVATE dissector_core)

# Tests
add_executable(test_protocol tests/test_protocol.cpp)
target_include_directories(test_protocol PRIVATE simulator)
target_link_libraries(test_protocol PRIVATE protocol)

add_executable(test_dissector tests/test_dissector.cpp)
target_include_directories(test_dissector PRIVATE simulator dissector)
target_link_libraries(test_dissector PRIVATE dissector_core protocol)

# Enable testing
enable_testing()
add_test(NAME test_protocol COMMAND test_protocol)
add_test(NAME test_dissector COMMAND test_dissector)

# Install
install(TARGETS device_sim packet_analyzer RUNTIME DESTINATION bin)