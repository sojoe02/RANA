#ifndef COLORUTILITY_H
#define COLORUTILITY_H

#include <QMutexLocker>
#include <QImage>

class ColorUtility
{
public:
    ColorUtility();

    struct zvalue
    {
        double cumulative;
        double frequency;
        double average;
        double highest;
    };

    static void AddMaxMinValues(zvalue min, zvalue max, int maxTime);

    static QColor ZValueToColor(double value, double min, double max);

    static QColor GetCumulativeColor(double value);

    static QColor GetFreqColor(double value);

    static QColor GetAvgColor(double value);

    static QColor GetHighest(double value);

    static zvalue GetMaxZLevels()
    { return MAX_Z_LEVELS; }

    static zvalue GetMinZLevels()
    { return MIN_Z_LEVELS; }

    static double Z_TRESSHOLD_PERCENTAGE;

    static int GetMaxTime()
    { return MAXTIME; }

    static void Init();

private:

    static QMutex MUTEX;
    static zvalue MAX_Z_LEVELS;
    static zvalue MIN_Z_LEVELS;
    static int MAXTIME;
};

#endif // COLORUTILITY_H
