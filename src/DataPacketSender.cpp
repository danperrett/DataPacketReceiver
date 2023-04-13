#include <iostream>
#include <boost/asio.hpp>
#include <boost/random.hpp>
#include <fstream>
#include <boost/filesystem.hpp>

#include "DataPacketReceiver.h"

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

bool SendDataToServer(const std::string & filename, std::size_t sizeOfFile, std::uint16_t id);

int main(int argc, char* argv[]) 
{
     if(argc != 2)
     {
          cout << "Usage: " << argv[0] << " filename\n";
          return 1;
     }


     boost::filesystem::path filepath(argv[1]);

     if (!boost::filesystem::exists(filepath)) 
     {
          cout << "File not found: " << argv[1] << "\n";
          return 1;
     }
     std::ifstream input(filepath.string(), std::ios::binary);

     if (!input.is_open()) 
     {
          cout << "Failed to open file: " << argv[1] << "\n";
          return 1;
     }
      // get the size of the file
     input.seekg(0, std::ios::end);
     size_t size = input.tellg();
     input.seekg(0, std::ios::beg);
     input.close();

     boost::random::mt19937 rng; // Mersenne Twister pseudo-random generator
     boost::random::uniform_int_distribution<> dist(1, 65000); // Uniform distribution between 1 and 1000

     std::uint16_t randomNumber = dist(rng); // Generate a random numbe
     SendDataToServer(filepath.string(), size, randomNumber);
}

bool SendDataToServer(const std::string & fileName, std::size_t sizeOfFile, std::uint16_t id)
{
     std::ifstream input(fileName, std::ios::binary);
     char* fileBuffer = new char[sizeOfFile];
     input.read(fileBuffer, sizeOfFile);
     DataHeaderStruct header;
     std::cout << "size of header " << sizeof(header) << std::endl;
     std::memset(&header, 0, sizeof(header));
     std::uint64_t lengthOfHeader = sizeof(header);
     header.headerEncrypted = 23;
     header.requestType = 0;
     header.headerVersion = 7; 
     header.id = id;
     std::string userName = "dperrett";
    
     header.userNameLength = strlen(userName.c_str());
     header.fileNameLength = strlen(fileName.c_str());
     std::memcpy(header.userName, userName.c_str(), header.userNameLength); 
     std::memcpy(header.fileName, fileName.c_str(), header.fileNameLength); 
     
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
     
     header.dataLength = sizeOfFile;
     std::size_t totalLength = sizeOfFile + lengthOfHeader + sizeof(std::uint64_t);
     std::cout << "totalLength " << totalLength << std::endl;
     char * buffer = new char[totalLength];
     std::memset(buffer, 0, totalLength);
     std::memcpy(buffer, &lengthOfHeader, sizeof(lengthOfHeader));
     std::memcpy(buffer + sizeof(std::uint64_t), &header, lengthOfHeader);
     std::memcpy(buffer + sizeof(std::uint64_t) + lengthOfHeader, fileBuffer, sizeOfFile);
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
