#include "tcpclient.h"

tcpclient::tcpclient()
{
    //portno = parser::getPortNo();
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        error("ERROR opening socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr->sin_family = AF_INET;

    //server = gethostbyname(parser::getHostName());
    if(server == NULL){
        error("ERROR, no such host");
    }

    bcopy((char *)server->h_addr, (char *)&serv_addr->sin_addr.s_addr, server->h_length);
    serv_addr->sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0){
       error("ERROR connecting");
    }

    bzero(buffer,256);
    n = write(sockfd,"Here is a message from client",18);
    if (n < 0){
        error("ERROR writing to socket");
    }

    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0){
        error("ERROR reading from socket");
    }
    std::cout << "Here is the message: " << buffer << std::endl;

}

tcpclient::~tcpclient()
{
    std::cout << "RIP TCP - Client" << std::endl;
}

void tcpclient::error(std::string msg)
{
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}
