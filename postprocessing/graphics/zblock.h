#ifndef ZBLOCK_H
#define ZBLOCK_H

#include <map>
#include <QRgb>

#include "../colorutility.h"


class ZBlock
{
public:
	ZBlock(int x, int y);

	void addZValue(double zvalue, int time);

	void registerMinMax();
	QRgb getCumulativeColor(int time);
	QRgb getFrequencyColor(int time);
	QRgb getHighestColor(int time);
	QRgb getAverageColor(int time);

private:

	ColorUtility::zvalue current;
	ColorUtility::zvalue max;
	ColorUtility::zvalue min;

	int x;
	int y;

	QRgb currentFrequencyColor;
	QRgb currentCumulativeColor;
	QRgb currentAverageColor;
	QRgb currentHighestColor;

	bool firstAddition;
	std::map<int, ColorUtility::zvalue> zmap;
	std::map<int, ColorUtility::zvalue>::iterator zitr;
};

#endif // ZBLOCK_H
