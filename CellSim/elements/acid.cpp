#include "acid.h"

Acid::Acid()
{
    rad = 20.0;
    mass = 1.0;
    symbol = "a";
    color = 0x00FF00;
}

void Acid::collide(Atom *a)
{
    if (a->symbol == "m") {return;}
    while (a->numBondsLt)
    {
        a->bondsLt[a->numBondsLt - 1]->removeBondGt(a);
        a->removeBondLt(a->numBondsLt);// This will decrement atom2->numBondsLt
    }
    while (a->numBondsGt)
    {
        a->bondsGt[a->numBondsGt - 1]->removeBondLt(a);
        a->removeBondGt(a->numBondsGt);// This will decrement atom2->numBondsGt
    }
}
