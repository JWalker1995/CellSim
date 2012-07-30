#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "simulation.h"

#include <QFile>

class FileManager : public QFile
{
    Q_OBJECT
public:
    explicit FileManager(QObject *parent = 0);

    void saveSim(Simulation* sim);
    
signals:
    
public slots:
    
};

#endif // FILEMANAGER_H
