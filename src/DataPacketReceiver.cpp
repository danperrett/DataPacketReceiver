#include <iostream>
#include <thread>
#include <fstream>
#include <boost/asio.hpp>


#include "DataPacketReceiver.h"
#include "DataReceiver.h"
#include "DataSender.h"
#include "FileCheckUtility.h"


using namespace boost::asio;
using ip::tcp;


//-----------------------------------------------------------------------------
void HandleConnection(tcp::socket socket) 
{
    std::cout << "New connection from " << socket.remote_endpoint().address().to_string() << std::endl;
    bool error = false;
    std::string msgBack = "Received OK";
    DataReceiver receiver;
    DataHeaderStruct header;
    DataSender sender;
    try
    {
        socket = receiver.GetInformation(std::move(socket), header);
        switch(header.requestType)
        {
        case PUT:
        {   /// Put request
            boost::asio::streambuf dataBuf;
            std::size_t receivedSize = boost::asio::read(socket, dataBuf, boost::asio::transfer_exactly(header.dataLength));
            std::vector<uint8_t> bytes(boost::asio::buffers_begin(dataBuf.data()), boost::asio::buffers_end(dataBuf.data()));    
            receiver.SendDataToFileSystem(header, bytes);
            std::string message = "File Received and stored correctly";
            socket = sender.SendResponseToSocket(std::move(socket), header, message);
        }
        break;
        case  GET:
        {
           
            std::size_t size;
            std::string filePath = sender.GetFilePath(header, size);
            socket = sender.SendDataToSocket(std::move(socket), filePath, size, header.id, GET);
        }
        break;
        case LIST:
        {
            std::string userFiles = GetListOfUserFiles(std::string(header.userName));
            socket = sender.SendResponseToSocket(std::move(socket), header, userFiles);
        }
        break;
        default:
            break;
        
        }
        std::string msgBack = "Received OK";
        
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

