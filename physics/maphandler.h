#ifndef MAPHANDLER_H
#define MAPHANDLER_H

#include <QtGui>

class mapHandler
{
public:
    mapHandler();
    static void setImage(QImage *argImage);
    static void getPixelInfo(int argX, int argY, int *argValue);
    static void setPixelInfo(int argX, int argY, int *argValue);

private:
    static QImage *image;
};

#endif // MAPHANDLER_H
