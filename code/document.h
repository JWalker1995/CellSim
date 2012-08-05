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
    ~Document();

    void addWindow();

    void open();
    void save();
    void saveAs();

    void openFile(QString path);

    QFile bridge;
    Simulation* sim;
    
private:
    QFileDialog fileDialog;

    QByteArray encodeSim();
    void decodeSim(QByteArray str);
    int encodeAtom(Atom* a, char* str);
    void decodeAtom(const char *&str);

signals:
    
public slots:
    
};

#endif // DOCUMENT_H
