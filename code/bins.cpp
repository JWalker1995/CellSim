// 0 < BIN_PX < 2 * (rad of largest atom)

#define BIN_PX 50
#define BIN_SIZE 4
#define BIN_CAP BIN_SIZE - 1
#define BIN_OVERFLOW 0.1

#define GET_CELL(x,y) (int(x / BIN_SIZE) + int(y / BIN_SIZE) * binsX)

#include "bins.h"
#include "math.h"

Bins::Bins(int width, int height)
{
    binsX = width / BIN_PX;
    binsY = height / BIN_PX;

    int binsLen = binsX * binsY * BIN_SIZE;
    int overflowLen = int(binsLen * BIN_OVERFLOW / BIN_SIZE) * BIN_SIZE;
    bins = new void*[binsLen];
    overflowStart = new void*[overflowLen];
    overflowCur = overflowStart;
    overflowEnd = overflowStart + overflowLen;
}

void Bins::addAtom(Atom* a)
{
    int bin = GET_CELL(a->nx, a->ny);
    addToBin(bins + bin, a);
}

void Bins::moveAtom(Atom* a)
{
    int bin = GET_CELL(a->nx, a->ny);
    if (bin != a->bin)
    {
        //bins[a->bin]
    }
}

void Bins::addToBin(void **bin, Atom *a)
{
    /*
    void** eb = bin + BIN_CAP;
    while (bin < eb)
    {
        if (bin) {bin = a;return;}
        bin++;
    }
    if (!bin)
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
    */
}

void Bins::removeFromBin(void **bin)
{
    bin = 0;
}
