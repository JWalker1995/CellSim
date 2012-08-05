// The physical size of the bin in pixels
// 0 < BIN_PX < 2 * (rad of largest atom)
#define BIN_PX 50.0

// The number of atoms that can be contained in one bin
#define BIN_CAP 7

// The amount of overflow bins, as a ratio of the number of normal bins
#define BIN_OVERFLOW 5.0



#define BIN_SIZE BIN_CAP+1

#define XY_TO_BIN(x,y) binsStart + ((x) + (y) * binsX) * BIN_SIZE

#include "bins.h"
#include <QDebug>

Bins::Bins() {}

void Bins::resize(int width, int height)
{
    binsX = int(width / BIN_PX);
    binsY = int(height / BIN_PX);

    int binsLen = binsX * binsY * BIN_SIZE;
    binsStart = new Bin[binsLen];
    binsEnd = binsStart + binsLen;

    int overflowLen = int(binsX * binsY * BIN_OVERFLOW) * BIN_SIZE;
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
        // Atom moved east
        oldBin -= binsX + 1; // Move oldBin northwest
        removeFromBin(oldBin, a);
        oldBin += binsX; // Move south
        removeFromBin(oldBin, a);
        oldBin += binsX; // Move south
        removeFromBin(oldBin, a);
        oldBin += 3; // Move 3 bins to the east
        addToBin(oldBin, a);
        oldBin -= binsX; // Move north
        addToBin(oldBin, a);
        oldBin -= binsX; // Move north
        addToBin(oldBin, a);
    }
    else if (oldBin - 1 == bin)
    {
        // Atom moved west
        oldBin += binsX + 1; // Move oldBin southeast
        removeFromBin(oldBin, a);
        oldBin -= binsX; // Move north
        removeFromBin(oldBin, a);
        oldBin -= binsX; // Move north
        removeFromBin(oldBin, a);
        oldBin -= 3; // Move 3 bins to the west
        addToBin(oldBin, a);
        oldBin += binsX; // Move south
        addToBin(oldBin, a);
        oldBin += binsX; // Move south
        addToBin(oldBin, a);
    }
    else if (oldBin + binsX == bin)
    {
        // Atom moved south
        oldBin -= binsX + 1; // Move oldBin northwest
        removeFromBin(oldBin, a);
        oldBin++;// Move east
        removeFromBin(oldBin, a);
        oldBin++;// Move east
        removeFromBin(oldBin, a);
        oldBin += binsX * 3;// Move 3 bins south
        addToBin(oldBin, a);
        oldBin--;// Move west
        addToBin(oldBin, a);
        oldBin--;// Move west
        addToBin(oldBin, a);
    }
    else if (oldBin - binsX == bin)
    {
        // Atom moved north
        oldBin += binsX + 1; // Move oldBin southeast
        removeFromBin(oldBin, a);
        oldBin--;// Move west
        removeFromBin(oldBin, a);
        oldBin--;// Move west
        removeFromBin(oldBin, a);
        oldBin -= binsX * 3;// Move 3 bins north
        addToBin(oldBin, a);
        oldBin++;// Move east
        addToBin(oldBin, a);
        oldBin++;// Move east
        addToBin(oldBin, a);
    }
    else if (oldBin + 1 + binsX == bin)
    {
        // Atom moved southeast
        oldBin -= binsX - 1;// Move oldBin northeast
        removeFromBin(oldBin, a);
        oldBin--;// Move west
        removeFromBin(oldBin, a);
        oldBin--;// Move west
        removeFromBin(oldBin, a);
        oldBin += binsX;// Move south
        removeFromBin(oldBin, a);
        oldBin += binsX;// Move south
        removeFromBin(oldBin, a);
        oldBin -= binsX - 3;// Move one bin north and 3 bins east
        addToBin(oldBin, a);
        oldBin += binsX;// Move south
        addToBin(oldBin, a);
        oldBin += binsX;// Move south
        addToBin(oldBin, a);
        oldBin--;// Move west
        addToBin(oldBin, a);
        oldBin--;// Move west
        addToBin(oldBin, a);
    }
    else if (oldBin - 1 - binsX == bin)
    {
        // Atom moved northwest
        oldBin += binsX - 1;// Move oldBin southwest
        removeFromBin(oldBin, a);
        oldBin++;// Move east
        removeFromBin(oldBin, a);
        oldBin++;// Move east
        removeFromBin(oldBin, a);
        oldBin -= binsX;// Move north
        removeFromBin(oldBin, a);
        oldBin -= binsX;// Move north
        removeFromBin(oldBin, a);
        oldBin += binsX - 3;// Move one bin south and 3 bins west
        addToBin(oldBin, a);
        oldBin -= binsX;// Move north
        addToBin(oldBin, a);
        oldBin -= binsX;// Move north
        addToBin(oldBin, a);
        oldBin++;// Move east
        addToBin(oldBin, a);
        oldBin++;// Move east
        addToBin(oldBin, a);
    }
    else if (oldBin - 1 + binsX == bin)
    {
        // Atom moved southwest
        oldBin -= binsX + 1;// Move oldBin northwest
        removeFromBin(oldBin, a);
        oldBin++;// Move east
        removeFromBin(oldBin, a);
        oldBin++;// Move east
        removeFromBin(oldBin, a);
        oldBin += binsX;// Move south
        removeFromBin(oldBin, a);
        oldBin += binsX;// Move south
        removeFromBin(oldBin, a);
        oldBin -= binsX + 3;// Move one bin north and 3 bins west
        addToBin(oldBin, a);
        oldBin += binsX;// Move south
        addToBin(oldBin, a);
        oldBin += binsX;// Move south
        addToBin(oldBin, a);
        oldBin++;// Move east
        addToBin(oldBin, a);
        oldBin++;// Move east
        addToBin(oldBin, a);
    }
    else if (oldBin + 1 - binsX == bin)
    {
        // Atom moved northeast
        oldBin += binsX + 1;// Move oldBin southeast
        removeFromBin(oldBin, a);
        oldBin--;// Move west
        removeFromBin(oldBin, a);
        oldBin--;// Move west
        removeFromBin(oldBin, a);
        oldBin -= binsX;// Move north
        removeFromBin(oldBin, a);
        oldBin -= binsX;// Move north
        removeFromBin(oldBin, a);
        oldBin += binsX + 3;// Move one bin south and 3 bins east
        addToBin(oldBin, a);
        oldBin -= binsX;// Move north
        addToBin(oldBin, a);
        oldBin -= binsX;// Move north
        addToBin(oldBin, a);
        oldBin--;// Move west
        addToBin(oldBin, a);
        oldBin--;// Move west
        addToBin(oldBin, a);
    }
    a->bin = bin;
}

void Bins::addToBin(Bin* bin, Atom *a)
{
    if ((bin < binsStart || bin >= binsEnd) && (bin < overflowStart || bin >= overflowEnd)) {return;}

    Bin* eb = bin + BIN_CAP;
    while (bin < eb)
    {
        if (!bin->a) {bin->a = a;return;}
        bin++;
    }
    if (!bin->b)
    {
        // This bin has not overflowed yet.
        bin->b = overflowCur;
        overflowCur += BIN_SIZE;
        if (overflowCur == overflowEnd)
        {
            overflowCur = overflowStart;
        }
    }
    addToBin(bin->b, a);
}

void Bins::removeFromBin(Bin* bin, Atom* a)
{
    qDebug() << "a";
    Bin* eb = bin + BIN_CAP;
    while (bin < eb)
    {
        if (bin->a == a) {bin->a = 0;qDebug() << "b";return;}
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
    qDebug() << "run" << bin << (bin - binsStart) << (bin - overflowStart);
    Bin* eb = bin + BIN_CAP;
    while (bin < eb)
    {
        if (bin->a && bin->a->i < a->i) {a->neighbor(bin->a);}
        bin++;
    }
    if (bin->b)
    {
        // This bin has overflowed, so call this method again on the overflow bins.
        runNeighbors(a, bin->b);
    }
    qDebug() << "fin";
}
