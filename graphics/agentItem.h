#ifndef AGENTITEM_H
#define AGENTITEM_H

#include <QPainter>
#include <QGraphicsItem>


class agentItem : public QGraphicsItem
{
    //Q_OBJECT
public:
    agentItem(QString id);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


private:

    QString id;


};

#endif // AGENTITEM_H
