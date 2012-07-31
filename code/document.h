#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "simulation.h"

#include <QFileDialog>
#include <QMdiArea>

class Document : public QMdiArea
{
    Q_OBJECT
public:
    explicit Document(QWidget *parent = 0);

    void open();
    void save();
    void saveAs();

    void openFile(QString path);

    Simulation* sim;
    
private:
    QFileDialog fileDialog;
    QFile bridge;

    QByteArray encodeSim();
    void decodeSim(QByteArray str);
    int encodeAtom(Atom* a, char* str);
    void decodeAtom(const char *&str);

signals:
    
public slots:
    
};

#endif // DOCUMENT_H
