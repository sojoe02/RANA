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
#ifndef AGENTITEM_H
#define AGENTITEM_H

#include <QPainter>
#include <QGraphicsItem>

#include "src/utility.h"

class agentItem : public QGraphicsItem {
  //Q_OBJECT
 public:
  agentItem(QString id, rgba color, double angle);

  QRectF
  boundingRect() const;

  void
  paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

  void
  setColor(rgba color);

  void
  showID(bool show);

  void
  showAngle(bool);

  void
  setAngle(double angle);

  double
  getAngle();

 private:

  QString id;
  QColor pencolor;
  bool showid;
  double angle = 0;
  bool angleShow;

};

#endif // AGENTITEM_H
