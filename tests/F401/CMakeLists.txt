cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

# Setup C compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# Setup C++ compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME ST7567_Nucleo_F401)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Enable CMake support for ASM, C, and CXX (C++) languages
enable_language(C CXX ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    Src/user.cpp
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Compiler options for C++ safety and optimization on MCU
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:
        -fno-exceptions
        -fno-rtti
        -fno-threadsafe-statics
    >
)


add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../../" "${CMAKE_CURRENT_BINARY_DIR}/ST7567_build")

if(NOT ACTIVE_DEMO STREQUAL "NONE")
    set(DEMO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../examples/${ACTIVE_DEMO}")
    
    # 1. Cerca ricorsivamente tutti i file .cpp e .c (importante se hai file C)
    file(GLOB_RECURSE DEMO_SOURCES 
        "${DEMO_DIR}/*.cpp"
        "${DEMO_DIR}/*.c"
    )
    
    # 2. Aggiungi i sorgenti al progetto
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${DEMO_SOURCES})
    
    # 3. Includi la cartella principale della demo
    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${DEMO_DIR})
    
    # 4. (Opzionale ma consigliato) Includi automaticamente tutte le sottocartelle per gli header
    # Questo serve se serial.h è dentro una sottocartella di ST7567_SerialWrapper
    file(GLOB_RECURSE DEMO_INC_DIRS LIST_DIRECTORIES true "${DEMO_DIR}/*")
    foreach(dir ${DEMO_INC_DIRS})
        if(IS_DIRECTORY ${dir})
            target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${dir})
        endif()
    endforeach()
endif()

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    # Add user defined libraries
    ST7567_FB_STM32	
    
)
