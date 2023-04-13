#include <iostream>
#include <thread>
#include <fstream>
#include <boost/asio.hpp>
#include <boost/endian/conversion.hpp>
#include <boost/filesystem.hpp>

#include "DataPacketReceiver.h"

using namespace boost::asio;
using ip::tcp;

static const std::string LOCATION = "/mnt/buffer/";

std::string CheckFilePath(const DataHeaderStruct & header)
{
    std::string filePath = LOCATION + header.userName;
    if ( !boost::filesystem::exists( filePath ) )
    {
        boost::filesystem::create_directory(filePath);
        filePath += "/" + std::to_string(header.id);
        boost::filesystem::create_directory(filePath);
    }
    else
    {
        filePath += "/" + std::to_string(header.id);
        if ( !boost::filesystem::exists( filePath ) )
        {
            boost::filesystem::create_directory(filePath);
        }
    }
    return filePath + "/";
}
void SendDataToFileSystem(const DataHeaderStruct & header, const std::vector<uint8_t> &bytes)
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

tcp::socket GetInformation(tcp::socket socket)
{
    std::size_t headerLengthSize = 8;	
    std::int64_t headerLength;
    std::size_t totalLength = 0;
    DataHeaderStruct header;
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
    if(header.requestType == 0)
    {   /// Put request
        boost::asio::streambuf dataBuf;
        std::size_t receivedSize = boost::asio::read(socket, dataBuf, boost::asio::transfer_exactly(header.dataLength));
        std::vector<uint8_t> bytes(boost::asio::buffers_begin(dataBuf.data()), boost::asio::buffers_end(dataBuf.data()));    
        SendDataToFileSystem(header, bytes);
        
    }
    else
    {
    }
    return std::move(socket);
}


void HandleConnection(tcp::socket socket) 
{
    std::cout << "New connection from " << socket.remote_endpoint().address().to_string() << std::endl;
    bool error = false;
    std::string msgBack = "Received OK";
    try
    {
        socket = GetInformation(std::move(socket));
        std::string msgBack = "Received OK";
        
    }
    catch(...)
    {
        error = true;
        msgBack = "Error";
    }
    char * buffer = new char[strlen(msgBack.c_str())];
    std::memcpy(buffer, msgBack.c_str(), strlen(msgBack.c_str()));
    boost::asio::write( socket, boost::asio::buffer(buffer, strlen(msgBack.c_str())));
  
}

int main() 
{
  io_service service;
  tcp::acceptor acceptor(service, tcp::endpoint(tcp::v4(), 12345));
  
  while (true) 
  {
      tcp::socket socket(service);
      acceptor.accept(socket);

      std::thread thread(HandleConnection, std::move(socket));
      thread.detach();
  }

  return 0;
}

