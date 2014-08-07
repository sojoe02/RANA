#ifndef OUTPUT_H
#define OUTPUT_H

#include <QtGui>

#include <atomic>
#include "mainwindow.h"



class MainWindow;
class Output
{
public:
    static Output* Inst();

    void kprintf(const char* msg, ...);
    void kerr(const char* msg, ...);

    void updateStatus(unsigned long long ms, unsigned long long eventInit,
                      unsigned long long internalEvents, unsigned long long externalEvents);

    void clearProgressBar(){

    }

    void progressBar(unsigned long long current, unsigned long long maximum);

    void updatePosition(int id, double x, double y);

    void setMainWindow(MainWindow *mainwindow);

    static std::atomic<int> DelayValue;

    static std::atomic<bool> RunSimulation;
    static std::atomic<bool> KillSimulation;

private:
    Output();
    static Output* output;
    static MainWindow* mainWindow;
    static std::mutex lock;
};

#endif // OUTPUT_H
