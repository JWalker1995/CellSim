#include "logblock.h"

LogBlock::LogBlock(Logger* _log)
{
    log = _log;
}

LogBlock::~LogBlock()
{
    end();
}

void LogBlock::end()
{
    log->endBlock();
}
