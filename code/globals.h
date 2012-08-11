#ifndef GLOBALS_H
#define GLOBALS_H

#include "mainwindow.h"
#include "atomeditor.h"
#include "dnaeditor.h"
#include "logger.h"

#include <QObject>

class Globals : public QObject
{
    Q_OBJECT
public:
    explicit Globals(QObject *parent = 0);

    static MainWindow* mw;
    static AtomEditor* ae;
    static DnaEditor* de;
    static Logger* log;
    static QStringList elementSymbols;
    static int numElements;

    static int compNums[190];

    static void init();
    static Atom* getAtom(int type);
    static const QString getElementName(const int type);
    static const int getElementType(const QString name);
    
signals:
    
public slots:
    
};

#endif // GLOBALS_H
