//
// Created by sojoe on 11/14/18.
//

#include "TestClient.h"
#include <array>

using asio::ip::tcp;

TestClient::TestClient(std::string host)
{
    std::cout << "starting test client" << std::endl;
    try
    {
        asio::io_context ioContext;

        for (int i = 0; i < 5; i++)
        {
            tcp::resolver resolver(ioContext);
            tcp::resolver::results_type endpoints = resolver.resolve("localhost", "1301");

            tcp::socket socket(ioContext);
            asio::connect(socket, endpoints);

//        for(;;)
//        {
            std::array<char, 128> buffer{};
            asio::error_code error;
            size_t len = socket.read_some(asio::buffer(buffer), error);
            std::string s(static_cast<const char *>(buffer.data()), len);

            std::cout << "length of buffer : " << len << " : " << s << std::endl;

            if (error == asio::error::eof)
            {
                std::cout << "test" << std::endl;
//                break; //connection broken
            }
            else if (error)
            {
                throw asio::system_error(error);
            }

//        }
//
        }
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "Client stopping" << std::endl;

}

int main(int argc, char *argv[])
{
    TestClient testClient("something");

}
