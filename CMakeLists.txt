cmake_minimum_required(VERSION 3.22)
project(ST7567_FB_STM32 CXX)

# Define the static library for the display driver
add_library(ST7567_FB_STM32 STATIC 
    ST7567_FB.cpp
)

# Expose the driver directory to whoever links this library
target_include_directories(ST7567_FB_STM32 PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ST7567_FB_STM32 PRIVATE 
    stm32cubemx
)