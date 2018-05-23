#ifndef BOPTHOOK_H
#define BOPTHOOK_H

#include <bayesopt/include/bopt_state.hpp>
#include <bayesopt/include/bayesopt/bayesopt.hpp>
#include "src/cli.h"
#include "src/api/tcpserver.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class Cli;
class BOptState;
class bopthook: public QObject, public bayesopt::ContinuousModel
{
    Q_OBJECT
public:
    bopthook(Cli *_cli, size_t dim, bayesopt::Parameters _param, int _portnumber);

    double evaluateSample( const vectord &Xi );

    bool checkReachability( const vectord &query ){ return true; }

    void resetFilePathList();
    std::string getNextFile();

    void sentParametersViaTcp(vectord rowOfParam);
    double messageSimDoneAndGetError(std::string);
    void sentMessageViaTcp(std::string);
    void saveCurrentState();

private:
    void setFilePathAttribute();

    std::vector<std::string> filePaths;
    size_t currentSimFile = 0;
    size_t totalNumFiles;

    Cli *cli = nullptr;

    bayesopt::BOptState state;

    tcpserver *tcp = nullptr;
    void tcpWaitForDoneMessage();
    void *loop();

    static void *bopthook_helper(void *context)
    {
        return ((bopthook *)context)->loop();
    }
};

#endif // BOPTHOOK_H
