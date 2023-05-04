
#include "DataSender.h"
#include "DataPacketReceiver.h"
#include "FileCheckUtility.h"


std::string DataSender::GetFilePath(const DataHeaderStruct & header, std::size_t & size)
{
    std::string filePath = LOCATION + header.userName + "/" + std::to_string(header.id) + "/" + header.fileName;

    if (!boost::filesystem::exists(filePath)) 
    {
        std::cout << "File not found: " << header.fileName << "\n";
        return "";
    }
    std::ifstream input(filePath, std::ios::binary);

    if (!input.is_open()) 
    {
        std::cout << "Failed to open file: " << header.fileName << "\n";
        return "";
     }
    // get the size of the file
    size = boost::filesystem::file_size(filePath);
    std::time_t seed_value = std::time(nullptr);

    return filePath;
}

tcp::socket DataSender::SendResponseToSocket(tcp::socket socket, DataHeaderStruct & header, const std::string & message)
{
    std::uint64_t lengthOfHeader = sizeof(header);
    header.requestType = RESPOND;
    header.userNameLength = 0;
    header.fileNameLength = 0;
    std::memset(header.fileName, 0, 30);
    std::memset(header.userName, 0, 30);
    header.dataLength = strlen(message.c_str());
    std::size_t totalLength = header.dataLength + lengthOfHeader + sizeof(std::uint64_t);
    char * buffer = new char[totalLength];

    std::memset(buffer, 0, totalLength);
    std::memcpy(buffer, &lengthOfHeader, sizeof(lengthOfHeader));
    std::memcpy(buffer + sizeof(std::uint64_t), &header, lengthOfHeader);
    std::memcpy(buffer + sizeof(std::uint64_t) + lengthOfHeader, message.c_str(), header.dataLength);
    boost::system::error_code error;
    boost::asio::write( socket, boost::asio::buffer(buffer, totalLength), error );
    if( error ) 
    {
        std::cout << "send failed: " << error.message() << std::endl;
    }
     
    delete buffer;
    return std::move(socket);
}

tcp::socket DataSender::GetDataFromSocket(tcp::socket socket, const std::string & fileName, const std::uint16_t id, const RequestType type)
{
    DataHeaderStruct header;
    std::cout << "size of header " << sizeof(header) << std::endl;
    std::memset(&header, 0, sizeof(header));
    std::memset(&header, 0, sizeof(header));
    std::uint64_t lengthOfHeader = sizeof(header);
    header.headerEncrypted = 23;
    header.requestType = type;
    header.headerVersion = 7; 
    header.id = id;
    std::string userName = "dperrett";

    header.userNameLength = strlen(userName.c_str());
    header.fileNameLength = strlen(fileName.c_str());
    std::memcpy(header.userName, userName.c_str(), header.userNameLength); 
    std::memcpy(header.fileName, fileName.c_str(), header.fileNameLength); 
    header.dataLength = 0;

    std::size_t totalLength = lengthOfHeader + sizeof(std::uint64_t);
    std::cout << "totalLength " << totalLength << std::endl;
    char * buffer = new char[totalLength];
    std::memset(buffer, 0, totalLength);
    std::memcpy(buffer, &lengthOfHeader, sizeof(lengthOfHeader));
    std::memcpy(buffer + sizeof(std::uint64_t), &header, lengthOfHeader);
    
    boost::system::error_code error;
    boost::asio::write( socket, boost::asio::buffer(buffer, totalLength), error );
    if( !error ) 
    {
        std::cout << "Client sent hello message!" << std::endl;
    }

    return std::move(socket);
}

tcp::socket DataSender::SendDataToSocket(tcp::socket socket, const std::string & fileName, const std::size_t sizeOfFile, const std::uint16_t id, const RequestType type)
{
     std::ifstream input(fileName, std::ios::binary);
     char* fileBuffer = new char[sizeOfFile];
     input.read(fileBuffer, sizeOfFile);
     DataHeaderStruct header;
     std::cout << "size of header " << sizeof(header) << std::endl;
     std::memset(&header, 0, sizeof(header));
     std::uint64_t lengthOfHeader = sizeof(header);
     header.headerEncrypted = 23;
     header.requestType = type;
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
        std::cout << "Client sent hello message!" << std::endl;
    }
    else 
    {
        std::cout << "send failed: " << error.message() << std::endl;
    }
    delete buffer;
    return std::move(socket);
}
