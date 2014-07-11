#ifndef MAPHANDLER_H
#define MAPHANDLER_H

#include <QtGui>

#include "utility.h"
#include "mainwindow.h"

#include <map>
#include <vector>

typedef std::vector< std::vector<int> > MATRICE;

class MainWindow;
class MapHandler
{
public:

    MapHandler(MainWindow *parent);
    static void setImage(QImage *argImage);
    static rgba getPixelInfo(int argX, int argY);
    static void setPixelInfo(int argX, int argY, rgba argValue);

    static MATRICE radialScan(int radius, char channel);

private:
    static QImage *image;
    static MainWindow* parent;
    static std::map<int, MATRICE> radialMasks;
};

#endif // MAPHANDLER_H
