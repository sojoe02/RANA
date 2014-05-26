#include <QApplication>

#include "mainwindow.h"
#include "ID.h"

int ID::aID = 0;
unsigned long long ID::eID = 0;
unsigned long long ID::tmu = 0;
unsigned long long ID::nID = 0;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
