#include "element.h"
#include <QString>
#include <QColor>

Element::Element(QString name2, QString symbol2, qreal rad2, qreal mass2, int color2)
{
    name=name2;
    symbol=symbol2.toLower();
    rad=rad2;
    mass=mass2;
    color=color2;
}
