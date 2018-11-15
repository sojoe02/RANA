//
// Created by sojoe on 11/13/18.
//

#include "ranaservice.h"
#include "ranainstance.h"

using asio::ip::tcp;

std::atomic<bool> RanaService::stopService;

RanaService::RanaService()
{
    stopService.store(false);

    std::cout << "Service starting" << std::endl;
    //asio::steady_timer t(io, asio::chrono::seconds(1));

    int count = 0;
    //t.async_wait(std::bind(&asyncPrint,std::placeholders::_1, &t, &count));
    //auto instance = new RanaInstance(&io);
    //instance->asyncPrint();
    try
    {
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 1301));
        for (; stopService == false;)
        {
            tcp::socket socket(io);
            acceptor.accept(socket);
            std::string message = makeDayTimeString();
            asio::error_code ignoredError;
            asio::write(socket, asio::buffer(message), ignoredError);
        }

    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    //io.run();
    //std::cout << "final count is " << count << std::endl;
}

std::string RanaService::makeDayTimeString()
{
    std::time_t now = std::time(0); //nullptr ?

    return std::ctime(&now);
}

void RanaService::asyncPrint(const asio::error_code &, asio::steady_timer *t, int *count)
{
    if (*count < 5)
    {
        ++(*count);
        t->expires_at(t->expiry() + asio::chrono::seconds(1));
        t->async_wait(std::bind(&asyncPrint, std::placeholders::_1, t, count));
        std::cout << *count << std::endl;
    }

    std::cout << "Async printing :)" << std::endl;
}

