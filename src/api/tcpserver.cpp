#include "tcpserver.h"

std::string tcpserver::Message;

void* tcpserver::Task(void *arg)
{
    int n;

    int newsockfd = (long)arg;
    char msg[MAXPACKETSIZE];
    pthread_detach(pthread_self());

    while(true)
    {

        n=recv(newsockfd,msg,MAXPACKETSIZE,0);

        if(n==0)
        {

            close(newsockfd);
            break;
        }

        msg[n]=0;
        //send(newsockfd,msg,n,0);

        Message = std::string(msg);

    }

    return 0;
}

void tcpserver::setup(int port)
{

    sockfd=socket(AF_INET,SOCK_STREAM,0);

    memset(&serv_addr,0,sizeof(serv_addr));

    serv_addr.sin_family=AF_INET;

    serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);

    serv_addr.sin_port=htons(port);

    bind(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(sockfd,5);

}

std::string tcpserver::receive()
{

    std::string str;

    while(1)
    {

        socklen_t sosize  = sizeof(cli_addr);

        newsockfd = accept(sockfd,(struct sockaddr*)&cli_addr,&sosize);

        str = inet_ntoa(cli_addr.sin_addr);

        pthread_create(&serverThread,NULL,&Task,(void *)newsockfd);

    }

    return str;
}

std::string tcpserver::getMessage()
{

    return Message;
}

void tcpserver::Send(std::string msg)
{

    send(newsockfd,msg.c_str(),msg.length(),0);

}

void tcpserver::clean()
{

    Message = "";

    memset(msg, 0, MAXPACKETSIZE);

}

void tcpserver::detach()
{

    close(sockfd);

    close(newsockfd);

}
