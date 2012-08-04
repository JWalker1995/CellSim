#ifndef LOGBLOCK_H
#define LOGBLOCK_H

#include "logger.h"

class LogBlock
{
public:
    LogBlock(class Logger* _log);
    ~LogBlock();

    void end();

private:
    Logger* log;

};

#endif // LOGBLOCK_H
