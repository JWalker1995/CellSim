#ifndef LOGGER_H
#define LOGGER_H

#include "logblock.h"

#include <QFile>

class Logger : public QFile
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = 0);
    ~Logger();

    class LogBlock scopedBlock(const QString str);
    void startBlock(const QString str);
    void endBlock();

    void event(const QString str);
    void data(const QString str);
    void warning(const QString str);
    void error(const QString str);
    void fatal(const QString str);
    // void flush(); // Writes all logs.

    void rawLog(const QString str);

private:
    int ind;

signals:
    
public slots:
    
};

#endif // LOGGER_H
