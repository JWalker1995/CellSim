#ifndef NEURON_H
#define NEURON_H

#include "atom.h"

class Neuron : public Atom
{
public:
    Neuron();

    void collide(Atom *a);
};

#endif // NEURON_H
