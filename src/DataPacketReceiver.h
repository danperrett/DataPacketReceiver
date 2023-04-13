#pragma once
#include <unistd.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <memory>
#include <stdexcept>
#include <string>

#pragma pack(1)
struct DataHeaderStruct
{ 
    std::uint8_t  headerEncrypted;
    std::uint8_t  requestType;
    std::uint64_t dataLength;
    std::uint16_t headerVersion;
    std::int32_t  id;
    std::int32_t  userNameLength;
    std::int32_t  fileNameLength;
    std::uint8_t  type;
    std::uint16_t fileType;
    std::uint16_t part;
    std::uint16_t of;
    std::uint32_t dataEncryption;
    char          fileName[30];
    char          userName[30];
    std::uint64_t ext1;
    std::uint64_t ext2;
    std::uint64_t ext3;
    std::uint64_t ext4;
};

const std::size_t headerEncryptedSize = sizeof(std::uint8_t);
const std::size_t requestTypeSize = sizeof(std::uint8_t);
const std::size_t dataLengthSize = sizeof(std::uint64_t);
const std::size_t headerVersionSize = sizeof(std::uint16_t);
const std::size_t idSize = sizeof(std::uint32_t);
const std::size_t userNameLengthSize = sizeof(std::int32_t);
const std::size_t fileNameLengthSize = sizeof(std::int32_t);
const std::size_t typeSize = sizeof(std::uint8_t);
const std::size_t fileTypeSize = sizeof(std::uint16_t);
const std::size_t partSize = sizeof(std::uint16_t);
const std::size_t ofSize = sizeof(std::uint16_t);
const std::size_t dataEncryptionSize = sizeof(std::uint32_t);
const std::size_t fileNameSize = 30;
const std::size_t userNameSize = 30;

