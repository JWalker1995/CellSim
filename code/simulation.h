#ifndef SIMULATION_H
#define SIMULATION_H

#include "atomeditor.h"
#include "bins.h"
#include "atom.h"

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QRectF>
#include <QElapsedTimer>

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

    void modify();

protected:
    int xBins;
    int yBins;

    Bins bins;

    int timer;
    void timerEvent(QTimerEvent *event);

    qreal fps;
    QElapsedTimer fpsTimer;

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

    struct
    {
        int a;
    } history;

    bool pressAdd;
    bool pressPan;
    bool pressSelect;

signals:
    void modified();
    
public slots:
    
};

#endif // SIMULATION_H
