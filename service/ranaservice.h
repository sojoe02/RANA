//
// Created by sojoe on 11/13/18.
//

#ifndef RANA_RANASERVICE_H
#define RANA_RANASERVICE_H

#include <asio.hpp>
#include <iostream>

class RanaService
{
public:
    RanaService();

    static void asyncPrint(const asio::error_code &, asio::steady_timer *t, int *count);

    std::string makeDayTimeString();

private:
    asio::io_context io;

};


#endif //RANA_RANASERVICE_H
