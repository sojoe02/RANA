#ifndef ZBLOCK_H
#define ZBLOCK_H

#include <map>
#include <QRgb>
#include <QPainter>
#include <QGraphicsItem>

#include "../colorutility.h"

enum class ZMode
{
	Average,
	Frequency,
	Cumulative,
	Highest
};

class ZBlock : public QGraphicsItem
{
public:
	ZBlock(int posX, int argY);
	~ZBlock();
	void addZValue(double zvalue, int time);
	void registerMinMax();	

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

	void setColor(int time, ZMode currentZMode);
	void changeMode(ZMode currentZMode);
	void setTime(int time);

private:

	QRgb getCumulativeColor(int time);
	QRgb getFrequencyColor(int time);
	QRgb getHighestColor(int time);
	QRgb getAverageColor(int time);

	ColorUtility::zvalue current;
	ColorUtility::zvalue max;
	ColorUtility::zvalue min;

	int posX;
	int posY;

	QRgb activeColor;

	bool firstAddition;
	std::map<int, ColorUtility::zvalue> zmap;
	std::map<int, ColorUtility::zvalue>::iterator zitr;

	ZMode currentZMode;
	int currentTime;
};

#endif // ZBLOCK_H
