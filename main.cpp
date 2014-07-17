#include <QApplication>
#include <stdio.h>

#include "mainwindow.h"
#include "ID.h"
#include "physics/phys.h"
#include "output.h"


int ID::aID = 0;
unsigned long long ID::eID = 0;
unsigned long long ID::tmu = 0;
unsigned long long ID::nID = 0;


int main(int argc, char *argv[])
{
    Phys::seedMersenne();
    Output::DelayValue = 0;

    QApplication a(argc, argv);
    MainWindow *w = new MainWindow();

    Output::Inst()->setMainWindow(w);

    w->show();

    return a.exec();
}
