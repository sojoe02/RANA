#ifndef MAPHANDLER_H
#define MAPHANDLER_H

#include <QtGui>

#include "utility.h"
#include "mainwindow.h"

class MainWindow;
class mapHandler
{
public:
    mapHandler(MainWindow *parent);
    static void setImage(QImage *argImage);
    static rgba *getPixelInfo(int argX, int argY);
    static void setPixelInfo(int argX, int argY, rgba argValue);

private:
    static QImage *image;
    static MainWindow* parent;
};

#endif // MAPHANDLER_H
