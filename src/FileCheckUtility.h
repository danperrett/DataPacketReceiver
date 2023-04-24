#pragma once
#include "DataPacketReceiver.h"
#include <boost/filesystem.hpp>


static const std::string LOCATION = "/mnt/buffer/";

std::string CheckFilePath(const DataHeaderStruct & header);

std::string GetListOfUserFiles(const std::string & userName);