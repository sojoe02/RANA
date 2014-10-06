#ifndef COLORUTILITY_H
#define COLORUTILITY_H

#include <QMutexLocker>
#include <QImage>

class ColorUtility
{
public:
	ColorUtility();

	struct zvalue{
		double cumulative;
		double frequency;
		double average;
		double highest;
	};

	static void AddMaxMinValues(zvalue min, zvalue max, int maxTime);

	static QRgb ZValueToColor(double value, double min, double max);

	static QRgb GetCumulativeColor(double value);
	static QRgb GetFreqColor(double value);
	static QRgb GetAvgColor(double value);
	static QRgb GetHighest(double value);

	static double Z_TRESSHOLD_PERCENTAGE;
	static int GetMaxTime(){return MAXTIME;}

	static void Init();

private:

	static QMutex MUTEX;
	static zvalue MAX_Z_LEVELS;
	static zvalue MIN_Z_LEVELS;
	static int MAXTIME;
};

#endif // COLORUTILITY_H
