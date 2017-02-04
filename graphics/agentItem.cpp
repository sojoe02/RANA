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


static const QPointF points[3] = {
	QPointF(0, 1.25),
	QPointF(0, -1.25),
	QPointF(4, 0),
};


agentItem::agentItem(QString id, rgba color, double angle) :
	id(id), pencolor(Qt::white), showid(true), angle(angle), angleShow(true)
{
	pencolor = QColor(color.red, color.green, color.blue, color.alpha);

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

	QPen pen (pencolor, 1);
	painter->setPen(pen);
//	painter->fillPath();

	//painter->drawRect(rect);
	if(showid)
	{
		QRectF rect = boundingRect();
		painter->setFont(QFont("Arial", 4));
		painter->drawText(rect, Qt::AlignCenter, id);
	}

	if(angleShow)
	{
//		painter->drawLine(0,0,5,0);
		painter->drawPolygon(points,3,Qt::OddEvenFill);

	}
	else
	{

		painter->drawPoint(0,0);
		painter->drawPoint(1,1);
		painter->drawPoint(-1,-1);
		painter->drawPoint(1,-1);
		painter->drawPoint(-1,1);
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

void agentItem::setAngle(double angle)
{
	this->angle = angle;
	this->setRotation(angle);

}

double agentItem::getAngle()
{
	return angle;
}

void agentItem::showAngle(bool angleShow)
{
	this->angleShow = angleShow;

}


