#include "bopthook.h"

bopthook::bopthook(Cli *_cli, size_t dim, bayesopt::Parameters _param, int _portNumber):
    ContinuousModel(dim, _param)
{
    this->cli = _cli;
    this->tcp = new tcpserver;
    this->tcp->setup(_portNumber);

    this->setFilePathAttribute();

    bool tmp_flag = false;
    while(tmp_flag)
    {
        std::string str = tcp->getMessage();
        if (str == "rdy"){
            tcp->clean();
            tmp_flag = false;
        }
        usleep(100);
    }
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

    tcp->Send(msgToMatlab);
    tcp->clean();

    bool tmp_flag = false;
    while(tmp_flag)
    {
        std::string str = tcp->getMessage();
        if (str == "filesRdy"){
            tcp->clean();
            tmp_flag = false;
        }
        usleep(100);
    }
}

double bopthook::messageSimDoneAndGetError(std::string msgToMatlab){
    tcp->Send(msgToMatlab);
    tcp->clean();

    double processedError = 1.0;

    bool tmp_flag = false;
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
            std::cout << "Run simulation with - " << filePath << std::endl;
            //control = new Control(this, filePath);
            //runController();
        }else{
            std::cout << "No more simulations" << std::endl;
            resetFilePathList();
            tmpFlag = false;
        }
    }

    double processedError = messageSimDoneAndGetError("simsDone");

    return processedError;
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
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8012_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8017_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8133_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8164_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8254_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8599_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8807_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8929_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8954_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8964_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8983_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/8988_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9149_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9152_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9200_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9479_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9490_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9736_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9796_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9992_green_2.lua");
    this->filePaths.push_back("/home/theis/Dropbox/frog/v1/bopt/testFiles/9999_green_2.lua");

    this->totalNumFiles = filePaths.size();
}





