#include "agentItem.h"


agentItem::agentItem(QString id) :
    id(id)
{
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
    QRectF rect = boundingRect();
    QPen pen (Qt::black, 3);
    painter->setPen(pen);
    painter->drawPoint(0,0);
    //painter->drawRect(rect);
    painter->setFont(QFont("Arial", 6));
    painter->drawText(rect, Qt::AlignCenter, id);
}

