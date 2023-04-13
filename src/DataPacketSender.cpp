#include <iostream>
#include <boost/asio.hpp>

#include "DataPacketReceiver.h"

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

int main() 
{
     DataHeaderStruct header;
     std::cout << "size of header " << sizeof(header) << std::endl;
     std::memset(&header, 0, sizeof(header));
     std::uint64_t lengthOfHeader = sizeof(header);
     header.headerEncrypted = 23;
     header.requestType = 0;
     header.headerVersion = 7; 
     header.id = 1;
     std::string userName = "dperrett";
     std::string fileName = "test.dat";
     header.userNameLength = strlen(userName.c_str());
     header.fileNameLength = strlen(fileName.c_str());
     std::memcpy(header.userName, userName.c_str(), header.userNameLength); 
     std::memcpy(header.fileName, fileName.c_str(), header.fileNameLength); 
     header.fileNameLength = 0;
     header.type = 1;
     header.fileType = 1;
     header.part = 1;
     header.of = 1;
     header.dataEncryption = 0;
     lengthOfHeader = sizeof(header);
     std::cout << "size of header " << sizeof(header) << std::endl;

     boost::asio::io_service io_service;
//socket creation
     tcp::socket socket(io_service);
//connection
     socket.connect( tcp::endpoint( boost::asio::ip::address::from_string("127.0.0.1"), 12345 ));
// request/message from client
     const std::string msg = "Hello from Client!\n";
     const std::size_t msgLength = strlen(msg.c_str());
     header.dataLength = msgLength;
     std::size_t totalLength = msgLength + lengthOfHeader + sizeof(std::uint64_t);
     std::cout << "totalLength " << totalLength << std::endl;
     char * buffer = new char[totalLength];
     std::memset(buffer, 0, totalLength);
     std::memcpy(buffer, &lengthOfHeader, sizeof(lengthOfHeader));
     std::memcpy(buffer + sizeof(std::uint64_t), &header, lengthOfHeader);
     std::memcpy(buffer + sizeof(std::uint64_t) + lengthOfHeader, msg.c_str(), msgLength);
     boost::system::error_code error;
     boost::asio::write( socket, boost::asio::buffer(buffer, totalLength), error );
     if( !error ) 
     {
        cout << "Client sent hello message!" << endl;
     }
     else 
     {
        cout << "send failed: " << error.message() << endl;
     }
 // getting response from server
    boost::asio::streambuf receive_buffer;
    boost::asio::read(socket, receive_buffer, boost::asio::transfer_all(), error);
    if( error && error != boost::asio::error::eof ) {
        cout << "receive failed: " << error.message() << endl;
    }
    else {
        const char* data = boost::asio::buffer_cast<const char*>(receive_buffer.data());
        cout << data << endl;
    }
    return 0;
}
