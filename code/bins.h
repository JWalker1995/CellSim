#ifndef BINS_H
#define BINS_H

#include "atom.h"

class Bins
{
public:
    Bins(int width, int height);

    void addAtom(Atom* a);
    void moveAtom(Atom* a);

private:
    int binsX;
    int binsY;

    void** bins;
    void** overflowStart;
    void** overflowCur;
    void** overflowEnd;

    void addToBin(void** bin, Atom* a);
    void removeFromBin(void** bin);
};

#endif // BINS_H
