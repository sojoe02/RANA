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
#include "agentItem.h"


agentItem::agentItem(QString id, rgba color) :
    id(id), pencolor(Qt::white), showid(true)
{
}

QRectF agentItem::boundingRect() const
{
    //Still needs to be implemented properly so it really depicts size of
    // graphical representation of the agent.
    //qreal penWidth = 1;
    return QRectF(0,0,10,10);
}

void agentItem::paint(QPainter *painter,
                      const QStyleOptionGraphicsItem *option,
                      QWidget *widget)
{
    QRectF rect = boundingRect();
	QPen pen (pencolor, 1);
    painter->setPen(pen);
    painter->drawPoint(0,0);
    painter->drawPoint(1,1);
    painter->drawPoint(-1,-1);
    painter->drawPoint(1,-1);
    painter->drawPoint(-1,1);
    //painter->drawRect(rect);
    if(showid)
	{
		painter->setFont(QFont("Arial", 4));
        painter->drawText(rect, Qt::AlignCenter, id);
	}
}

void agentItem::setColor(rgba color)
{
	pencolor = QColor(color.red, color.green, color.blue, color.alpha);
	this->update();
}

void agentItem::showID(bool show)
{
	this->showid = show;
}


