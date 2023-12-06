cmake_minimum_required(VERSION 3.27.4)
project(Chess VERSION 0.1.0 LANGUAGES CXX)
# set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

file(GLOB_RECURSE CHESS_SRC src/*.cpp)
add_executable(${PROJECT_NAME} ${CHESS_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
