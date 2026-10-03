#include "Debug.h"

void debug(const char *dbgMessage, Communication communication)
{
    communication.transmitString(dbgMessage);
}