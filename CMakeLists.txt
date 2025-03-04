# Enable export of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Specify minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Define project name
project(8bitVM)

# Set the C standard
set(CMAKE_C_STANDARD 99)

# Manually specify SDL2 paths
set(SDL2_INCLUDE_DIRS /opt/homebrew/Cellar/sdl2/2.32.2/include/SDL2)
set(SDL2_LIBRARIES /opt/homebrew/Cellar/sdl2/2.32.2/lib/libSDL2.dylib)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include ${SDL2_INCLUDE_DIRS})

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

# Source files
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.c)

# Add executable
add_executable(main ${SOURCES})

# Set the output directory for the executable
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Link libraries
target_link_libraries(main ${SDL2_LIBRARIES} "-framework Cocoa" "-framework OpenGL" "-framework CoreAudio")

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Custom target to generate .clang_complete
add_custom_target(
  generate_clang_complete
  COMMAND python ${CMAKE_SOURCE_DIR}/scripts/cc_args.py ${CMAKE_BINARY_DIR}/compile_commands.json > ${CMAKE_SOURCE_DIR}/.clang_complete
  DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
)
