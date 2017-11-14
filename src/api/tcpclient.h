#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include <iostream>
#include <string.h>

class tcpclient
{
    //  Public methods
    public:
        tcpclient();
        ~tcpclient();

    //  Public attributes
    public:

    //  Private methods
    private:
        void error(std::string msg);

    //  Private attributes
    private:
        int sockfd;
        int portno;
        int n;

        char buffer[256];

        struct sockaddr_in *serv_addr;
        struct hostent *server;
};

#endif // TCPSERVER_H
