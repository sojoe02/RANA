#include <math.h>
#include <climits>

#include "maphandler.h"

QImage *MapHandler::image = NULL;
MainWindow *MapHandler::parent = NULL;
std::map<int, MATRICE > MapHandler::radialMasks;


MapHandler::MapHandler(MainWindow *parent)
{
    parent = parent;
}

void MapHandler::setImage(QImage *argImage)
{
    MapHandler::image = argImage;
}

rgba MapHandler::getPixelInfo(int argX, int argY)
{
    rgba values;
    
    if(image != NULL && image->width() >= argX && image->height() >= argY)
    {

        QRgb info = image->pixel(argX, argY);
        
        values.red = qRed(info);
        values.green = qGreen(info);
        values.blue = qBlue(info);
        values.alpha = 0;

    }else
    {
        values.red = 256;
        values.green = 256;
        values.blue = 256;
        values.alpha = 256;
    }
    
    return values;
}

bool MapHandler::setPixelInfo(int argX, int argY, rgba argValue)
{
    if (image != NULL && image->width() >= argX && image->height() >= argY)
    {
        QRgb value;
        //Output::Inst()->kprintf("%i, %i, %i",argValue.red, argValue.green, argValue.blue);
        int red = argValue.red;
        int green = argValue.green;
        int blue = argValue.blue;
        
        value = qRgb(red,green,blue);
        image->setPixel(argX, argY, value);
        return true;
    } else
        return false;
}

MATRICE MapHandler::radialScan( int radius, char channel, int posX, int posY)
{
    int range = radius*2+1;
    std::vector< std::vector<int> > tmpvec(radius, std::vector<int>(radius) );
    MATRICE mask = tmpvec;
    std::vector< std::vector<int> > tmpvec1(range, std::vector<int>(range) );
    MATRICE result = tmpvec1;
    
    if(image == NULL)
        return result;
    
    if(MapHandler::radialMasks.find(radius) == MapHandler::radialMasks.end())
    {
        //calculate new mask:
        //this can be optimized, since the scan is radial
        double angle = 0;
        double dx = 0;
        double dy = 0;
        
        for(int x = 0; x < radius; x++)
        {
            for(int y =0; y < radius; y++)
            {
                angle = atan2(x,y);
                dx = cos(angle) * (double)x;
                dy = sin(angle) * (double)y;
                
                if(abs(dx + 0.5) > radius || abs(dy + 0.5) > radius)
                {
                    
                    
                } else
                    mask[x][y] = 1;
            }
        }
        MapHandler::radialMasks[radius] = mask;
    }
    mask = MapHandler::radialMasks[radius];
    //perform the actual scan:
    
    for(int x = 0 ; x < range ; x++)
    {
        for(int y = 0 ; y < range ; y++)
        {
            if(image->width() >= x+posX && image->height() >= y+posY)
            {
                QRgb info = image->pixel(x+posX, y+posY);
                int value = mask[x][y];
                
                if(value != -1)
                {
                    if(channel == 'r')
                        result[x][y] = value * qRed(info);
                    if(channel == 'g')
                        result[x][y] = value * qGreen(info);
                    if(channel == 'b')
                        result[x][y] = value * qBlue(info);
                } else
                    result[x][y] =  -1;
            } else
                result[x][y] = -2;
        }
    }
    return result;
}



