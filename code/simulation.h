#ifndef SIMULATION_H
#define SIMULATION_H

#include "atomeditor.h"

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QRectF>
#include <atom.h>

class Simulation : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Simulation(QWidget *parent);
    ~Simulation();

    void playPause(bool play);

    void reset();

    Atom* addAtom(QPointF pos, int element, unsigned short state);
    void replaceAtom(int selI, Atom *a, Atom *b);
    void deleteAtom();
    void resize(int width, int height);

    void selectAll();

    bool frameSpeedSlow;

    int frameInterval;

    qreal targEnergy;

    int numAtoms;
    int numAtomsAlloc;
    int numBonds;
    int numBondsAlloc;

    Atom** atoms;
    QList<Atom*> selected;

protected:
    int xBins;
    int yBins;

    Bins bins;

    int timer;
    void timerEvent(QTimerEvent *event);

    void drawForeground(QPainter *painter, const QRectF &rect);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

    void deleteItems();

    QRectF bounds;

    qreal energyMul; // Energy multiplier

    void allocAtoms();

    QGraphicsLineItem** bonds;
    void allocBonds();

    bool pressAdd;
    bool pressPan;
    bool pressSelect;

signals:
    
public slots:
    
};

#endif // SIMULATION_H
