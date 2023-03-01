#include "logger.h"

Logger::Logger()
{
}

void Logger::stop()
{
    closeLogFile();
}
