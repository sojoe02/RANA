#include "maphandler.h"


QImage *mapHandler::image = NULL;

mapHandler::mapHandler()
{
}

void mapHandler::setImage(QImage *argImage)
{
    image = argImage;

}

void mapHandler::getPixelInfo(int argX, int argY, int *argValue)
{
    if (image !=NULL)
    {
        QRgb info = image->pixel(argX, argY);
        argValue[0] = qRed(info);
        argValue[1] = qGreen(info);
        argValue[2] = qBlue(info);
        argValue[3] = qAlpha(info);
    }
}

