# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Project name
project(KBDTrainer VERSION 1.0)

# C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SDL2_DIR "C:\\SDL2")
set(SDL2_IMAGE_DIR "C:\\SDL2_image")

find_package(SDL2 REQUIRED HINTS ${SDL2_DIR})
find_package(SDL2_IMAGE REQUIRED HINTS ${SDL2_IMAGE_DIR})

set(HEADER_FILES
    include/input.h
    include/render.h
    include/game.h
)

add_definitions(-D_AMD64_)

# Manually specify source files
set(SOURCE_FILES
    src/main.c
    src/input.c
    src/render.c
    src/game.c
)

include_directories(include)

# Add XInput library for Windows
if(WIN32)
    link_libraries(xinput)
endif()

# Create executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2::SDL2main SDL2_image::SDL2_image) 

# Build the exe in the base project folder
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
)

# Get DLL locations
get_target_property(SDL2_DLL_PATH SDL2::SDL2 IMPORTED_LOCATION)
get_target_property(SDL2_IMAGE_DLL_PATH SDL2_image::SDL2_image IMPORTED_LOCATION)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL2_DLL_PATH}"
        "${CMAKE_SOURCE_DIR}/bin"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL2_IMAGE_DLL_PATH}"
        "${CMAKE_SOURCE_DIR}/bin"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "${CMAKE_SOURCE_DIR}/bin/assets"
    COMMENT "Copying SDL2 and SDL2_image DLLs to output directory"
)