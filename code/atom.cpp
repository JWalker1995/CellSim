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

    //setFlags(QGraphicsItem::ItemIsMovable);

    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
    // | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable
}


void Atom::neighbor(Atom *a)
{
    qreal dx = a->nx - nx;
    qreal dy = a->ny - ny;
    qreal r = rad + a->rad;
    qreal dxsq = dx * dx;
    qreal dysq = dy * dy;
    qreal dsq = dxsq + dysq;
    qreal rsq = r * r;
    if (dsq < rsq)
    {
        // Colliding
        bounce(a, getT(a, dx, dy, dsq));
        runReaction(a, false);
        a->runReaction(this, true);
        collide(a);
        a->collide(this);
    }
}

void Atom::changeState(int newState)
{
    state = newState;
    update();// Should update only state
}

int Atom::bondLtI(Atom *a)
{
    int i = 0;
    while (i < numBondsLt)
    {
        if (bondsLt[i] == a) {return i;}
        i++;
    }
    return -1;
}
int Atom::bondGtI(Atom *a)
{
    int i = 0;
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
void Atom::removeBonds()
{
    while (numBondsLt)
    {
        bondsLt[--numBondsLt]->removeBondGt(this);
    }
    while (numBondsGt)
    {
        bondsGt[--numBondsGt]->removeBondLt(this);
    }
}

void Atom::runReaction(Atom *a, bool ltA)
{
    // The items of arr							Replace Negatives with their values
    // arr[0] = this type must match			Yes
    // arr[1] = a type must match			Yes
    // arr[2] = this state comparison			No (<, >, =, !=)
    // arr[3] = a state comparison			No (<, >, =, !=)
    // arr[4] = this op1						Yes
    // arr[5] = a op1						Yes
    // arr[6] = this operand					No (+, -, *, /)
    // arr[7] = a operand					No (+, -, *, /)
    // arr[8] = this op2						Yes
    // arr[9] = a op2						Yes
    // arr[10] = atom bond must match			No (0, 1)
    // arr[11] = atom bond is set to			No (0, 1)
    // arr[12] = this state is set to op1		Yes
    // arr[13] = a state is set to op1		Yes
    // arr[14] = this state is set to operator	No (+, -, *, /)
    // arr[15] = a state is set to operator	No (+, -, *, /)
    // arr[16] = this state is set to op2		Yes
    // arr[17] = a state is set to op2		Yes

    // As you can see, it just so happens that negatives are replaced in a pattern (Yes, Yes, No, No, Yes, Yes, No, No, ...)

    // -5 = none
    // -4 = this type
    // -3 = a type
    // -2 = this state
    // -1 = a state

    if (reaction[0] == -5) {return;}

    int arr[18];

    int i = 0;
    while (i < 18)
    {
        switch(reaction[i])
        {
        case -4:
            arr[i] = element;
            break;
        case -3:
            arr[i] = a->element;
            break;
        case -2:
            arr[i] = state;
            break;
        case -1:
            arr[i] = a->state;
            break;
        default:
            arr[i] = reaction[i];
        }
        i++;

        switch(reaction[i])
        {
        case -4:
            arr[i] = element;
            break;
        case -3:
            arr[i] = a->element;
            break;
        case -2:
            arr[i] = state;
            break;
        case -1:
            arr[i] = a->state;
            break;
        default:
            arr[i] = reaction[i];
        }
        i++;

        if (i < 18)
        {
            arr[i] = reaction[i];
            i++;

            arr[i] = reaction[i];
            i++;
        }
    }

    if (arr[0] != element || arr[1] != a->element) {return;}

    if (testEqu(arr, 2, state) || testEqu(arr, 3, a->state)) {return;}

    int bondI;
    if (ltA) {bondI = bondGtI(a);}
    else {bondI = bondLtI(a);}

    bool nBonded = bondI == -1;

    if (nBonded == arr[10]) {return;}

    changeState(evalEqu(arr, 12));
    a->changeState(evalEqu(arr, 13));

    if (arr[11])
    {
        // Bond
        if (nBonded)
        {
            if (ltA)
            {
                if (numBondsGt < 6 && a->numBondsLt < 6)
                {
                    bondsGt[numBondsGt++] = a;
                    a->bondsLt[a->numBondsLt++] = this;
                }
            }
            else
            {
                if (numBondsLt < 6 && a->numBondsGt < 6)
                {
                    bondsLt[numBondsLt++] = a;
                    a->bondsGt[a->numBondsGt++] = this;
                }
            }
        }
    }
    else
    {
        // Remove bond
        if (!nBonded)
        {
            if (ltA)
            {
                removeBondGt(bondI);
                a->removeBondLt(a->bondLtI(this));
            }
            else
            {
                removeBondLt(bondI);
                a->removeBondGt(a->bondGtI(this));
            }
            if (symbol == "s" && a->symbol == "s")
            {
                // Both sugars
                qreal rx = a->nx - nx;
                qreal ry = a->ny - ny;
                qreal d = 10.0 / sqrt(rx * rx + ry * ry);
                rx *= d;
                ry *= d;
                vx -= rx;
                vy -= ry;
                a->vx += rx;
                a->vy += ry;
            }
        }
    }
}
bool Atom::testEqu(int* arr, int i, int num)
{
    if (arr[i] == -1) {return false;}

    int num2 = evalEqu(arr, i + 2);

    // This function actually returns the inverse of the comparison, so that the calling function can break if this function returns true.
    switch (arr[i])
    {
    case 0:
        // Equals
        return (num2 != num);
        break;
    case 1:
        // Does not equal
        return (num2 == num);
        break;
    case 2:
        // Greater than
        return (num2 < num);
        break;
    case 3:
        // Less than
        return (num2 > num);
        break;
    }
    return true;
}
int Atom::evalEqu(int* arr, int i)
{
    switch(arr[i + 2])
    {
    case 4:
        return arr[i] + arr[i + 4];
        break;
    case 5:
        return arr[i] - arr[i + 4];
        break;
    case 6:
        return arr[i] * arr[i + 4];
        break;
    case 7:
        return arr[i] / arr[i + 4];
        break;
    }
    return arr[i];
}


qreal Atom::getT(Atom *a, qreal relX, qreal relY, qreal dsq)
{
    // This function calculates and returns the number of timesteps to move each atom back.
    // This function returns an exact value.
    qreal relXv = a->vx - vx;
    qreal relYv = a->vy - vy;
    qreal relXv2 = relXv * relXv;
    qreal relYv2 = relYv * relYv;
    qreal relXXv = relX * relXv;
    qreal relYYv = relY * relYv;
    return (relXXv + relYYv + qSqrt(2 * relXXv * relYYv - relXv2 * relY * relY - relYv2 * relX * relX + (relYv2 + relXv2) * dsq)) / (relXv2 + relYv2);
}

void Atom::bounce(Atom *a, qreal t)
{
    // Moves the atom back t timesteps, so that the atoms are at the instant of the collision.
    // Then, we run the bounce function, which changes the velocities of each atom.
    // Then, we move each atom forward t timesteps.

    if (t > 1) {return;}

    qreal relX = a->nx - nx;
    qreal relY = a->ny - ny;
    qreal d = rad + a->rad;

    nx -= vx * t;
    ny -= vy * t;
    a->nx -= a->vx * t;
    a->ny -= a->vy * t;
    relX = a->nx - nx;
    relY = a->ny - ny;
    qreal ax = relX / d;
    qreal ay = relY / d;

    qreal p = 2 * (vx * ax + vy * ay - a->vx * ax - a->vy * ay) / (mass + a->mass);
    vx -= p * mass * ax;
    vy -= p * mass * ay;
    a->vx += p * a->mass * ax;
    a->vy += p * a->mass * ay;

    nx += vx * t;
    ny += vy * t;
    a->nx += a->vx * t;
    a->ny += a->vy * t;
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

    //painter->setClipRect( option->exposedRect );
    painter->setRenderHint(QPainter::Antialiasing, true);

    QString str = symbol+QString().setNum(state);

    QFont f = painter->font();
    //f.setStyleStrategy(QFont::NoAntialias);
    f.setPointSizeF(20-str.size()*2.5);
    f.setBold(selected);
    painter->setFont(f);

    painter->drawEllipse(atomRect);
    painter->setPen(color);
    painter->drawText(atomRect, Qt::AlignCenter, str);
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
