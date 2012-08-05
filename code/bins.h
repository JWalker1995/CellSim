#ifndef BINS_H
#define BINS_H

#include "atom.h"

class Atom;

union Bin
{
    Atom* a;
    Bin* b;
};

class Bins
{
public:
    Bins();

    void resize(int width, int height);

    void addAtom(Atom* a);
    void moveAtom(Atom* a);

    void runNeighbors(Atom* a, Bin *bin);

private:
    int binsX;
    int binsY;

    Bin* binsStart;
    Bin* binsEnd;

    Bin* overflowStart;
    Bin* overflowCur;
    Bin* overflowEnd;

    void addToBin(Bin* bin, Atom* a);
    void removeFromBin(Bin* bin, Atom* a);
};

#endif // BINS_H
