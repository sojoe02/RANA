#include "zmap.h"

//enum class ZMode
//{
//	Average,
//	Frequency,
//	Cumulative,
//	Highest
//};

ZMap::ZMap()
{
}
QRectF ZMap::boundingRect() const
{
 return QRectF(0,0,1,1);
}
void ZMap::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}
