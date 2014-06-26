#include "maphandler.h"


QImage *mapHandler::image = NULL;
MainWindow *mapHandler::parent = NULL;

mapHandler::mapHandler(MainWindow *parent)
{
    parent = parent;
}

void mapHandler::setImage(QImage *argImage)
{
    image = argImage;

}

rgba* mapHandler::getPixelInfo(int argX, int argY)
{
    if (image !=NULL)
    {
        QRgb info = image->pixel(argX, argY);
        rgba* values = new rgba;
        values->red = qRed(info);
        values->green = qGreen(info);
        values->blue = qBlue(info);
        values->alpha = 0;
        //beware of potentional memory leak!
        return values;

    } else
        return NULL;
}

void mapHandler::setPixelInfo(int argX, int argY, rgba argValue)
{
    if (image != NULL && image->width() >= argX && image->height() >= argY)
    {
        QRgb value;
        value = qRgb(argValue.red, argValue.green, argValue.red);
        image->setPixel(argX, argY, value);
        //parent->updateMap();
    }
}



