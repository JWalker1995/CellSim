#ifndef ACID_H
#define ACID_H

#include "atom.h"

class Acid : public Atom
{
public:
    Acid();

    void collide(Atom *a);
};

#endif // ACID_H
