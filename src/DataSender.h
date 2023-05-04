/// @brief DPerrett

#pragma once
#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <boost/asio.hpp>
#include <boost/endian/conversion.hpp>
#include <boost/filesystem.hpp>

#include "DataPacketReceiver.h"

using namespace boost::asio;
using ip::tcp;

class DataSender
{
    public:
        /// @brief constructor
        DataSender()
        {

        }

        /// @brief destructor
        virtual ~DataSender()
        {

        }

        /// @brief
        std::string GetFilePath(const DataHeaderStruct & header, std::size_t & size);

        /// @brief  
        /// @param socket 
        /// @param fileName 
        /// @param sizeOfFile 
        /// @param id 
        /// @return 
        tcp::socket SendDataToSocket(tcp::socket socket, const std::string & fileName, const std::size_t sizeOfFile, const std::uint16_t id, const RequestType type);

        ///
        tcp::socket SendResponseToSocket(tcp::socket socket, DataHeaderStruct & header, const std::string & message);

        ///
        tcp::socket GetDataFromSocket(tcp::socket socket, const std::string & fileName, const std::uint16_t id, const RequestType type);
};