#ifndef GLOBALS_H
#define GLOBALS_H

#include "mainwindow.h"
#include "atomeditor.h"
#include "logger.h"

#include <QObject>
#include <QList>

class Globals : public QObject
{
    Q_OBJECT
public:
    explicit Globals(QObject *parent = 0);

    static MainWindow* mw;
    static AtomEditor* ae;
    static Logger* log;
    static QSettings persist;
    static QStringList elementSymbols;
    static int numElements;

    static void init();
    static Atom* getAtom(int type);
    static const QString getElementName(const int type);
    static const int getElementType(const QString name);
    
signals:
    
public slots:
    
};

#endif // GLOBALS_H
