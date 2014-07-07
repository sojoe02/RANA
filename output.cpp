#include <cstring>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>

#include "output.h"

Output* Output::output;
MainWindow* Output::mainWindow;

Output* Output::Inst()
{
    if(!output)
        output = new Output();

    return output;
}


Output::Output()
{

}

void Output::kprintf(const char* msg, ...)
{
    va_list args;
    va_start(args, msg);

    char buffer[256];
    sprintf(buffer, msg, args);

    mainWindow->write_output(buffer);

    va_end(args);
}

void Output::progressBar(unsigned long long current, unsigned long long maximum)
{
    int progress = (current * 100)/maximum;
    mainWindow->advanceProgess(progress);
}

void Output::setMainWindow(MainWindow *mainwindow)
{
    Output::mainWindow = mainwindow;
}
