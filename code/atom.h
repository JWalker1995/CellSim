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

    void changeState(int newState);
    int bondLtI(Atom* a);
    int bondGtI(Atom* a);
    void removeBondLt(Atom* a);
    void removeBondGt(Atom* a);
    void removeBondLt(int i);
    void removeBondGt(int i);
    void removeBonds();

    void neighbor(Atom* a);

    virtual void collide(Atom* a) {};
    virtual void serialize() {};

    void changeSceneSize();

    bool select();
    bool deselect();
    bool selected;

    int i;

    Atom** bin;// Pointer to a bin, which is of type Atom*

    qreal nx;
    qreal ny;

    qreal vx;
    qreal vy;
    qreal rad;
    qreal mass;

    int element;
    QString symbol;
    unsigned short state;
    int reaction[18];
    QString reactionStr;

    Atom* bondsLt[6];
    int numBondsLt;
    Atom* bondsGt[6];
    int numBondsGt;

private:
    qreal maxX;
    qreal maxY;

    void runReaction(Atom *a, bool ltA);
    bool testEqu(int* arr, int i, int num);
    int evalEqu(int* arr, int i);

    qreal getT(Atom *a, qreal relX, qreal relY, qreal dsq);
    void bounce(Atom *a, qreal t);

protected:
    int color;
    QRectF atomRect;

signals:
    
public slots:
    
};

#endif // ATOM_H
