#ifndef ZBLOCK_H
#define ZBLOCK_H

#include <map>
#include <QRgb>
#include <QPainter>
#include <QGraphicsItem>

#include "src/postprocessing/colorutility.h"

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

	QColor getCumulativeColor(int time);
	QColor getFrequencyColor(int time);
	QColor getHighestColor(int time);
	QColor getAverageColor(int time);

	ColorUtility::zvalue current;
	ColorUtility::zvalue max;
	ColorUtility::zvalue min;

	int posX;
	int posY;

	QColor activeColor;
	QColor defaultColor;

	bool firstAddition;
	std::map<int, ColorUtility::zvalue> zmap;
	std::map<int, ColorUtility::zvalue>::iterator zitr;

	ZMode currentZMode;
	int currentTime;
	int maxTime;
};

#endif // ZBLOCK_H
