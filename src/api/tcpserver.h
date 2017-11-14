#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <iostream>
#include <string.h>

class tcpserver
{
    //  Public methods
    public:
        tcpserver();
        ~tcpserver();

    //  Public attributes
    public:

    //  Private methods
    private:
        void error(std::string msg);

    //  Private attributes
    private:
        int sockfd;
        int newsockfd;
        int portno;
        socklen_t clilen;
        int n;

        char buffer[256];

        struct sockaddr_in *serv_addr;
        struct sockaddr_in *cli_addr;
};

#endif // TCPSERVER_H
