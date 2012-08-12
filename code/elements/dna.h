#ifndef DNA_H
#define DNA_H

#include "atom.h"

class Dna : public Atom
{
public:
    Dna();

    QString codon;
};

#endif // DNA_H
