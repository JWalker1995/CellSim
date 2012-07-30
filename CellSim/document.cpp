#include "document.h"
#include "simview.h"
#include "globals.h"

#include <QString>
#include <QDebug>

Document::Document(QWidget *parent) :
    QMdiArea(parent)
{
    sim = new Simulation(this);
    addSubWindow(new SimView(sim, this));
}

void Document::save()
{
    QByteArray comp = encodeSim();
    qDebug() << comp.length();
    qDebug() << comp;
}

QByteArray Document::encodeSim()
{
    char* str;

    int j = 0;
    while (j < sim->numAtoms)
    {
        encodeAtom(sim->atoms[j], str, i);
        j++;
    }

    QByteArray comp = qCompress(QByteArray(str, i), 9);

    return comp;
}

void Document::decodeSim(QByteArray str)
{
    sim = new Simulation(this);
}

void Document::encodeAtom(Atom* a, char* str)
{
    // Selected * type (1 char)
    // nx (4 chars)
    // ny (4 chars)
    // vx (4 chars)
    // vy (4 chars)
    // state (2 chars)
    // reactionStr (variable length)
    // bondsLt (variable length)

    *(str++) = char(a->element * 2 + a->selected);

    float nx = float(a->nx);
    memcpy(str += 4, nx, 4);
    float ny = float(a->ny);
    memcpy(str += 4, ny, 4);

    float vx = float(a->vx);
    memcpy(str += 4, vx, 4);
    float vy = float(a->vy);
    memcpy(str += 4, vy, 4);

    memcpy(str += 2, a->state, 2);

    int len = a->reactionStr.length();
    *(str++) = char(len);
    memcpy(str += len, a->reactionStr, len);

    int maxBond = 0;
    int j = 0;
    while (j < a->numBondsLt)
    {
        int bond = a->bondsLt[j];
        if (bond > maxBond) { maxBond = bond; }
        j++;
    }

    int bondSize = int(log(maxBond) / 2.40823);
    *(str++) = char(a->numBondsLt * 4 + bondSize);
    j = 0;
    while (j < a->numBondsLt)
    {
        memcpy(str += bondSize, a->bondsLt[j], bondSize);
    }
}

void Document::decodeAtom(char* str)
{
    bool selected = bool(*str % 2);
    int element = int(*(str++) / 2);

    float nx;
    memcpy(&nx, str += 4, 4);
    float ny;
    memcpy(&ny, str += 4, 4);

    float vx;
    memcpy(&vx, str += 4, 4);
    float vy;
    memcpy(&vy, str += 4, 4);

    int state;
    memcpy(&state, str += 2, 2);

    int maxBond = sim->numAtoms - 1;

    Atom* a = sim->addAtom(QPointF(qreal(nx), ny), element, state);

    a->vx = vx;
    a->vy = vy;
    if (selected) { a->select(); }

    int len = int(str[i++]);
    Globals::ae->reactionStrToArr(QByteArray(str[i += len], len));

    int numBonds = int(*str / 4);
    int lenBonds = *(str++) % 4;
    int i = 0;
    while (i < numBonds)
    {
        int bi;
        memcpy(&bi, str += lenBonds, lenBonds);

        if (Q_UNLIKELY(bi > maxBond)) {return;}
        Atom* b = sim->atoms[bi];
        if (Q_LIKELY(a->numBondsLt < 6 && b->numBondsGt < 6))
        {
            a->bondsLt[a->numBondsLt++] = b;
            b->bondsGt[b->numBondsGt++] = a;
        }
    }
}
