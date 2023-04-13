#include <iostream>
#include <thread>
#include <boost/asio.hpp>
#include <boost/endian/conversion.hpp>

#include "DataPacketReceiver.h"

using namespace boost::asio;
using ip::tcp;

void SendDataToFileSystem(const DataHeaderStruct & header, const char * data)
{
    std::cout << data << std::endl;
}

void GetInformation(tcp::socket socket)
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
        boost::asio::read(socket, dataBuf, boost::asio::transfer_exactly(header.dataLength));
     
        const char* data = boost::asio::buffer_cast<const char*>(dataBuf.data());
        SendDataToFileSystem(header, data);
        std::string msgBack = "Received OK";
        char * buffer = new char[strlen(msgBack.c_str())];
        std::memcpy(buffer, msgBack.c_str(), strlen(msgBack.c_str()));
        boost::asio::write( socket, boost::asio::buffer(buffer, strlen(msgBack.c_str())));
    }
    else
    {
    }
}


void HandleConnection(tcp::socket socket) 
{
    std::cout << "New connection from " << socket.remote_endpoint().address().to_string() << std::endl;
    bool error = false;
    std::string msgBack = "Received OK";
    try
    {
        GetInformation(std::move(socket));
    }
    catch(...)
    {
        error = true;
        msgBack = "Error";
    }
  
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

