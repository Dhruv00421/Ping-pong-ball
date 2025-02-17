cmake_minimum_required(VERSION 3.10)
project(Pingpongball)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Raylib (installed via vcpkg)
#find_package(raylib REQUIRED)

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# Add project source files
add_executable(Pingpongball 
    src/main.cpp 
    src/Ball.cpp    src/Ball.h
    src/Paddle.cpp  src/Paddle.h
    src/Globals.cpp src/Globals.h
)

# Include directories for dependencies
include_directories(${CMAKE_SOURCE_DIR}/Vendors/Imgui)
include_directories(${CMAKE_SOURCE_DIR}/Vendors/rlIm)
include_directories(${CMAKE_SOURCE_DIR}/Vendors/Raylib/include)

# Add ImGui source files
set(IMGUI_FILES
    Vendors/Imgui/imgui.cpp
    Vendors/Imgui/imgui_draw.cpp
    Vendors/Imgui/imgui_tables.cpp
    Vendors/Imgui/imgui_widgets.cpp
    Vendors/Imgui/backends/imgui_impl_glfw.cpp
    Vendors/Imgui/backends/imgui_impl_opengl3.cpp
)

# Add rlIm source files
set(RLIM_FILES
    Vendors/Rlim/rlImGui.cpp
    Vendors/Rlim/imgui_impl_raylib.cpp
)

#set(IMGUI_FILES
#    Vendors/.cpp
#    Vendors/.cpp
#    Vendors/.cpp
#)

# Check if source files exist
foreach(file IN LISTS IMGUI_FILES RLIM_FILES)
    if(NOT EXISTS ${CMAKE_SOURCE_DIR}/${file})
        message(FATAL_ERROR "File not found: ${CMAKE_SOURCE_DIR}/${file}")
    endif()
endforeach()


# Add ImGui and rlIm as static libraries
add_library(imgui STATIC ${IMGUI_FILES})
add_library(rlIm STATIC ${RLIM_FILES})

# Link libraries
target_link_libraries(Pingpongball PRIVATE imgui rlIm)
target_link_libraries(Pingpongball PRIVATE ${CMAKE_SOURCE_DIR}/Vendors/Raylib/lib/raylib.lib)
#target_link_libraries(Pingpongball PRIVATE winmm OpenAL32)
target_link_libraries(Pingpongball PRIVATE ${CMAKE_SOURCE_DIR}/Vendors/lib/OpenAL32.lib)



# Ensure sounds are copied to the build directory
add_custom_target(CopySounds ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Sounds ${CMAKE_BINARY_DIR}/Sounds
)

# Enable high warning levels for better debugging
if (MSVC)
    target_compile_options(Pingpongball PRIVATE /W4)
else()
    target_compile_options(Pingpongball PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Link against the Windows multimedia library
if (WIN32)
    target_link_libraries(Pingpongball PRIVATE winmm)
endif()