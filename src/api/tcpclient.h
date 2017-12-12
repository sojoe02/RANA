#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>


class tcpclient
{
    //  Public methods
    public:
        tcpclient();

        bool setup(std::string address, int port);
        bool Send(std::string data);

        std::string receive(int size = 4096);
        std::string read();

    //  Public attributes
    public:

    //  Private methods
    private:
        void error(std::string msg);

    //  Private attributes
    private:
        int sock;
        std::string address;
        int port;
        struct sockaddr_in server;
};

#endif // TCPCLIENT_H
