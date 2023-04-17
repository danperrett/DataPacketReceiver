#------------------------------------------------------------------------------
# @file   CMakeLists.txt
# @brief  CMakeLists file for src directory
#
#------------------------------------------------------------------------------
#                Copyright AP Sensing 2021. All Rights Reserved
#                           https://www.apsensing.com
#------------------------------------------------------------------------------

file(GLOB RECV_SOURCE DataPacketReceiver.cpp DataSender.cpp DataReceiver.cpp FileCheckUtility.cpp)
file(GLOB SEND_SOURCE DataPacketSender.cpp DataReceiver.cpp  FileCheckUtility.cpp DataSender.cpp)
list(REMOVE_ITEM RECV_SOURCE DataPacketSender.cpp)

# HDF5 test utility
add_executable(DataPacketReceiver ${RECV_SOURCE})
target_link_libraries(DataPacketReceiver ${Boost_LIBRARIES})
target_link_libraries(DataPacketReceiver pthread)

add_executable(DataPacketSender ${SEND_SOURCE})
target_link_libraries(DataPacketSender ${Boost_LIBRARIES})
target_link_libraries(DataPacketSender pthread)

