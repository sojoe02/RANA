//
// Created by sojoe on 11/14/18.
//
#include <iostream>
#include <functional>
#include "RanaInstance.h"


RanaInstance::RanaInstance(asio::io_service *io)
        : io(io), count(0), timer(*io, asio::chrono::seconds(1))
{
}

void RanaInstance::asyncPrint()
{
    std::cout << "Async printing :)" << std::endl;
    if (count < 5)
    {
        ++count;
        timer.expires_at(timer.expiry() + asio::chrono::seconds(1));
        timer.async_wait(std::bind(&RanaInstance::asyncPrint, this));
        std::cout << count << std::endl;
    }
}
