#include "neuron.h"

Neuron::Neuron()
{
    rad = 20.0;
    mass = 1.0;
    symbol = "n";
    color = 0x000000;

    atomRect.setRect(-20, -20, 40, 40);
}

void Neuron::collide(Atom *a)
{
    if (a->symbol != "n" || state > a->state) {return;}

    if (a->i < i)
    {
        // Use bondsLt
        if (bondLtI(a) != -1)
        {
            state = a->state;
            update();
        }
    }
    else
    {
        // Use bondsGt
        if (bondGtI(a) != -1)
        {
            state = a->state;
            update();
        }
    }
}
