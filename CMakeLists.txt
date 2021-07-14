cmake_minimum_required(VERSION 3.10)
project(client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-fpermissive")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Required packages
find_package(ENet REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(Protobuf REQUIRED)

# Dependencies and include locations
include_directories(include ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${ENET_INCLUDE_DIRS} ${Protobuf_INCLUDE_DIRS})

file(GLOB SOURCES src/*.cpp src/*.c proto/*.cc)
file(GLOB HEADERS include/*.hpp include/*.h proto/*.h)

# Compile proto files
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/gamestate.proto proto/playeraction.proto)

add_executable(client ${SOURCES} ${HEADERS} ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(client ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${ENET_LIBRARIES} ${Protobuf_LIBRARIES})
