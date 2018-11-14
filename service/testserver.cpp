//
// Created by sojoe on 11/14/18.
//

#include "testserver.h"

using asio::ip::tcp;

TestServer::TestServer()
{


}

std::string TestServer::makeDayTimeString()
{
    std::time_t now = std::time(0); //nullptr ?

    return std::ctime(&now);
}
