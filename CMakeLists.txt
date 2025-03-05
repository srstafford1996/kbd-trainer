# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

# Project name
project(KBDTrainer VERSION 1.0)

# C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(SDL3_DIR "C:\\SDL\\SDL3")
set(SDL3_IMAGE_DIR "C:\\SDL\\SDL3_image")
set(SDL3_TTF_DIR "C:\\SDL\\SDL3_ttf")

find_package(SDL3 REQUIRED HINTS ${SDL3_DIR})
find_package(SDL3_IMAGE REQUIRED HINTS ${SDL3_IMAGE_DIR})
find_package(SDL3_TTF REQUIRED HINTS ${SDL3_TTF_DIR})

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
target_link_libraries(${PROJECT_NAME} SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf) 

# Build the exe in the base project folder
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin"
)

# Get DLL locations
get_target_property(SDL3_DLL_PATH SDL3::SDL3 IMPORTED_LOCATION)
get_target_property(SDL3_IMAGE_DLL_PATH SDL3_image::SDL3_image IMPORTED_LOCATION)
get_target_property(SDL3_TTF_DLL_PATH SDL3_ttf::SDL3_ttf IMPORTED_LOCATION)


add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL3_DLL_PATH}"
        "${CMAKE_SOURCE_DIR}/bin"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL3_IMAGE_DLL_PATH}"
        "${CMAKE_SOURCE_DIR}/bin"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SDL3_TTF_DLL_PATH}"
        "${CMAKE_SOURCE_DIR}/bin"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "${CMAKE_SOURCE_DIR}/bin/assets"
    COMMENT "Copying SDL3 and SDL3_image DLLs to output directory"
)