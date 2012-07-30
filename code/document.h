#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "simulation.h"

#include <QMdiArea>

class Document : public QMdiArea
{
    Q_OBJECT
public:
    explicit Document(QWidget *parent = 0);

    void save();

    Simulation* sim;
    
private:
    QByteArray encodeSim();
    void decodeSim(QByteArray str);
    int encodeAtom(Atom* a, char* str);
    void decodeAtom(char* str);

signals:
    
public slots:
    
};

#endif // DOCUMENT_H
