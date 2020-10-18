cmake_minimum_required(VERSION 3.5)

message(STATUS "Configuring Load Test")

# Load test client
add_executable(LoadTestClient
    Private/LoadTestClientMain.cpp
)

target_include_directories(LoadTestClient
    PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/Private
)

target_link_libraries(LoadTestClient
    PRIVATE
        -static-libgcc
        -static-libstdc++
        ${SDL2_LIBRARIES}
        SDL2_net-static
        Shared
)

# Compile with C++17
target_compile_features(LoadTestClient PRIVATE cxx_std_17)
set_target_properties(LoadTestClient PROPERTIES CXX_EXTENSIONS OFF)

# Enable compile warnings.
target_compile_options(LoadTestClient PUBLIC -Wall -Wextra)