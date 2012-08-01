#include "document.h"
#include "simview.h"
#include "globals.h"

#include "math.h"

#include <QFileDialog>
#include <QFile>
#include <QString>
#include <QDebug>

Document::Document(QWidget *parent) :
    QMdiArea(parent)
{
    sim = new Simulation(this);
    addWindow();
}

void Document::addWindow()
{
    QMdiSubWindow* w = addSubWindow(new SimView(sim, this));
    w->setAttribute(Qt::WA_DeleteOnClose, true);
    w->showMaximized();
}

void Document::open()
{
    fileDialog.setFileMode(QFileDialog::ExistingFiles);
    if (fileDialog.exec())
    {
        QStringList files = fileDialog.selectedFiles();
        openFile(files.first());
        int i = 1;
        int c = files.length();
        while (i < c)
        {
            Globals::mw->openDoc(files[i]);
            i++;
        }
    }
}

void Document::save()
{
    if (bridge.fileName().isEmpty())
    {
        fileDialog.setFileMode(QFileDialog::AnyFile);
        if (fileDialog.exec())
        {
            bridge.setFileName(fileDialog.selectedFiles().first());
        }
    }

    bridge.open(QFile::WriteOnly);
    bridge.write(encodeSim());
    bridge.close();
}

void Document::saveAs()
{

}

void Document::openFile(QString path)
{
    bridge.setFileName(path);
    bridge.open(QFile::ReadOnly);
    decodeSim(bridge.readAll());
    bridge.close();
}

QByteArray Document::encodeSim()
{
    QByteArray str = QByteArray();

    char atomStr[128];
    int i = 0;
    while (i < sim->numAtoms)
    {
        int len = encodeAtom(sim->atoms[i], atomStr);
        str.append(atomStr, len);
        i++;
    }

    str = qCompress(str, 9);

    return str;
}

void Document::decodeSim(QByteArray str)
{
    str = qUncompress(str);
    qDebug() << QString(str.toHex());
    int len = str.length();
    str.append(QString(128, ' '));

    const char* cStr = str.data();
    const char* cStrEnd = cStr + len;

    while (cStr < cStrEnd)
    {
        decodeAtom(cStr);
    }
}

int Document::encodeAtom(Atom* a, char* str)
{
    // Selected * type (1 char)
    // nx (4 chars)
    // ny (4 chars)
    // vx (4 chars)
    // vy (4 chars)
    // state (2 chars)
    // reactionStr (variable length)
    // bondsLt (variable length)

    *str = char(a->element * 2 + a->selected);
    str++;

    float nx = float(a->nx);
    memcpy(str, &nx, 4);
    str += 4;
    float ny = float(a->ny);
    memcpy(str, &ny, 4);
    str += 4;

    float vx = float(a->vx);
    memcpy(str, &vx, 4);
    str += 4;
    float vy = float(a->vy);
    memcpy(str, &vy, 4);
    str += 4;

    memcpy(str, &a->state, 2);
    str += 2;

    int len = a->reactionStr.length();
    *str = char(len);
    str++;
    memcpy(str, a->reactionStr.toAscii().data(), len);
    str += len;

    int maxBond = 2;
    int i = 0;
    while (i < a->numBondsLt)
    {
        int bond = a->bondsLt[i]->i;
        if (bond > maxBond) { maxBond = bond; }
        i++;
    }

    int bondSize = int(ceil(log(maxBond) / 2.40823));
    *str = char(a->numBondsLt * 4 + bondSize);
    str++;
    i = 0;
    while (i < a->numBondsLt)
    {
        memcpy(str, &a->bondsLt[i]->i, bondSize);
        str += bondSize;
        i++;
    }

    return 21 + len + bondSize * a->numBondsLt;
}

void Document::decodeAtom(const char *&str)
{
    bool selected = bool(*str % 2);
    int element = int(*str / 2);
    str++;
    //if (element < 0 || element >= Globals::numElements) {return;}

    float nx;
    memcpy(&nx, str, 4);
    str += 4;
    float ny;
    memcpy(&ny, str, 4);
    str += 4;

    float vx;
    memcpy(&vx, str, 4);
    str += 4;
    float vy;
    memcpy(&vy, str, 4);
    str += 4;

    unsigned short state;
    memcpy(&state, str, 2);
    str += 2;

    int maxBond = sim->numAtoms - 1;

    Atom* a = sim->addAtom(QPointF(qreal(nx), qreal(ny)), element, state);

    a->vx = vx;
    a->vy = vy;
    if (selected) { a->select(); }

    int len = int(*str);
    str++;
    Globals::ae->reactionStrToArr(QString(QByteArray(str, len)), a->reaction);
    str += len;

    int numBonds = int(*str / 4);
    int lenBonds = *str % 4;
    qDebug() << numBonds << lenBonds;
    str++;
    int i = 0;
    while (i < numBonds)
    {
        int bi;
        memcpy(&bi, str, lenBonds);
        str += lenBonds;

        if (bi > maxBond) {return;}
        Atom* b = sim->atoms[bi];
        if (a->numBondsLt < 6 && b->numBondsGt < 6)
        {
            a->bondsLt[a->numBondsLt++] = b;
            b->bondsGt[b->numBondsGt++] = a;
        }
        i++;
    }
}
