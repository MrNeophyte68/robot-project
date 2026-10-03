/* Identification:

Travail : TRAVAIL_PRATIQUE_7
Section # : 3
Équipe # : 6467
Correcteur : Paul Petibon

Description du programme:

Le programme ci-dessous permet de déboguer un programme à l'aide de RS232.
*/

#include "Communication.h"

#ifdef DEBUG
#define DEBUG_PRINT(x, com) debug (x, com) 
#else
#define DEBUG_PRINT(x, com) do {} while (0) 

void debug(const char *dbgMessage, Communication communication);

#endif