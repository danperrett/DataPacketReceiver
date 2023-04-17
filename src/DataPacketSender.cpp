#include <iostream>
#include <boost/asio.hpp>
#include <boost/random.hpp>
#include <fstream>
#include <boost/filesystem.hpp>
#include <ctime>
#include "DataPacketReceiver.h"
#include "DataSender.h"
#include "DataReceiver.h"

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;


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
     size_t size = boost::filesystem::file_size(filepath);
     std::time_t seed_value = std::time(nullptr);
     boost::random::mt19937 rng(seed_value); // Mersenne Twister pseudo-random generator
     boost::random::uniform_int_distribution<> dist(1, 65000); // Uniform distribution between 1 and


     std::uint16_t randomNumber = dist(rng); // Generate a random numbe
     std::cout << randomNumber << std::endl;
     std::cout << size << std::endl;
      boost::asio::io_service io_service;
//socket creation
     tcp::socket socket(io_service);
//connection
     socket.connect( tcp::endpoint( boost::asio::ip::address::from_string("188.166.158.22"), 12345 ));
     DataSender sender;
     socket = sender.SendDataToSocket(std::move(socket), filepath.string(), size, randomNumber);
     DataReceiver receiver;
     DataHeaderStruct header;
     receiver.GetInformation(std::move(socket),header);
     switch(header.requestType)
     {
          case RESPOND:
          {
               boost::asio::streambuf dataBuf;
               std::size_t receivedSize = boost::asio::read(socket, dataBuf, boost::asio::transfer_exactly(header.dataLength));
               
          }
     }
     
}
    