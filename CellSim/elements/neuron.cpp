#include "neuron.h"

Neuron::Neuron()
{
    rad = 20.0;
    mass = 1.0;
    symbol = "n";
    color = 0x000000;
}

void Neuron::collide(Atom *a)
{
    if (a->symbol != "n" || state > a->state) {return;}

    if (a->i < i)
    {
        // Use bondsLt
        int i = 0;
        while (i < numBondsLt)
        {
            if (bondsLt[i] == a)
            {
                state = a->state;
                return;
            }
            i++;
        }
    }
    else
    {
        // Use bondsGt
        int i = 0;
        while (i < numBondsGt)
        {
            if (bondsGt[i] == a)
            {
                state = a->state;
                return;
            }
            i++;
        }
    }
}
