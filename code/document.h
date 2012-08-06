#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "simulation.h"

#include <QGraphicsView>
#include <QFileDialog>

class Document : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Document(QWidget *parent = 0);
    ~Document();

    void open();
    void save();
    void saveAs();

    void openFile(QString path);

    void setModified();
    void setSaved();
    bool confirmClose();

    QFile bridge;
    Simulation* sim;
    
private:
    bool modified;

    QFileDialog fileDialog;

    QByteArray encodeSim();
    void decodeSim(QByteArray str);
    int encodeAtom(Atom* a, char* str);
    void decodeAtom(const char *&str);

signals:
    
public slots:
    
};

#endif // DOCUMENT_H
