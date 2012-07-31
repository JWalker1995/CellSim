#include "atom.h"
#include "globals.h"

#include <QRectF>
#include <QPainter>
#include <QString>
#include <QFont>
#include <QGraphicsScene>

#include <QDebug>

Atom::Atom() : QGraphicsItem()
{
    nx = 0;
    ny = 0;

    // Set random starting velocity
    vx = (float)rand()/(float)RAND_MAX*2-1;
    vy = (float)rand()/(float)RAND_MAX*2-1;

    int i = 0;
    while (i < 18) {reaction[i++] = 0;}
    i = 0;
    while (i < 6) {bondsLt[i] = 0; bondsGt[i] = 0; i++;}
    numBondsLt = 0;
    numBondsGt = 0;

    selected = false;

    atomRect.setRect(-20,-20,40,40);

    //setFlags(QGraphicsItem::ItemIsMovable);

    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    // | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable
}

void Atom::changeType(int newType)
{
    /*
    //this = Globals::getAtom(newType);
    QPointF p = pos();
    int s = state;

    Atom* b = Globals::getAtom(newType);

    memcpy(this, b, sizeof(Atom*));
    delete b;

    setPos(p);
    state = s;

    qDebug() << this->symbol;
    update();
    */
}

void Atom::changeState(int newState)
{
    state = newState;
    update();
}

char Atom::bondLtI(Atom *a)
{
    char i = 0;
    while (i < numBondsLt)
    {
        if (bondsLt[i] == a) {return i;}
        i++;
    }
    return -1;
}
char Atom::bondGtI(Atom *a)
{
    char i = 0;
    while (i < numBondsGt)
    {
        if (bondsGt[i] == a) {return i;}
        i++;
    }
    return -1;
}

void Atom::removeBondLt(Atom *a)
{
    removeBondLt(bondLtI(a));
}
void Atom::removeBondGt(Atom *a)
{
    removeBondGt(bondGtI(a));
}

void Atom::removeBondLt(int i)
{
    while (++i < numBondsLt)
    {
        bondsLt[i - 1] = bondsLt[i];
    }
    bondsLt[i - 1] = 0;
    numBondsLt--;
}
void Atom::removeBondGt(int i)
{
    while (++i < numBondsGt)
    {
        bondsGt[i - 1] = bondsGt[i];
    }
    bondsGt[i - 1] = 0;
    numBondsGt--;
}

void Atom::changeSceneSize()
{
    maxX = scene()->width()-rad;
    maxY = scene()->height()-rad;
}

bool Atom::select()
{
    if (selected){return false;}
    selected = true;
    this->update();
    return true;
}
bool Atom::deselect()
{
    if (!selected){return false;}
    selected = false;
    this->update();
    return true;
}

QRectF Atom::boundingRect() const
{
    return atomRect;
}

void Atom::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Painting should only be called when the atom is changed (type, state).
    //painter->setFont(textFont);

    QString str = symbol+QString().setNum(state);

    QFont f = painter->font();
    f.setPointSizeF(20-str.size()*2.5);
    f.setBold(selected);
    painter->setFont(f);

    painter->drawEllipse(atomRect);
    painter->setPen(color);
    painter->drawText(atomRect, Qt::AlignCenter,str);
}
void Atom::advance(int phase)
{
    if (phase == 0)
    {
        QPointF p = pos();
        qreal px = p.x()+vx;
        qreal py = p.y()+vy;
        if (px<rad)
        {
            px = rad*2-px;
            vx = -vx;
        }
        else if (px>maxX)
        {
            px = maxX*2-px;
            vx = -vx;
        }
        if (py<rad)
        {
            py = rad*2-py;
            vy = -vy;
        }

        else if (py>maxY)
        {
            py = maxY*2-py;
            vy = -vy;
        }
        this->nx = px;
        this->ny = py;
        this->setPos(px,py);
    }
}
