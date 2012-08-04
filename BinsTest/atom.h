#ifndef ATOM_H
#define ATOM_H

#include "bins.h"

#include <QGraphicsItem>
#include <QRectF>
#include <QVector2D>
#include <QString>

class Atom : public QGraphicsItem
{
public:
    explicit Atom();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void advance(int phase);

    Bin* bin;

    qreal nx;
    qreal ny;

private:
    QRectF atomRect;

signals:
    
public slots:
    
};

#endif // ATOM_H
