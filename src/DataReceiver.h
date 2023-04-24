/// @brief Dperrett code
#pragma once

#include <iostream>
#include <thread>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/endian/conversion.hpp>
#include <boost/filesystem.hpp>

#include "DataPacketReceiver.h"

using namespace boost::asio;
using ip::tcp;

class DataReceiver
{
    public:

    /// @brief constructor
    DataReceiver()
    {

    }

    virtual ~DataReceiver()
    {

    }

    ip::tcp::socket GetInformation(tcp::socket socket, DataHeaderStruct & header);

    void SendDataToFileSystem(const DataHeaderStruct & header, const std::vector<uint8_t> &bytes);

};