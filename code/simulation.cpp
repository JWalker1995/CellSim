#include "mainwindow.h"
#include "simulation.h"
#include "globals.h"
#include "atom.h"

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

    frameInterval = 0;
    timer = 0;

    numUndosAlloc = 64;
    numUndos = 0;

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

    modify();
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
        fps = 0;
        fpsTimer.start();
        timer = startTimer(frameInterval);
        modify();
    }
    else
    {
        timer = 0;
    }
}

void Simulation::timerEvent(QTimerEvent* event)
{
    int time = fpsTimer.restart();
    if (time)
    {
        fps = fps * 0.95 + 1000 / time * 0.05;
        qDebug() << fps;
    }

    advance();

    int curBond = 0;

    qreal dx;
    qreal dy;
    qreal dsq;
    qreal d;

    qreal e = 0.0;

    Atom *ref1;
    Atom *ref2;
    int i=0;
    int j;
    while (i < numAtoms)
    {
        ref1 = atoms[i];

        bins.moveAtom(ref1);

        ref1->vx *= energyMul;
        ref1->vy *= energyMul;

        d = sqrt(ref1->vx * ref1->vx + ref1->vy * ref1->vy);
        if (d > 50)
        {
            ref1->vx *= 50 / d;
            ref1->vy *= 50 / d;
            d = 50;
        }
        e += d;

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

        bins.runNeighbors(ref1, ref1->bin);
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

    bins.addAtom(a);

    modify();

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

    modify();

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
    modify();
}

void Simulation::modify()
{
    emit modified();
}


void Simulation::resize(int width, int height)
{
    bounds.setRect(0, 0, width, height);
    this->setSceneRect(bounds);
    bins.resize(width, height);

    modify();
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


void Simulation::addUndo(void (*func)())
{
    undos[]
}
