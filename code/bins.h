#ifndef BINS_H
#define BINS_H

#include "atom.h"

class Bins
{
public:
    Bins();

    void resize(int width, int height);

    void addAtom(Atom* a);
    void moveAtom(Atom* a);

    void runNeighbors(Atom* a, Atom** bin);

private:
    int binsX;

    Atom** binsStart;
    Atom** binsEnd;

    bool addToBin(Atom* a, Atom **bin);
    void removeFromBin(Atom* a, Atom** bin);

    void run3Bins(Atom* a, Atom** bin);
};

#endif // BINS_H
