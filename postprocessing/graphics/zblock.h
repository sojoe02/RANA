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
	ZBlock(int x, int y);
	~ZBlock();
	void addZValue(double zvalue, int time);
	void registerMinMax();	

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	void setColor(int time, ZMode zmode);
	void changeMode(ZMode zmode);
private:

	QRgb getCumulativeColor(int time);
	QRgb getFrequencyColor(int time);
	QRgb getHighestColor(int time);
	QRgb getAverageColor(int time);

	ColorUtility::zvalue current;
	ColorUtility::zvalue max;
	ColorUtility::zvalue min;

	int x;
	int y;

	QRgb currentFrequencyColor;
	QRgb currentCumulativeColor;
	QRgb currentAverageColor;
	QRgb currentHighestColor;
	QRgb activeColor;

	bool firstAddition;
	std::map<int, ColorUtility::zvalue> zmap;
	std::map<int, ColorUtility::zvalue>::iterator zitr;

	ZMode zmode;
};

#endif // ZBLOCK_H
