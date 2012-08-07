#include "document.h"
#include "globals.h"

#include "math.h"

#include <QFileDialog>
#include <QFile>
#include <QString>
#include <QDebug>

Document::Document(QWidget *parent) :
    QGraphicsView(parent)
{
    setOptimizationFlag(QGraphicsView::DontSavePainterState);
    //setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    sim = new Simulation(this);
    setScene(sim);

    modified = true;
    setSaved();
}

Document::~Document()
{
    delete sim;
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

    setSaved();
}

void Document::saveAs()
{
    fileDialog.setFileMode(QFileDialog::AnyFile);
    if (fileDialog.exec())
    {
        bridge.setFileName(fileDialog.selectedFiles().first());
    }

    bridge.open(QFile::WriteOnly);
    bridge.write(encodeSim());
    bridge.close();

    setSaved();
}

void Document::openFile(QString path)
{
    bridge.setFileName(path);
    bridge.open(QFile::ReadOnly);
    decodeSim(bridge.readAll());
    bridge.close();
}

void Document::setModified()
{
    if (modified) {return;}
    QFileInfo file = QFileInfo(bridge.fileName());
    Globals::mw->setDocTitle(i, file.fileName() + "*");

    modified = true;
}
void Document::setSaved()
{
    if (!modified) {return;}
    QFileInfo file = QFileInfo(bridge.fileName());
    Globals::mw->setDocTitle(i, file.fileName());

    modified = false;
}

bool Document::confirmClose()
{
    if (!modified) {return true;}

    QMessageBox msgBox;
    msgBox.setText("This simulation has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Save) {save();}
    else if (ret == QMessageBox::Cancel) {return false;}
    return true;
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
    sim->reset();

    str = qUncompress(str);
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

    //*str = char(a->element * 2 + a->selected);
    *str = char(a->element * 2);
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

    int maxBond = 0;
    int i = 0;
    while (i < a->numBondsLt)
    {
        int bond = a->bondsLt[i]->i;
        if (bond > maxBond) { maxBond = bond; }
        i++;
    }

    int bondSize;
    if (maxBond < 256) {bondSize = 1;}
    else if (maxBond < 256 * 256) {bondSize = 2;}
    else {bondSize = 4;}

    *str = char(a->numBondsLt * 4 + bondSize);
    str++;
    i = 0;
    while (i < a->numBondsLt)
    {
        switch (bondSize)
        {
            case 1:
            {
                char j = char(a->bondsLt[i]->i);
                memcpy(str, &j, 1);
            } break;
            case 2:
            {
                short j = short(a->bondsLt[i]->i);
                memcpy(str, &j, 2);
            } break;
            case 4:
            {
                int j = int(a->bondsLt[i]->i);
                memcpy(str, &j, 4);
            } break;
        }

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

    int maxBond = sim->numAtoms;

    Atom* a = sim->addAtom(QPointF(qreal(nx), qreal(ny)), element, state);

    a->vx = vx;
    a->vy = vy;
    if (selected)
    {
        if (a->select())
        {
            sim->selected.append(a);
            Globals::ae->updateAdd();
        }
    }

    int len = int(*str);
    str++;
    Globals::ae->reactionStrToArr(QString(QByteArray(str, len)), a->reaction);
    str += len;

    int numBonds = int(*str / 4);
    int lenBonds = *str % 4;
    str++;
    int i = 0;
    while (i < numBonds)
    {
        int bi;
        switch (lenBonds)
        {
            case 1:
            {
                char cbi;
                memcpy(&cbi, str, 1);
                bi = int(cbi);
            } break;
            case 2:
            {
                short cbi;
                memcpy(&cbi, str, 2);
                bi = int(cbi);
            } break;
            case 4:
            {
                int cbi;
                memcpy(&cbi, str, 4);
                bi = int(cbi);
            } break;
        }

        str += lenBonds;

        if (bi < maxBond)
        {
            Atom* b = sim->atoms[bi];
            if (a->numBondsLt < 6 && b->numBondsGt < 6)
            {
                a->bondsLt[a->numBondsLt++] = b;
                b->bondsGt[b->numBondsGt++] = a;
            }
        }
        i++;
    }
}
