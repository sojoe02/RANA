//
// Created by sojoe on 11/14/18.
//

#ifndef RANA_RANAINSTANCE_H
#define RANA_RANAINSTANCE_H

#include <asio.hpp>

class RanaInstance
{
public:
    RanaInstance(asio::io_service *io);

    void asyncPrint();

private:
    asio::io_service *io;
    asio::steady_timer timer;
    int count;
};


#endif //RANA_RANAINSTANCE_H
