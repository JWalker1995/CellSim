#ifndef SIMVIEW_H
#define SIMVIEW_H

#include "simulation.h"

#include <QGraphicsView>

class SimView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SimView(Simulation *scene, QWidget* parent);
    
signals:
    
public slots:
    
};

#endif // SIMVIEW_H
