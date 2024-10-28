cmake_minimum_required (VERSION 2.8.11)
project (Transformation)

find_package(Eigen3 REQUIRED)
# include_directories(EIGEN3_INCLUDE_DIR)
# message(STATUS "EIGEN3_INCLUDE_DIR: ${EIGEN3_INCLUDE_DIR}")#输出路径，实际上由于版本更新，现在已经不这么调用了：https://eigen.tuxfamily.org/dox/TopicCMakeGuide.html

add_executable (Transformation main.cpp)
target_link_directories(Transformation Eigen3::Eigen) #新的调用方式：https://eigen.tuxfamily.org/dox/TopicCMakeGuide.html
