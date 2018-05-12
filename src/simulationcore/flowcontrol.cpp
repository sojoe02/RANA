//--begin_license--
//
//Copyright 	2013 	Søren Vissing Jørgensen.
//			2014	Søren Vissing Jørgensen, Center for Bio-Robotics, SDU, MMMI.
//
//This file is part of RANA.
//
//RANA is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//RANA is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with RANA.  If not, see <http://www.gnu.org/licenses/>.
//
//--end_license--

#include "flowcontrol.h"

#include <thread>  //REMOVE

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::steady_clock;

#include <time.h>
#include <ctime>
#include <fstream>

FlowControl::FlowControl(Control *control)
    :control(control), mapGenerated(false),masteragent(new Supervisor()), stop(false), fetchPositions(false),
      agentAmount(0),luaFilename(""),storePositions(true),
      positionFilename("_positionData.pos")
{
    //Phys::seedMersenne();
    //file = std::ofstream(positionFilename.c_str(),std::ofstream::out| std::ofstream::trunc);
    //file.open(positionFilename.c_str(),std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);

    Output::Inst()->kprintf("Initiating simulation #%i",control->newSimulation());


    parser *p = parser::getInstance();
    if(p->enableTcpConnection()){
        enableTcpFlag = true;
        std::cout << "Setting up TCP connection" << std::endl;
        tcp = new tcpserver;
        tcp->setup(p->getPort());

        pthread_t t1;
        pthread_detach(pthread_self());
        pthread_create(&t1, NULL, &FlowControl::FlowControl_helper, this);
    }

}

FlowControl::~FlowControl()
{
    //ID::resetSystem();
    Phys::setCTime(0);
    delete masteragent;
}
/**
 * Checks if an environment has been generated.
 * @return bool false if there is not environment, true if there is.
 * */
bool FlowControl::checkEnvPresence()
{
    return mapGenerated;
}

/**
 * Generates the environment.
 * Upon environment generation the sectors will be placed, autons will be
 * assigned to a sector and placed within it's parameters.
 */
void FlowControl::generateEnvironment(double width, double height, int threads,
                                      int agentAmount, double timeResolution, int macroFactor, std::string filename)
{
    //srand(time(0));
    //Phys::seedMersenne();
    this->timeResolution = timeResolution;
    this->macroFactor = macroFactor;

    macroResolution = macroFactor * timeResolution;

    Output::KillSimulation = false;

    ID::resetSystem();
    Phys::setTimeRes(timeResolution);
    Phys::setCTime(0);
    Phys::setMacroFactor(macroFactor);
    Phys::setEnvironment(width, height);
    //Shared::initShared();
    Interfacer::initInterfacer(masteragent);
    //Scanning::edgeMask();

    masteragent->generateMap(width,height,threads,timeResolution, macroResolution);
    mapWidth = width;
    mapHeight = height;
    agentAmount = agentAmount;
    luaFilename = filename;
    masteragent->populateSystem(0, 0, agentAmount, filename); //TODO: Check if this is fine. So far no problem
    retrievePopPos();
    mapGenerated = true;
}

void FlowControl::populateSystem()
{
    //srand(time(0));
    //Phys::seedMersenne();
    masteragent->populateSystem(0, 0, agentAmount, luaFilename);
    retrievePopPos();
    mapGenerated = true;
    //Output::Inst()->enableRunBotton(true);
}

/**
 * Retrieval of auton positions.
 * Will write the positions of all autons to the std::lists given as arguments.
 * @see Master::retrievePopPos()
 * @see Control::refreshPopPos()
 */
void FlowControl::retrievePopPos()
{

    std::list<agentInfo> agentPositions = masteragent->retrievePopPos();

    control->refreshPopPos(agentPositions);

    if(Output::RunSimulation.load())
    {
        if(storePositions == true )
        {

            //std::ofstream file(positionFilename.c_str(),std::ofstream::out | std::ofstream::app);
            for(const auto &apos : agentPositions)
            {

                agentTmu agenttmu;
                agenttmu.x = apos.x;
                agenttmu.y = apos.y;
                agenttmu.id = apos.id;
                agenttmu.tmu = cMacroStep;
                //Output::Inst()->kprintf("id %d, posx %d, posY %d",agenttmu.info.id, agenttmu.info.x, agenttmu.info.y);

                file.write(reinterpret_cast<char*>(&agenttmu), sizeof(agentTmu));
            }
            //file.close();
        }
    }
}

void FlowControl::toggleLiveView(bool enable)
{
    fetchPositions.store(enable);
}

/**
 * Runs the simulation.
 * Start a simulation run, this will run a simulation, width the defined macro and micro
 * precision, the simulation can be cancelled via the atomic boolean stop. The run will
 * update the progress bar and status window in the running panel.
 * @param time the amount of seconds the simulation will simulate.
 */
void FlowControl::runSimulation(int _time)
{
    std::string positionFilePath = Output::Inst()->RanaDir;
    positionFilePath.append("/");
    positionFilePath.append(positionFilename.c_str());
    if(remove(positionFilePath.c_str()) != 0)
    {
        Output::Inst()->kprintf("Position file does not exist");
    }

    file.open(positionFilePath.c_str(),std::ofstream::out | std::ofstream::app | std::ofstream::binary);
    stop = false;
    Output::Inst()->kprintf("Running Simulation of: %i[s], with resolution of %f",_time, timeResolution);
    Output::RunSimulation = true;
    unsigned long long iterations = (double)_time/timeResolution;

    auto start = steady_clock::now();
    auto start2 = steady_clock::now();
    auto start3 = steady_clock::now();
    auto end = steady_clock::now();
    //unsigned long long run_time = 0;
    cMacroStep = 0;
    cMicroStep = ULLONG_MAX;

    unsigned long long i = 0;//, j = 0;

/*
    double previousVal = -1;
    std::ofstream myfile;
    myfile.open ("/home/theis/test23", std::ios::app);
    int gggasdf = 30;
    myfile << -1 << "\n";
*/
    for(i = 0; i < iterations;)
    {
        if(Output::KillSimulation.load() == true)
        {
            return;
        }

        Phys::setCTime(i);


        if(i == cMicroStep && cMicroStep != ULLONG_MAX)
        {
            masteragent->microStep(i);
            //Output::Inst()->kprintf("i is now %lld", i);
        }
        if(i == cMacroStep)
        {
            masteragent->macroStep(i);
            cMacroStep += macroFactor;
            int delay = Output::DelayValue.load();
            if(delay != 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            }
            if(cMacroStep % (int)Phys::getMacroFactor() == 0)
            {
                retrievePopPos();
            }
        }

        i = cMacroStep;
        cMicroStep = masteragent->getNextMicroTmu();

        if( i > cMicroStep)
        {
            i = cMicroStep;
        }

        //		//Update the status and progress bar screens:
        end = steady_clock::now();

        if(duration_cast<milliseconds>(end-start).count() > 100)
        {

            masteragent->printStatus();

            Output::Inst()->progressBar(cMacroStep,iterations);

            //int delay = Output::DelayValue.load();
            //std::this_thread::sleep_for(std::chrono::milliseconds(5));
            //if(delay != 0)
            //fetchPositions.store(true);
            //if(fetchPositions.load())
            //{
            //   retrievePopPos();
            //}

            start = end;

        }

        if(stop.load() == true || Output::RunSimulation==false)
        {
            Output::Inst()->kprintf("Stopping simulator at microstep %llu \n", i);
            break;
        }
/*
        if((i/1000)%gggasdf == 0){
            if( Shared::getNumber("hello") != -1 && Shared::getNumber("hello") != previousVal){
                previousVal = Shared::getNumber("hello");
                double timeDelayDiff = i/double(1000000)-previousVal;
                myfile << timeDelayDiff << "\n";
                //std::cout << i/double(1000000) << "\t" << previousVal << "\t" << i/double(1000000)-previousVal << std::endl;
            }
        }
*/
        if(this->enableTcpFlag)    //Only wrong if TCP server is used
        {
            tcpWaitForDoneMessage();
        }
    }
    /*Used for timing how long simulations take.*/
/*
    auto end3 = steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end3 - start3);
    std::cout << "Simulation run took:\t" << elapsed.count()/double(1000000) << std::endl;
    myfile << elapsed.count()/double(1000000) << "\n";
    myfile.close();
*/
    /*
    std::ofstream myfile;
    myfile.open ("/home/theis/test9", std::ios::app);
    myfile << elapsed.count()/double(1000000) << "\n";
    myfile.close();
    */

    retrievePopPos();
    masteragent->simDone();
    masteragent->printStatus();
    Output::Inst()->progressBar(i,iterations);

    Output::RUNTIME = i;

    auto endsim = steady_clock::now();
    duration_cast<seconds>(start2-endsim).count();
    Output::Inst()->kprintf("Simulation run took:\t %llu[s] of computing time", duration_cast<seconds>(endsim - start2).count());
    file.close();
}

void FlowControl::tcpWaitForDoneMessage()
{
    bool tmp_flag = true;
    while(tmp_flag)
    {
        std::string str = tcp->getMessage();
        if (str == "done\n"){
            //std::cout << "Server got: " << str << std::endl;
            tcp->Send("Msg-01\n");
            tcp->clean();
            tmp_flag = false;
        }
        else if( str != "" ){
            //std::cout << "Server got: " << str << std::endl;
            std::stringstream ss(str);
            std::vector<std::string> result;

            while( ss.good() )
            {
                std::string substr;
                std::getline( ss, substr, ',' );
                result.push_back( substr );
            }

            std::string tcpInputAgentName = result.front();   //This is the name of the TCP input agent.
            result.erase (result.begin());
            Shared::addTcpInputToAgent(tcpInputAgentName, result);
            tcp->Send("Msg-02\n");
            tcp->clean();
            tmp_flag = false;
        }
        usleep(100);
    }
}

/**
 * This is a helper function which is made such that the TCP connection
 * can utilize a thread to continiously check on messages.
 *
 * https://stackoverflow.com/questions/1151582/pthread-function-from-a-class
 */
void * FlowControl::loop()
{
    pthread_detach(pthread_self());
    tcp->receive();
    tcp->detach();
}

/**
 * Stop currently running simulation
 * Stops the active simulation run via setting an atomic boolean.
 */
void FlowControl::stopSimulation()
{
    stop.store(true);
}

/**
 * Save eEvent data to disk
 * @see EventQueue::saveEEventData
 */
void FlowControl::saveExternalEvents(std::string filename)
{
    masteragent->saveExternalEvents(filename);
}
