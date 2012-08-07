#include "acid.h"

#include <QDebug>

Acid::Acid()
{
    rad = 20.0;
    mass = 1.0;
    symbol = "a";
    color = 0x00FF00;

    atomRect.setRect(-20, -20, 40, 40);
}

void Acid::collide(Atom *a)
{
    if (a->symbol == "m") {return;}
    a->removeBonds();
}
