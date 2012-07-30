#include "simview.h"

SimView::SimView(Simulation *scene, QWidget *parent) :
    QGraphicsView(scene, parent)
{
    setOptimizationFlag(QGraphicsView::DontSavePainterState);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}
