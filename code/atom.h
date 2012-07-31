#ifndef ATOM_H
#define ATOM_H

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

    void changeType(int newType);
    void changeState(int newState);
    char bondLtI(Atom* a);
    char bondGtI(Atom* a);
    void removeBondLt(Atom* a);
    void removeBondGt(Atom* a);
    void removeBondLt(int i);
    void removeBondGt(int i);

    virtual void collide(Atom* a) {};

    void changeSceneSize();

    bool select();
    bool deselect();
    bool selected;

    unsigned int i;

    qreal nx;
    qreal ny;

    qreal vx;
    qreal vy;
    qreal rad;
    qreal mass;

    unsigned char element;
    QString symbol;
    unsigned short state;
    int reaction[18];
    QString reactionStr;

    Atom* bondsLt[6];
    char numBondsLt;
    Atom* bondsGt[6];
    char numBondsGt;

private:
    QRectF atomRect;
    qreal maxX;
    qreal maxY;

protected:
    int color;

signals:
    
public slots:
    
};

#endif // ATOM_H
