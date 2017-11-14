#include "tcpserver.h"

tcpserver::tcpserver()
{
    //portno = parser::getPortNo();
    socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error("ERROR opening socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr->sin_family = AF_INET;
    serv_addr->sin_addr.s_addr = INADDR_ANY;
    serv_addr->sin_port = htons(portno);

    if( bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        error("ERROR on binding");
    }

    listen(sockfd,5);
    clilen = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0){
        error("ERROR on accept");
    }

    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0){
        error("ERROR reading from socket");
    }
    std::cout << "Here is the message: " << buffer << std::endl;

    //printf("Here is the message: %s\n",buffer);
    n = write(newsockfd,"I got your message",18);

    if (n < 0){
        error("ERROR writing to socket");
    }
}

tcpserver::~tcpserver()
{
    std::cout << "RIP TCP - Server" << std::endl;
}

void tcpserver::error(std::string msg)
{
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}
