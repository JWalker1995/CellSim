#ifndef BINS_H
#define BINS_H

#include "atom.h"

union Bin
{
    Atom* a;
    Bin* b;
}

class Bins
{
public:
    Bins(int width, int height);

    void addAtom(Atom* a);
    void moveAtom(Atom* a);

private:
    int binsX;
    int binsY;

    Bin* bins;

    //void** bins;
    Bin* overflowStart;
    Bin* overflowCur;
    Bin* overflowEnd;

    void addToBin(Bin* bin, Atom* a);
    void removeFromBin(Bin* bin, Atom* a);
};

#endif // BINS_H
