#ifndef ZBLOCK_H
#define ZBLOCK_H

#include <QPainter>
#include <QGraphicsItem>
#include <map>

class ZBlock : public QGraphicsItem
{
public:
	ZBlock(int x, int y);

	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	void addZValue(double zvalue, unsigned long long time);

private:

	struct zvalue{
		double cumulative;
		double frequency;
		double average;
		double highest;
	};

	double max_cumulative;
	double max_frequency;
	double max_average;
	double max_highest;

	double min_cumulative;
	double min_frequency;
	double min_average;
	double min_highest;

	bool firstAddition;

	int x, y, size;

	std::map<unsigned long long, ZBlock::zvalue> zmap;

};

#endif // ZBLOCK_H
