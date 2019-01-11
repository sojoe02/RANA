

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <set>
#include <chrono>
#include <math.h>

#include "api/phys.h"
#include "eventprocessing.h"
#include "output.h"
#include "simulationcore/agents/agentluainterface.h"
#include "api/shared.h"
#include "api/gridmovement.h"

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::steady_clock;

EventProcessing::EventProcessing()
        : zBlocks(new QHash<QString, ZBlock *>()),
          simInfo(new EventQueue::simInfo), test_number(0)
{
}

EventProcessing::~EventProcessing()
{
    delete zBlocks;
    delete simInfo;
}

void EventProcessing::resetEventProcessor()
{
    zBlocks->clear();
    eventbin.clear();
}

EventQueue::simInfo *EventProcessing::readEventInfo(std::string path)
{
    std::ifstream file(path.c_str(), std::ifstream::binary);

    if (file.is_open())
    {
        file.read(reinterpret_cast<char *>
                  (simInfo), sizeof(EventQueue::simInfo));

        Output::kprintf("<b>SIMULATION INFO</b>");
        Output::kprintf(simInfo->luaFileName);
        Output::kprintf("\nHeight [m]\t:\t%f\n", simInfo->areaY);
        Output::kprintf("Width [m]\t:\t%f\n", simInfo->areaX);
        Output::kprintf("Agent Amount\t:\t%d\n", simInfo->numberOfAgents);
        Output::kprintf("Event Amount\t:\t%llu\n", simInfo->eventAmount);
        Output::kprintf("Time Resolution\t:\t%f\n", simInfo->timeResolution);
        Output::kprintf("MacroFactor\t:\t%d\n", simInfo->macroFactor);
        Output::kprintf("Simulation runtime\t:%llu\n", simInfo->tmuAmount);

        //while(!file.eof())
        //{
        //	file.read(reinterpret_cast<char*>(&devent), sizeof(EventQueue::dataEvent));
        //	eventbin.push_back(devent);
        //}
    }
    else
    {
        Output::kprintf("File not found ... or worse");
    }

    file.close();

    return simInfo;
}

EventQueue::simInfo *EventProcessing::getDataEvent()
{
    return simInfo;
}

void EventProcessing::binEvents(QRegExp regex, std::string path, int to, int from)
{
    Output::kprintf("Binning Events from %i[s] to %i[s], data path is %s", from, to, path.c_str());

    std::ifstream file(path.c_str(), std::ifstream::binary);

    eventbin.clear();

    if (file.is_open())
    {
        file.read(reinterpret_cast<char *>(simInfo), sizeof(EventQueue::simInfo));

        while (!file.eof())
        {
            file.read(reinterpret_cast<char *>(&devent), sizeof(EventQueue::dataEvent));
            //calculate the activation in seconds:
            double activation = devent.activationTime / simInfo->timeResolution;
            //Output::Inst()->kdebug("devent activation time %i" , devent.activationTime);
            //Output::Inst()->kprintf("Event description %s", devent.desc);

            if (activation >= from && activation <= to)
            {
                QString desc = devent.desc;
                //Output::Inst()->kprintf("%s,you never know",devent.desc);
                if (desc.contains(regex))
                {
                    //Output::Inst()->ppprintf("Binning event with ID %i",devent.id);
                    eventbin.push_back(devent);
                }
            }
        }
    }
    else
        Output::kprintf("File not found ... or worse");

    file.close();
}

QHash<QString, ZBlock *> *EventProcessing::getZBlocks()
{
    return zBlocks;
}

void EventProcessing::processBinnedEvents(double timeResolution, std::string path,
                                          double mapResolution, double thresshold)
{

    Output::kprintf("Processing %lu events, path is %s", eventbin.size(), path.c_str());

    int xAmount = simInfo->areaX / mapResolution;
    int yAmount = simInfo->areaY / mapResolution;

    //Output::Inst()->setEventSceneRect(xAmount, yAmount);

    Output::kprintf("x and y amount: %i,%i", xAmount, yAmount);

    //put in a timer:
    auto start = steady_clock::now();
    auto end = steady_clock::now();
    auto start2 = steady_clock::now();

    //clear the zblok map:
    //for(auto it = zBlocks->begin(); it != zBlocks->end(); ++it)
    //{
    //	it.value()->deleteLater();
    //}
    zBlocks->clear();

    //populate the zblok map:
    for (int x = 0; x <= xAmount; x++)
    {
        for (int y = 0; y <= yAmount; y++)
        {
            ZBlock *block = new ZBlock(x, y);
            char buffer[32] = {};
            sprintf(buffer, "%i,%i", x, y);
            //std::string key = buffer;
            //zBlocks->insert(std::pair<std::string, ZBlock>(key, block));
            //The Qt way:
            QString key = buffer;
            zBlocks->insert(key, block);
        }
    }

    //setup the agent environment:
    Phys::setTimeRes(timeResolution);
    Phys::setCTime(0);
    Phys::setMacroFactor(simInfo->macroFactor);
    Phys::setEnvironment(simInfo->areaX / simInfo->mapResolution,
                         simInfo->areaY / simInfo->mapResolution);
    Shared::initShared();
    GridMovement::clearGrid();
    QImage *mapImage = new QImage(simInfo->areaX / mapResolution,
                                  simInfo->areaY / mapResolution,
                                  QImage::Format_RGB32);

    QGraphicsPixmapItem *mapItem = new QGraphicsPixmapItem(QPixmap::fromImage(*mapImage));
    //scene->addItem(mapItem);
    mapItem->setPixmap(QPixmap::fromImage(*mapImage));

    //process all the binned events, new c11 style better get used to it:
    auto it = std::begin(eventbin);
    for (; it != std::end(eventbin) && Output::RunEventProcessing.load() == true; ++it)
    {
        processEvent(&*it, thresshold,
                     mapResolution, timeResolution, path);

        end = steady_clock::now();
        if (duration_cast<milliseconds>(end - start).count() > 50)
        {
            //Output::Inst()->ppprogressbar(std::distance(begin(eventbin), it), eventbin.size());
            start = end;
        }
    }
    if (!eventbin.empty())
        //Output::Inst()->ppprogressbar(std::distance(begin(eventbin), it), eventbin.size());

        for (auto it = zBlocks->begin(); it != zBlocks->end(); ++it)
    {
        it.value()->registerMinMax();
    }

    auto endsim = steady_clock::now();

    Output::kprintf("Event processing took:\t %llu[s] ",
                    duration_cast<seconds>(endsim - start2).count());

    //Output::Inst()->ppprintf("Over and out...");

    delete mapImage;
    delete mapItem;
}

void EventProcessing::processEvent(EventQueue::dataEvent *event,
                                   double thresshold, double mapRes, double timeRes, std::string arg_path)
{
    //Output::Inst()->ppprintf("doing event, id %i ", event->id);

    Phys::setMacroFactor(simInfo->macroFactor);
    Phys::setTimeRes(simInfo->timeResolution);

    //figure out whether a filename was provided:
    std::string filename = event->filename;
    std::string path = arg_path;

    if (filename.compare("NULL") != 0)
    {
        //build a new path 'clumsily':
        path = path.substr(0, path.find_last_of("\\/")) + "/" + filename;
    }

    //Output::Inst()->kdebug("path is: %s, %s", path.c_str(), filename.c_str());
    AgentLuaInterface *auton =
            new AgentLuaInterface(event->originID, event->originX, event->originY, 0, NULL, path);

    QSet <QString> *visited = new QSet<QString>();

    int width = simInfo->areaX / mapRes;
    int height = simInfo->areaY / mapRes;

    double z = 0;
    double duration = 0;
    //calculate the z value at origin, to get thresshold value:
    auton->processFunction(event, simInfo->mapResolution,
                           event->originX,
                           event->originY,
                           z, duration);

    Output::kprintf("z value at origin is = %f, duration is = %f, thresshold= %f", z, duration, thresshold);

    double thressholdZ = z * thresshold;

    recursiveZlevel(auton, event, visited, event->originX / mapRes,
                    event->originY / mapRes, 0, 0,
                    height, width, mapRes, timeRes, thressholdZ);

    delete visited;
    delete auton;
}

void EventProcessing::recursiveZlevel(AgentLuaInterface *auton, EventQueue::dataEvent *event,
                                      QSet <QString> *visited,
                                      int x, int y, int displaceX, int displaceY,
                                      int width, int height,
                                      double mapRes, double timeRes, double thressholdZ)
{
    if (Output::RunEventProcessing.load() == false)
        return;

    //Output::Inst()->ppprintf("%s,%s","8", "8");
    //char buffer[16] = {};
    int xd = x + displaceX;
    int yd = y + displaceY;

    //sprintf(buffer,"%i,%i",xd, yd);
    QString key;
    QTextStream(&key) << xd << "," << yd;
    //Output::Inst()->ppprintf("zblock key is: %s", key.toStdString().c_str());

    //have the position been accessed?
    if (visited->find(key) != visited->end())
    {
        return;
    }
    else
        visited->insert(key);

    double distance = sqrt(pow((event->originX - (x + displaceX) * mapRes), 2)
                           + pow((event->originY - (y + displaceY) * mapRes), 2));

    double arrivalTime = 0;

    if (event->propagationSpeed == 0)
    {
        arrivalTime = event->activationTime / simInfo->timeResolution / timeRes;
    }
    else
    {
        arrivalTime = (event->activationTime / simInfo->timeResolution / timeRes)
                      + distance / (event->propagationSpeed) / timeRes;
    }
    //Output::Inst()->ppprintf("arrival time: %f, x: %i, y: %i", arrivalTime,
    //                   x+displaceX, y+displaceY);

    //insert z value and take event duration into account:
    QHash<QString, ZBlock *>::iterator zitr = zBlocks->find(key);
    double z = 0;

    if (zitr == zBlocks->end())
        return;
    else
    {

        double duration = 0;
        double xArg = double(x + displaceX) * mapRes;
        double yArg = double(y + displaceY) * mapRes;

        auton->processFunction(event, mapRes, xArg, yArg, z, duration);

        if (z <= thressholdZ)
            return;

        //Output::Inst()->ppprintf("Process returns: %f, %f", z, duration);

        if (zitr != zBlocks->end())
        {
            zitr.value()->addZValue(z, arrivalTime);
            // Output::Inst()->kprintf("%f", arrivalTime);
        }

        double max = 0;

        if (duration != 0)
            max = duration / timeRes;

        //Output::Inst()->kprintf("%f", max);

        for (int i = 1; i < (int) max; i++)
        {
            if (zitr != zBlocks->end())
            {
                //auton->processFunction(event, i/timeRes,x+displaceX*mapRes,
                //				   y+displaceY*mapRes,z,duration);
                zitr.value()->addZValue(z, arrivalTime + i);
            }
        }

    }

    //Output::Inst()->ppprintf("Z value is: %f, position is: %i,%i", z ,x+displaceX,y+displaceY );

    //return if z is below thresshold:
    //if (z <= thressholdZ)
    //return;

    //do the recursive calls:
    if (x + displaceX + 1 <= width)
        recursiveZlevel(auton, event, visited, x, y, displaceX + 1, displaceY,
                        width, height, mapRes, timeRes, thressholdZ);

    if (y + displaceY + 1 <= height)
        recursiveZlevel(auton, event, visited, x, y, displaceX, displaceY + 1,
                        width, height, mapRes, timeRes, thressholdZ);

    if (x + displaceX - 1 >= 0)
        recursiveZlevel(auton, event, visited, x, y, displaceX - 1, displaceY,
                        width, height, mapRes, timeRes, thressholdZ);

    if (y + displaceY - 1 >= 0)
        recursiveZlevel(auton, event, visited, x, y, displaceX, displaceY - 1,
                        width, height, mapRes, timeRes, thressholdZ);

}
