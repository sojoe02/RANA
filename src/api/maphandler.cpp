//--begin_license--
//
//Copyright 	2013 	Søren Vissing Jørgensen.
//			2014	Søren Vissing Jørgensen, Center for Bio-Robotics, SDU, MMMI.  
//
//This file is part of RANA.
//
//RANA is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//RANA is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with RANA.  If not, see <http://www.gnu.org/licenses/>.
//
//--end_license--
#include <math.h>
#include <climits>

#include "src/output.h"
#include "src/api/phys.h"
#include "src/api/maphandler.h"

QImage *MapHandler::image = NULL;
MainWindow *MapHandler::parent = NULL;
std::unordered_map<int, MatriceInt> MapHandler::radialMasks;
std::shared_timed_mutex MapHandler::mapMutex;

MapHandler::MapHandler(MainWindow *parent)
{
  parent = parent;
}

void
MapHandler::setImage(QImage *argImage)
{
  MapHandler::image = argImage;
}

rgba
MapHandler::getPixelInfo(int argX, int argY)
{
  std::shared_lock<std::shared_timed_mutex> readerLock(mapMutex);

  rgba values;

  if(image != NULL && image->width() > argX && image->height() > argY
	 && argX >= 0 && argY >= 0)
	{

	  QRgb info = image->pixel(argX, argY);

	  values.red = qRed(info);
	  values.green = qGreen(info);
	  values.blue = qBlue(info);
	  values.alpha = 0;

	}
  else
	{
	  values.red = 256;
	  values.green = 256;
	  values.blue = 256;
	  values.alpha = 256;
	}

  return values;
}

bool
MapHandler::checkAndChange(int argX, int argY, rgba check_color, rgba change_color)
{

  if(image != NULL && image->width() > argX && image->height() > argY
	 && argX >= 0 && argY >= 0)
	{
	  std::lock_guard<std::shared_timed_mutex>
		  writerlock(mapMutex);

	  QRgb info = image->pixel(argX, argY);

	  if(check_color.red == qRed(info) && check_color.green == qGreen(info)
		 && check_color.blue == qBlue(info))
		{
		  QRgb value = qRgb(change_color.red, change_color.green, change_color.blue);
		  image->setPixel(argX, argY, value);
		  return true;
		}

	}

  return false;
}

bool
MapHandler::setPixelInfo(int argX, int argY, rgba argValue)
{

  if(image != NULL && image->width() > argX && image->height() > argY
	 && argX >= 0 && argY >= 0)
	{

	  std::lock_guard<std::shared_timed_mutex>
		  writerLock(mapMutex);

	  QRgb value;
	  //Output::Inst()->kprintf("%i, %i, %i",argValue.red, argValue.green, argValue.blue);
	  int red = argValue.red;
	  int green = argValue.green;
	  int blue = argValue.blue;

	  value = qRgb(red, green, blue);
	  image->setPixel(argX, argY, value);

	  return true;
	}
  else
	return false;
}

/*MatriceInt MapHandler::drawCircle( int radius, char channel, int posX, int posY)
{
    int range = radius*2+1;
    std::vector< std::vector<int> > tmpvec(radius, std::vector<int>(radius) );
    MatriceInt mask = tmpvec;
    std::vector< std::vector<int> > tmpvec1(range, std::vector<int>(range) );
    MatriceInt result = tmpvec1;

    //if(image == NULL)
    //  return result;

    if(MapHandler::radialMasks.find(radius) == MapHandler::radialMasks.end())
    {
        //calculate new mask:
        //this can be optimized, since the scan is radial
        double angle = 0;
        double dx = 0;
        double dy = 0;

        for(int x = 0; x < radius; x++)
        {
            for(int y = 0; y < radius; y++)
            {

                double distance = sqrt( pow((double)x, 2) + pow((double)y,2));
                Output::Inst()->kprintf("%f,d", distance, radius);
                //if the distance is smaller than the actual radius.
                if (distance + 0.5  < radius)
                {
                    result[x][y] = 1;
                    rgba color;
                    color.blue = 255;

                    setPixelInfo(posX+x, posY+y, color);
                    setPixelInfo(posX-x, posY+y, color);
                    setPixelInfo(posX-x, posY-y, color);
                    setPixelInfo(posX+x, posY-y, color);
                } else result[x][y] = -1;

            }
        }
        MapHandler::radialMasks[radius] = result;
    }
    mask = MapHandler::radialMasks[radius];
    //perform the actual scan:

    for(int x = 0 ; x < radius ; x++)
    {
        for(int y = 0 ; y < radius ; y++)
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

            Output::Inst()->kprintf("x,y : %i,%i, value : %i", x, y, mask[x][y]);
        }
    }
    return mask;
}

*/

