#include "bopthook.h"

bopthook::bopthook(Cli *_cli, size_t dim, bayesopt::Parameters _param, int _portNumber):
    ContinuousModel(dim, _param)
{
    this->cli = _cli;
    this->tcp = new tcpserver;
    this->tcp->setup(_portNumber);

    pthread_t t1;
    pthread_detach(pthread_self());
    pthread_create(&t1, nullptr, &bopthook::bopthook_helper, this);

    this->setFilePathAttribute();

    usleep(100);

    bool tmp_flag = true;
    while(tmp_flag)
    {
        std::string str = tcp->getMessage();
        if (str == "rdy\n"){
            tcp->clean();
            tmp_flag = false;
            std::cout << "Got ready from matlab" << std::endl;
        }
        usleep(100);
    }
}

void bopthook::sentMessageViaTcp(std::string msg){
    tcp->Send(msg);
    tcp->clean();
}

void bopthook::sentParametersViaTcp(vectord rowOfParam){
    std::string msgToMatlab;

    msgToMatlab += std::to_string(rowOfParam.size());
    msgToMatlab += ',';
    for(size_t i = 0; i < rowOfParam.size(); i++){
        msgToMatlab += std::to_string(rowOfParam(i));
        msgToMatlab += ',';
    }
    std::cout << "msg: " << msgToMatlab << std::endl;

    sentMessageViaTcp(msgToMatlab);

    bool tmp_flag = true;
    while(tmp_flag)
    {
        std::string str = tcp->getMessage();
        if (str == "filesRdy\n"){
            tcp->clean();
            tmp_flag = false;
            std::cout << "Got filesRdy from matlab" << std::endl;
        }
        usleep(100);
    }
}

double bopthook::messageSimDoneAndGetError(std::string msgToMatlab){
    std::cout << "msg: " << msgToMatlab << std::endl;
    tcp->Send(msgToMatlab);
    tcp->clean();

    double processedError = 1.0;

    bool tmp_flag = true;
    while(tmp_flag)
    {
        std::string str = tcp->getMessage();
        if (str != ""){
            processedError = std::stod(str);
            tcp->clean();
            tmp_flag = false;
        }
        usleep(100);
    }
    return processedError;
}

double bopthook::evaluateSample( const vectord &Xi ){
    sentParametersViaTcp(Xi);
    /**
      Run all simulation sequences here
    **/
    bool tmpFlag = true;
    while(tmpFlag){
        std::string filePath = getNextFile();
        if(filePath != "noMoreFiles"){
            /**
                Run simulation
            **/
            std::cout <<"Run simulation with - " << filePath<< std::endl;
            cli->boptRunController(filePath);
        }else{
            std::cout << "No more simulations" << std::endl << std::endl;
            resetFilePathList();
            tmpFlag = false;
        }
    }

    double processedError = messageSimDoneAndGetError("simsDone\n");
    return processedError;
}

void bopthook::saveCurrentState(){
    this->saveOptimization(state);
    state.saveToFile("state.dat");
}

void bopthook::resetFilePathList(){
    currentSimFile = 0;
}

std::string bopthook::getNextFile(){
    if(currentSimFile < totalNumFiles){
        currentSimFile++;
        return filePaths[currentSimFile-1];
    }
    return "noMoreFiles";
}

void bopthook::setFilePathAttribute(){
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8012_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8017_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8133_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8164_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8254_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8599_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8807_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8929_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8954_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8964_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8983_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8988_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9149_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9152_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9200_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9479_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9490_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9736_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9796_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9992_green_4.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9999_green_4.lua");

    this->totalNumFiles = filePaths.size();
}

/**
 * This is a helper function which is made such that the TCP connection
 * can utilize a thread to continiously check on messages.
 *
 * https://stackoverflow.com/questions/1151582/pthread-function-from-a-class
 */
void * bopthook::loop()
{
    pthread_detach(pthread_self());
    tcp->receive();
    tcp->detach();
}



