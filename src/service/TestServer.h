//
// Created by sojoe on 11/14/18.
//

#ifndef RANA_TESTSERVER_H
#define RANA_TESTSERVER_H

#include <asio.hpp>
#include <string>

#

class TestServer
{
public:
    TestServer();

    std::string makeDayTimeString();

    static std::atomic_bool stopService;
};


#endif //RANA_TESTSERVER_H
