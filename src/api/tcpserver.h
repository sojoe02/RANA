#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include <pthread.h>
#include <thread>  //REMOVE

#define MAXPACKETSIZE 8192

class tcpserver
{
    //  Public methods
    public:
        void setup(int port);
        std::string receive();
        std::string getMessage();

        void Send(std::string msg);
        void detach();
        void clean();

    //  Public attributes
    public:
        int sockfd;//
        int newsockfd;//
        int pid;//
        int n;//

        pthread_t serverThread;//

        char msg[ MAXPACKETSIZE ];//

        static std::string Message; //

        struct sockaddr_in serv_addr; //
        struct sockaddr_in cli_addr; //

    //  Private methods
    private:
        static void * Task(void * argv);

    //  Private attributes
    private:

};

#endif // TCPSERVER_H
