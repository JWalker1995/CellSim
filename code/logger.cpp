#include "logger.h"

Logger::Logger(QObject *parent) :
    QFile(parent)
{
    setFileName("log.txt");
    open(QIODevice::Append);
}

Logger::~Logger()
{
    close();
}

LogBlock Logger::scopedBlock(const QString str)
{
    startBlock(str);
    return LogBlock(this);
}

void Logger::startBlock(const QString str)
{
    rawLog(str);
    ind++;
}

void Logger::endBlock()
{
    ind--;
    rawLog("finished");
}

void Logger::event(const QString str)
{
    rawLog("Event: " + str);
}

void Logger::data(const QString str)
{
    rawLog("Data: " + str);
}

void Logger::warning(const QString str)
{
    rawLog("Warning: " + str);
}

void Logger::error(const QString str)
{
    rawLog("Error: " + str);
}

void Logger::fatal(const QString str)
{
    rawLog("Fatal: " + str);
}

void Logger::rawLog(const QString str)
{
    write((QString("    ").repeated(ind) + str + "\n").toAscii());
    flush();
}
