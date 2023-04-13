#------------------------------------------------------------------------------
# @file   CMakeLists.txt
# @brief  CMakeLists file for root directory
#
#------------------------------------------------------------------------------
#                Copyright AP Sensing 2021. All Rights Reserved
#                           https://www.apsensing.com
#------------------------------------------------------------------------------

project (APSensingDASTools)
cmake_minimum_required (VERSION 3.5)
include(ExternalProject)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#FIND_PACKAGE(Boost 1.58.0 COMPONENTS filesystem system REQUIRED)
FIND_PACKAGE(Boost 1.58.0 COMPONENTS filesystem system REQUIRED)
MESSAGE(STATUS "** Boost Include: ${Boost_INCLUDE_DIR}")
MESSAGE(STATUS "** Boost Libraries: ${Boost_LIBRARIES}")

# Add the sub directories
add_subdirectory(src)
