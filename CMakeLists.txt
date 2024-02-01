cmake_minimum_required(VERSION 3.18)

project(
    AITest
)

find_package(Torch REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

add_subdirectory(dpf)
add_subdirectory(plugins/AITest)

# add_executable(AITest AITest.cpp)
target_link_libraries(AITest PUBLIC "${TORCH_LIBRARIES}")
# set_property(TARGET AITest PROPERTY CXX_STANDARD 14)