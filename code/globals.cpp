#include "globals.h"

#include "elements/membrane.h"
#include "elements/acid.h"
#include "elements/carbon.h"
#include "elements/ribosome.h"
#include "elements/neuron.h"
#include "elements/gold.h"
#include "elements/uranium.h"
#include "elements/dna.h"
#include "elements/beta.h"
#include "elements/delta.h"
#include "elements/sugar.h"
#include "elements/motorneuron.h"
#include "elements/atp.h"
#include "elements/mitochondrion.h"
#include "elements/enzyme.h"

Globals::Globals(QObject *parent) :
    QObject(parent)
{
}

MainWindow* Globals::mw;
AtomEditor* Globals::ae;
Logger* Globals::log;
QStringList Globals::elementSymbols;
int Globals::numElements;

void Globals::init()
{
    // Calculate element symbols
    int i = 0;
    Atom* a;
    while ((a = getAtom(i)))
    {
        Globals::elementSymbols.append(a->symbol);
        delete a;
        i++;
    }
    Globals::numElements = i;
}

Atom* Globals::getAtom(int type)
{
    Atom* a;
    switch (type)
    {
    case 0: a = new Membrane(); break;
    case 1: a = new Carbon(); break;
    case 2: a = new Beta(); break;
    case 3: a = new Delta(); break;
    case 4: a = new Neuron(); break;
    case 5: a = new Motorneuron(); break;
    case 6: a = new Mitochondrion(); break;
    case 7: a = new Atp(); break;
    case 8: a = new Dna(); break;
    case 9: a = new Ribosome(); break;
    case 10: a = new Enzyme(); break;
    case 11: a = new Acid(); break;
    case 12: a = new Uranium(); break;
    case 13: a = new Sugar(); break;
    case 14: a = new Gold(); break;
    default: return 0;
    }
    a->element = type;
    return a;
}

const QString Globals::getElementName(const int type)
{
    switch (type)
    {
    case 0: return "Membrane"; break;
    case 1: return "Carbon"; break;
    case 2: return "Beta"; break;
    case 3: return "Delta"; break;
    case 4: return "Neuron"; break;
    case 5: return "Motorneuron"; break;
    case 6: return "Mitochondrion"; break;
    case 7: return "Atp"; break;
    case 8: return "Dna"; break;
    case 9: return "Ribosome"; break;
    case 10: return "Enzyme"; break;
    case 11: return "Acid"; break;
    case 12: return "Uranium"; break;
    case 13: return "Sugar"; break;
    case 14: return "Gold"; break;
    default: return "";
    }
}

const int Globals::getElementType(const QString name)
{
    int i = 0;
    while (true)
    {
        if (Globals::getElementName(i) == name)
        {
            return i;
        }
        i++;
    }
}
