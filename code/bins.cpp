// The physical size of the bin in pixels
// 0 < BIN_PX < 2 * (rad of largest atom)
#define BIN_PX 50

// The number of atoms that can be contained in one bin
#define BIN_CAP 3

// The amount of overflow bins, as a ratio of the number of normal bins
#define BIN_OVERFLOW 0.1



#define BIN_SIZE BIN_CAP+1

#define XY_TO_BIN(x,y) bins + x + y * binsX

#include "bins.h"

Bins::Bins(int width, int height)
{
    binsX = width / BIN_PX;
    binsY = height / BIN_PX;

    int binsLen = binsX * binsY * BIN_SIZE;
    int overflowLen = int(binsX * binsY * BIN_OVERFLOW) * BIN_SIZE;
    bins = new Bin[binsLen];
    overflowStart = new Bin[overflowLen];
    overflowCur = overflowStart;
    overflowEnd = overflowStart + overflowLen;
}

void Bins::addAtom(Atom* a)
{
    int binX = int(a->nx / BIN_PX);
    int binY = int(a->ny / BIN_PX);

    int i = -1;
    while (i < 2)
    {
        int j = -1;
        while (j < 2)
        {
            Bin* bin = XY_TO_BIN(binX + i, binY + j);
            addToBin(bin, a);
            if (!i && !j) {a->bin = bin;}
            j++;
        }
        i++;
    }
}

void Bins::moveAtom(Atom* a)
{
    Bin* bin = XY_TO_BIN(int(a->nx / BIN_PX), int(a->ny / BIN_PX));
    Bin* oldBin = a->bin;
    if (oldBin == bin) {return;}

    if (oldBin + 1 == bin)
    {
        // Atom moved one bin to the right
        oldBin -= binsX + 1; // Move oldBin up and to the left.
        removeFromBin(oldBin, a);
        oldBin += binsX; // Move down
        removeFromBin(oldBin, a);
        oldBin += binsX; // Move down
        removeFromBin(oldBin, a);
        oldBin += 3; // Move 3 bins to the right
        addToBin(oldBin, a);
        oldBin -= binsX; // Move up
        addToBin(oldBin, a);
        oldBin -= binsX; // Move up
        addToBin(oldBin, a);
    }
    else if (oldBin - 1 == bin)
    {
        // Atom moved one bin to the left
        oldBin -= binsX - 1; // Move oldBin up and to the right.
        removeFromBin(oldBin, a);
        oldBin += binsX; // Move down
        removeFromBin(oldBin, a);
        oldBin += binsX; // Move down
        removeFromBin(oldBin, a);
        oldBin -= 3; // Move 3 bins to the left
        addToBin(oldBin, a);
        oldBin -= binsX; // Move up
        addToBin(oldBin, a);
        oldBin -= binsX; // Move up
        addToBin(oldBin, a);
    }
    a->bin = bin;
}

void Bins::addToBin(Bin* bin, Atom *a)
{
    Bin* eb = bin + BIN_CAP;
    while (bin < eb)
    {
        if (!bin->a) {bin->a = a;return;}
        bin++;
    }
    if (bin->b)
    {
        bin = bin->b;
    }
    else
    {
        // This bin has not overflowed yet.
        bin = overflowCur;
        overflowCur += BIN_SIZE;
        if (overflowCur == overflowEnd)
        {
            overflowCur = overflowStart;
        }
    }
    addToBin(bin, a);
}

void Bins::removeFromBin(Bin* bin, Atom* a)
{
    Bin* eb = bin + BIN_CAP;
    while (bin < eb)
    {
        if (bin->a == a) {bin->a = 0;return;}
        bin++;
    }
    if (bin->b)
    {
        // This bin has overflowed
        removeFromBin(bin->b, a);
    }
}

void Bins::runNeighbors(Atom *a, Bin* bin)
{
    Bin* eb = bin + BIN_CAP;
    while (bin < eb)
    {
        if (bin->a) {a->neighbor(bin->a);}
        bin++;
    }
    if (bin->b)
    {
        // This bin has overflowed
        runNeighbors(a, bin->b);
    }
}
