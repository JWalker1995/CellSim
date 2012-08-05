#include "mainwindow.h"
#include "simulation.h"
#include "globals.h"
#include "atom.h"

#include "elements/membrane.h"
#include "elements/acid.h"
#include "elements/carbon.h"
#include "elements/ribosome.h"
#include "elements/neuron.h"
#include "elements/gold.h"
#include "elements/uranium.h"
#include "elements/adenine.h"
#include "elements/thymine.h"
#include "elements/guanine.h"
#include "elements/cytosine.h"
#include "elements/beta.h"
#include "elements/delta.h"
#include "elements/sugar.h"
#include "elements/motorneuron.h"
#include "elements/atp.h"
#include "elements/mitochondrion.h"

#include <QRectF>
#include <QPainter>
#include <QString>
#include <QColor>
#include <QGraphicsSceneMouseEvent>
#include <QPoint>
#include <QListIterator>
#include <QMessageBox>
#include <QtCore/qmath.h>
#include <QDebug>

Simulation::Simulation(QWidget* parent = 0) : QGraphicsScene(parent)
{
    LogBlock b = Globals::log->scopedBlock("Initializing new simulation");

    resize(1000,500);

    pressAdd=false;
    pressPan=false;
    pressSelect=false;

    numAtomsAlloc = 64;
    atoms = new Atom*[numAtomsAlloc];
    numAtoms = 0;

    numBondsAlloc = 64;
    bonds = new QGraphicsLineItem*[numBondsAlloc];
    int i = 0;
    while (i < numBondsAlloc)
    {
        bonds[i] = addLine(0, 0, 0, 0);
        bonds[i]->setVisible(false);
        i++;
    }
    numBonds = 0;

    targEnergy = 1.5;
    energyMul = 1.0;

    frameInterval = 20;
    timer = 0;

    Globals::ae->init(&selected);
}

Simulation::~Simulation()
{
    deleteItems();
}

void Simulation::deleteItems()
{
    int i = 0;
    while (i < numAtoms)
    {
        delete atoms[i];
        i++;
    }

    i = 0;
    while (i < numBonds)
    {
        delete bonds[i];
        i++;
    }

    delete[] atoms;
    delete[] bonds;
}

void Simulation::reset()
{
    LogBlock b = Globals::log->scopedBlock("Resetting simulation");

    resize(1000,500);

    selected.clear();
    deleteItems();

    numAtomsAlloc = 64;
    atoms = new Atom*[numAtomsAlloc];
    numAtoms = 0;

    numBondsAlloc = 64;
    bonds = new QGraphicsLineItem*[numBondsAlloc];
    int i = 0;
    while (i < numBondsAlloc)
    {
        bonds[i] = addLine(0, 0, 0, 0);
        bonds[i]->setVisible(false);
        i++;
    }
    numBonds = 0;

    targEnergy = 1.5;
    energyMul = 1.0;

    frameInterval = 20;
}

void Simulation::allocAtoms()
{
    LogBlock b = Globals::log->scopedBlock("Allocating " + QString().setNum(numAtomsAlloc * 2) + " atoms");

    Atom** temp = new Atom*[numAtomsAlloc * 2];

    int i = 0;
    while (i < numAtomsAlloc)
    {
        temp[i] = atoms[i];
        i++;
    }

    numAtomsAlloc *= 2;

    delete[] atoms;
    atoms = temp;
}

void Simulation::allocBonds()
{
    LogBlock b = Globals::log->scopedBlock("Allocating " + QString().setNum(numBondsAlloc * 2) + " bonds");

    QGraphicsLineItem** temp = new QGraphicsLineItem*[numBondsAlloc * 2];

    int i = 0;
    while (i < numBondsAlloc)
    {
        temp[i] = bonds[i];
        i++;
    }

    numBondsAlloc *= 2;

    while (i < numBondsAlloc)
    {
        temp[i] = addLine(0, 0, 0, 0);
        temp[i]->setVisible(false);
        i++;
    }
    delete[] bonds;
    bonds = temp;
}

void Simulation::drawForeground(QPainter *painter, const QRectF &rect)
{
    painter->setPen(QPen(Qt::black, 4));
    //painter->setRenderHint(QPainter::Antialiasing);
    painter->drawRect(bounds);
}

void Simulation::playPause(bool play)
{
    if (timer)
    {
        killTimer(timer);
    }
    if (play)
    {
        timer = startTimer(frameInterval);
    }
    else
    {
        timer = 0;
    }
}

void Simulation::timerEvent(QTimerEvent* event)
{
    advance();

    int curBond = 0;

    qreal dx;
    qreal dy;
    qreal r;

    qreal dxsq;
    qreal dysq;
    qreal dsq;
    qreal d;
    qreal rsq;

    qreal e = 0.0;

    Atom *ref1;
    Atom *ref2;
    int i=0;
    int j;
    while (i < numAtoms)
    {
        ref1 = atoms[i];

        ref1->vx *= energyMul;
        ref1->vy *= energyMul;

        e += sqrt(ref1->vx * ref1->vx + ref1->vy * ref1->vy);

        j = 0;
        while (j < ref1->numBondsLt)
        {
            ref2 = ref1->bondsLt[j];
            dx = ref2->nx - ref1->nx;
            dy = ref2->ny - ref1->ny;
            dsq = dx * dx + dy * dy;
            d = sqrt(dsq) / 5e4;
            dx *= d;
            dy *= d;
            ref1->vx += dx;
            ref1->vy += dy;
            ref2->vx -= dx;
            ref2->vy -= dy;

            if (curBond >= numBondsAlloc) {allocBonds();}
            bonds[curBond]->setLine(ref1->nx, ref1->ny, ref2->nx, ref2->ny);
            bonds[curBond]->setVisible(true);
            curBond++;

            j++;
        }

        j = 0;
        while (j < i)
        {
            ref2 = atoms[j];
            dx = ref2->nx - ref1->nx;
            dy = ref2->ny - ref1->ny;
            r = ref1->rad + ref2->rad;
            if (abs(dx) < r && abs(dy) < r)
            {
                dxsq = dx * dx;
                dysq = dy * dy;
                dsq = dxsq + dysq;
                rsq = r * r;
                if (dsq < rsq)
                {
                    // Colliding
                    if (rand() < 0)
                    {
                        if (ref1->numBondsLt < 6 && ref2->numBondsGt < 6)
                        {
                            int k = 0;
                            while (k < ref1->numBondsLt)
                            {
                                if (ref1->bondsLt[k] == ref2)
                                {
                                    k = 6;
                                }
                                k++;
                            }
                            if (k != 7)
                            {
                                ref1->bondsLt[ref1->numBondsLt++] = ref2;
                                ref2->bondsGt[ref2->numBondsGt++] = ref1;
                            }
                        }
                    }

                    fastBounce(ref1, ref2, getT(ref1, ref2, dx, dy, dsq));
                    runReaction(ref1, ref2, false);
                    runReaction(ref2, ref1, true);
                    ref1->collide(ref2);
                    ref2->collide(ref1);
                }
            }
            j++;
        }
        i++;
    }
    while (numBonds > curBond)
    {
        bonds[--numBonds]->setVisible(false);
    }
    numBonds = curBond;

    if (numAtoms)
    {
        e = numAtoms / e * targEnergy;
        if (e > 1.01) {e = 1.01;}
        else if (e < .99) {e = .99;}
        energyMul = e;
    }
}

void Simulation::runReaction(Atom *a1, Atom *a2, bool a1LtA2)
{
    // The items of arr							Replace Negatives with their values
    // arr[0] = a1 type must match			Yes
    // arr[1] = a2 type must match			Yes
    // arr[2] = a1 state comparison			No (<, >, =, !=)
    // arr[3] = a2 state comparison			No (<, >, =, !=)
    // arr[4] = a1 op1						Yes
    // arr[5] = a2 op1						Yes
    // arr[6] = a1 operand					No (+, -, *, /)
    // arr[7] = a2 operand					No (+, -, *, /)
    // arr[8] = a1 op2						Yes
    // arr[9] = a2 op2						Yes
    // arr[10] = atom bond must match			No (0, 1)
    // arr[11] = atom bond is set to			No (0, 1)
    // arr[12] = a1 state is set to op1		Yes
    // arr[13] = a2 state is set to op1		Yes
    // arr[14] = a1 state is set to operator	No (+, -, *, /)
    // arr[15] = a2 state is set to operator	No (+, -, *, /)
    // arr[16] = a1 state is set to op2		Yes
    // arr[17] = a2 state is set to op2		Yes

    // As you can see, it just so happens that negatives are replaced in a pattern (Yes, Yes, No, No, Yes, Yes, No, No, ...)

    // -5 = none
    // -4 = a1 type
    // -3 = a2 type
    // -2 = a1 state
    // -1 = a2 state

    if (a1->reaction[0] == -5) {return;}

    int arr[18];

    int i = 0;
    while (i < 18)
    {
        switch(a1->reaction[i])
        {
        case -4:
            arr[i] = a1->element;
            break;
        case -3:
            arr[i] = a2->element;
            break;
        case -2:
            arr[i] = a1->state;
            break;
        case -1:
            arr[i] = a2->state;
            break;
        default:
            arr[i] = a1->reaction[i];
        }
        i++;

        switch(a1->reaction[i])
        {
        case -4:
            arr[i] = a1->element;
            break;
        case -3:
            arr[i] = a2->element;
            break;
        case -2:
            arr[i] = a1->state;
            break;
        case -1:
            arr[i] = a2->state;
            break;
        default:
            arr[i] = a1->reaction[i];
        }
        i++;

        if (i < 18)
        {
            arr[i] = a1->reaction[i];
            i++;

            arr[i] = a1->reaction[i];
            i++;
        }
    }

    if (arr[0] != a1->element || arr[1] != a2->element) {return;}

    if (testEqu(arr, 2, a1->state) || testEqu(arr, 3, a2->state)) {return;}

    int bondI;
    if (a1LtA2) {bondI = a1->bondGtI(a2);}
    else {bondI = a1->bondLtI(a2);}

    bool nBonded = bondI == -1;

    if (nBonded == arr[10]) {return;}

    a1->changeState(evalEqu(arr, 12));
    a2->changeState(evalEqu(arr, 13));

    if (arr[11])
    {
        // Bond
        if (nBonded)
        {
            if (a1LtA2)
            {
                if (a1->numBondsGt < 6 && a2->numBondsLt < 6)
                {
                    a1->bondsGt[a1->numBondsGt++] = a2;
                    a2->bondsLt[a2->numBondsLt++] = a1;
                }
            }
            else
            {
                if (a1->numBondsLt < 6 && a2->numBondsGt < 6)
                {
                    a1->bondsLt[a1->numBondsLt++] = a2;
                    a2->bondsGt[a2->numBondsGt++] = a1;
                }
            }
        }
    }
    else
    {
        // Remove bond
        if (!nBonded)
        {
            if (a1LtA2)
            {
                a1->removeBondGt(bondI);
                a2->removeBondLt(a2->bondLtI(a1));
            }
            else
            {
                a1->removeBondLt(bondI);
                a2->removeBondGt(a2->bondGtI(a1));
            }
        }
    }
}

bool Simulation::testEqu(int* arr, int i, int num)
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

int Simulation::evalEqu(int* arr, int i)
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


qreal Simulation::getT(Atom *ref1, Atom *ref2, qreal relX, qreal relY, qreal dsq)
{
    // This function calculates and returns the number of timesteps to move each atom back.
    // This function returns an exact value.
    qreal relXv = ref2->vx - ref1->vx;
    qreal relYv = ref2->vy - ref1->vy;
    qreal relXv2 = relXv * relXv;
    qreal relYv2 = relYv * relYv;
    qreal relXXv = relX * relXv;
    qreal relYYv = relY * relYv;
    return (relXXv + relYYv + qSqrt(2 * relXXv * relYYv - relXv2 * relY * relY - relYv2 * relX * relX + (relYv2 + relXv2) * dsq)) / (relXv2 + relYv2);
}

void Simulation::fastBounce(Atom *ref1, Atom *ref2, qreal t)
{
    // Moves the atom back t timesteps, so that the atoms are at the instant of the collision.
    // Then, we run the bounce function, which changes the velocities of each atom.
    // Then, we move each atom forward t timesteps.

    if (t > 1) {return;}

    qreal relX = ref2->nx - ref1->nx;
    qreal relY = ref2->ny - ref1->ny;
    qreal d = ref1->rad + ref2->rad;

    ref1->nx -= ref1->vx * t;
    ref1->ny -= ref1->vy * t;
    ref2->nx -= ref2->vx * t;
    ref2->ny -= ref2->vy * t;
    relX = ref2->nx - ref1->nx;
    relY = ref2->ny - ref1->ny;
    qreal ax = relX / d;
    qreal ay = relY / d;

    qreal p = 2 * (ref1->vx * ax + ref1->vy * ay - ref2->vx * ax - ref2->vy * ay) / (ref1->mass + ref2->mass);
    ref1->vx -= p * ref1->mass * ax;
    ref1->vy -= p * ref1->mass * ay;
    ref2->vx += p * ref2->mass * ax;
    ref2->vy += p * ref2->mass * ay;

    ref1->nx += ref1->vx * t;
    ref1->ny += ref1->vy * t;
    ref2->nx += ref2->vx * t;
    ref2->ny += ref2->vy * t;

/*
   qreal va1 = ref1.vx * ax + ref1.vy * ay;
   qreal vb1 = -ref1.vx * ay + ref1.vy * ax;
   qreal va2 = ref2.vx * ax + ref2.vy * ay;
   qreal vb2 = -ref2.vx * ay + ref2.vy * ax;
   qreal vap1 = va1 + 2 * (va2 - va1) / (1 + ref1.mass / ref2.mass);
   qreal vap2 = va2 + 2 * (va1 - va2) / (1 + ref2.mass / ref1.mass);
   ref1.vx = vap1 * ax - vb1 * ay;
   ref1.vy = vap1 * ay + vb1 * ax;
   ref2.vx = vap2 * ax - vb1 * ay;
   ref2.vy = vap2 * ay + vb2 * ax;
   ref1.x += ref1.vx * t;
   ref1.y += ref1.vy * t;
   ref2.x += ref2.vx * t;
   ref2.y += ref2.vy * t;
 */
}

void Simulation::slowBounce(Atom *ref1, Atom *ref2, qreal t)
{
    fastBounce(ref1, ref2, t);
}

Atom* Simulation::addAtom(QPointF pos, int element, unsigned short state)
{
    Atom *a = Globals::getAtom(element);

    a->i = numAtoms;

    a->setPos(pos);
    a->nx=pos.x();
    a->ny=pos.y();
    a->changeState(state);

    addItem(a);
    a->changeSceneSize();

    if (numAtoms >= numAtomsAlloc) {allocAtoms();}
    atoms[numAtoms] = a;
    numAtoms++;
    return a;
}

void Simulation::replaceAtom(int selI, Atom* a, Atom* b)
{
    b->i = a->i;
    b->setPos(a->pos());
    b->state = a->state;
    b->nx = a->nx;
    b->ny = a->ny;
    b->vx = a->vx;
    b->vy = a->vy;
    memcpy(b->bondsLt, a->bondsLt, sizeof(Atom*) * 6);
    memcpy(b->bondsGt, a->bondsGt, sizeof(Atom*) * 6);
    b->numBondsLt = a->numBondsLt;
    b->numBondsGt = a->numBondsGt;
    memcpy(b->reaction, a->reaction, sizeof(int) * 18);
    b->reactionStr = a->reactionStr;
    b->selected = a->selected;

    int j = 0;
    while (j < b->numBondsLt)
    {
        int k = 0;
        int c = b->bondsLt[j]->numBondsGt;
        Atom** bonds = b->bondsLt[j]->bondsGt;
        while (k < c)
        {
            if (bonds[k] == a) {bonds[k] = b;}
            k++;
        }
        j++;
    }
    j = 0;
    while (j < b->numBondsGt)
    {
        int k = 0;
        int c = b->bondsGt[j]->numBondsLt;
        Atom** bonds = b->bondsGt[j]->bondsLt;
        while (k < c)
        {
            if (bonds[k] == a) {bonds[k] = b;}
            k++;
        }
        j++;
    }

    removeItem(a);
    addItem(b);
    b->changeSceneSize();

    atoms[a->i] = b;

    if (selI != -1)
    {
        selected[selI] = b;
    }

    delete a;
/*
    int actualNumAtoms = numAtoms;
    numAtoms = atomIndex(a);
    qDebug() << numAtoms;
    qDebug() << a;
    removeItem(atoms[numAtoms]);
    qDebug() << "a";
    addAtom(QPointF(a->nx, a->ny), type, a->state);
    qDebug() << atoms[numAtoms - 1];

    selected.replace(i, atoms[numAtoms - 1]);

    numAtoms = actualNumAtoms;
    qDebug() << "e";

    delete a;

    qDebug() << "f";

    */
}
void Simulation::deleteAtom()
{
}


void Simulation::resize(int width, int height)
{
    bounds.setRect(0, 0, width, height);
    this->setSceneRect(bounds);
}

void Simulation::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
        pressAdd=true;
        break;
    case Qt::MiddleButton:
        pressPan=true;
        break;
    case Qt::RightButton:
        pressSelect=true;
        break;
    default:;
    }

    mouseMoveEvent(event);// Even if the mouse doesn't move, still call the events
}
void Simulation::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
        pressAdd=false;
        break;
    case Qt::MiddleButton:
        pressPan=false;
        break;
    case Qt::RightButton:
        pressSelect=false;
        break;
    default:;
    }
}

void Simulation::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    // Left click: If mouse is over an atom, set add type to that atom. Add atom if not overlapping
    // Middle click: Pan
    // Right click: Create selection rectangle
    QPointF pos=event->scenePos();
    if (pressSelect)
    {
        bool shiftPressed=event->modifiers().testFlag(Qt::ShiftModifier);
        bool controlPressed=event->modifiers().testFlag(Qt::ControlModifier);
        if (!shiftPressed && !controlPressed)
        {
            // Shift key is not pressed - deselect all
            int c=selected.count();
            while (c--)
            {
                selected[c]->deselect();
            }
            selected.clear();
            Globals::ae->updateRemove();
        }
        QGraphicsItem *item=itemAt(pos);
        if (item!=0)
        {
            Atom* ref1=dynamic_cast<Atom*>(item);
            if (controlPressed)
            {
                if (ref1->deselect())
                {
                    selected.removeOne(ref1);
                    Globals::ae->updateRemove();
                }
            }
            else
            {
                if (ref1->select())
                {
                    selected.append(ref1);
                    Globals::ae->updateAdd();
                }
            }
        }
    }

    if (pressPan)
    {

    }

    if (pressAdd)
    {
        qreal rad=20;
        if (pos.x()<rad || pos.y()<rad || pos.x()>bounds.width()-rad || pos.y()>bounds.height()-rad) {return;}

        qreal dx;
        qreal dy;
        qreal r;

        Atom *ref1;

        int i=0;
        while (i<numAtoms)
        {
            ref1=atoms[i];
            dx=pos.x()-ref1->nx;
            dy=pos.y()-ref1->ny;
            r=ref1->rad+rad;
            if (abs(dx)<r && abs(dy)<r && dx*dx+dy*dy < r*r)
            {
                return;
            }
            i++;
        }
        addAtom(pos,rand()*Globals::numElements/RAND_MAX,1);
    }
}

void Simulation::selectAll()
{
    int i = 0;
    while (i < numAtoms)
    {
        if (atoms[i]->select())
        {
            selected.append(atoms[i]);
            Globals::ae->updateAdd();
        }
        i++;
    }
}
