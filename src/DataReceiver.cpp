/// @brief Dperrett

#include "DataReceiver.h"
#include "FileCheckUtility.h"
#include "DataPacketReceiver.h"

void DataReceiver::SendDataToFileSystem(const DataHeaderStruct & header, const std::vector<uint8_t> &bytes)
{
 //   std::cout << data << std::endl;
    std::string path = CheckFilePath(header);
    if(header.fileNameLength > 0)
    {
        if(header.type == 1)
        {
            path += header.fileName;
            std::ofstream file(path, std::ios::binary);
            file.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
            file.close();
        }
    }
}

//-----------------------------------------------------------------------------
tcp::socket DataReceiver::GetInformation(tcp::socket socket, DataHeaderStruct & header)
{
    std::size_t headerLengthSize = 8;	
    std::int64_t headerLength;
    std::size_t totalLength = 0;
    
    boost::system::error_code error;
    boost::asio::streambuf buf;
    boost::asio::read(socket, boost::asio::buffer(&headerLength, sizeof(std::int64_t)));
    boost::asio::read(socket, buf, boost::asio::transfer_exactly(headerLength));
    std::memcpy(&(header.headerEncrypted), boost::asio::buffer_cast<const char*>(buf.data()) + totalLength, headerEncryptedSize);
    totalLength += headerEncryptedSize;
    std::memcpy(&(header.requestType), boost::asio::buffer_cast<const char*>(buf.data()) + totalLength, headerEncryptedSize);
    totalLength += requestTypeSize;
    std::memcpy(&(header.dataLength), 
                  boost::asio::buffer_cast<const char*>(buf.data()) + totalLength, dataLengthSize);

    totalLength += dataLengthSize;
    std::memcpy(&(header.headerVersion), 
                  boost::asio::buffer_cast<const char*>(buf.data()) + totalLength, headerVersionSize);
    totalLength += headerVersionSize;
    std::memcpy(&(header.id), 
                  boost::asio::buffer_cast<const char*>(buf.data()) + totalLength, idSize);
    totalLength += idSize;
    std::memcpy(&(header.userNameLength), 
                  boost::asio::buffer_cast<const char*>(buf.data()) + totalLength, userNameLengthSize);
    totalLength += userNameLengthSize;
    std::memcpy(&(header.fileNameLength), 
                  boost::asio::buffer_cast<const char*>(buf.data()) + totalLength, fileNameLengthSize);
    totalLength += fileNameLengthSize;
    std::memcpy(&(header.type), 
                  boost::asio::buffer_cast<const char*>(buf.data()) + totalLength, typeSize);
    totalLength += typeSize;
    std::memcpy(&(header.fileType), 
                  boost::asio::buffer_cast<const char*>(buf.data()) + totalLength, fileTypeSize);
    totalLength += fileTypeSize;
    std::memcpy(&(header.part), 
                  boost::asio::buffer_cast<const char*>(buf.data()) + totalLength, partSize);
    totalLength += partSize;
    std::memcpy(&(header.of), 
                  boost::asio::buffer_cast<const char*>(buf.data()) + totalLength, ofSize);
    totalLength += ofSize;
    std::memcpy(&(header.dataEncryption), 
                  boost::asio::buffer_cast<const char*>(buf.data()) + totalLength, dataEncryptionSize);
    totalLength += dataEncryptionSize;
    std::memcpy(&(header.fileName), 
                  boost::asio::buffer_cast<const char*>(buf.data()) + totalLength, fileNameSize);
    totalLength += fileNameSize;
    std::memcpy(&(header.userName), 
                  boost::asio::buffer_cast<const char*>(buf.data()) + totalLength, userNameSize);
    
    return std::move(socket);
}

