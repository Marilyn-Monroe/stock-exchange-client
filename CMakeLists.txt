cmake_minimum_required(VERSION 3.14)
project(stock_exchange_client CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

add_subdirectory(third_party/nlohmann_json)

add_library(${PROJECT_NAME}_objs OBJECT
        src/common.hpp
)

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs Threads::Threads ${Boost_LIBRARIES} nlohmann_json)
