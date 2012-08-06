// The physical size of the bin in pixels
// 0 < BIN_PX < 2 * (rad of largest atom)
#define BIN_PX 50.0

// The number of atoms that can be contained in one bin
#define BIN_SIZE 4



#define XY_TO_BIN(x,y) binsStart + ((x) + (y) * binsX) * BIN_SIZE

#include "bins.h"

Bins::Bins() {}

void Bins::resize(int width, int height)
{
    binsX = int(width / BIN_PX);
    int binsY = int(height / BIN_PX);

    int binsLen = binsX * binsY * BIN_SIZE;
    binsStart = new Atom*[binsLen];
    binsEnd = binsStart + binsLen;

    memset(binsStart, 0, sizeof(Atom*) * binsLen);// Set all elements of binsStart to 0
}

void Bins::addAtom(Atom* a)
{
    Atom** bin = XY_TO_BIN(int(a->nx / BIN_PX), int(a->ny / BIN_PX));
    addToBin(a, bin);
    a->bin = bin;
}

void Bins::moveAtom(Atom* a)
{
    Atom** bin = XY_TO_BIN(int(a->nx / BIN_PX), int(a->ny / BIN_PX));
    Atom** oldBin = a->bin;
    if (oldBin == bin) {return;}

    if (addToBin(a, bin))
    {
        removeFromBin(a, oldBin);
        a->bin = bin;
    }
}

bool Bins::addToBin(Atom *a, Atom **bin)
{
    if (bin < binsStart || bin >= binsEnd) {return false;}

    Atom** eb = bin + BIN_SIZE;
    while (bin < eb)
    {
        if (!*bin)
        {
            *bin = a;
            return true;
        }
        bin++;
    }
    return false;
}

void Bins::removeFromBin(Atom* a, Atom** bin)
{
    Atom** eb = bin + BIN_SIZE;
    while (bin < eb)
    {
        if (*bin == a)
        {
            *bin = 0;
            return;
        }
        bin++;
    }
}

void Bins::runNeighbors(Atom* a, Atom** bin)
{
    bin -= (binsX + 1) * BIN_SIZE;
    run3Bins(a, bin);
    bin += binsX * BIN_SIZE;
    run3Bins(a, bin);
    bin += binsX * BIN_SIZE;
    run3Bins(a, bin);
}
void Bins::run3Bins(Atom* a, Atom** bin)
{
    Atom** eb = bin + BIN_SIZE * 3;
    if (bin < binsStart) {bin = binsStart;}
    else if (eb > binsEnd) {eb = binsEnd;}
    while (bin < eb)
    {
        if (*bin && (*bin)->i < a->i) {a->neighbor(*bin);}
        bin++;
    }
}
