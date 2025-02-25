# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Project name
project(KBDTrainer VERSION 1.0)

# C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(HEADER_FILES
    include/input.h
)

add_definitions(-D_AMD64_)

# Manually specify source files
set(SOURCE_FILES
    src/main.c
    src/input.c
)

# Include directories
include_directories(include)

# Find packages (uncomment as needed)
# find_package(SDL2 REQUIRED)
# find_package(OpenGL REQUIRED)

# Add XInput library for Windows
if(WIN32)
    link_libraries(xinput)
endif()

# Create executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

# Link libraries (uncomment as needed)
# target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2)
# target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})

# Installation
install(TARGETS ${PROJECT_NAME} DESTINATION bin)