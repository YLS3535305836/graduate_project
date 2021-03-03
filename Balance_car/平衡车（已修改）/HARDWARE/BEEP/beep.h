#ifndef __BEEP_H
#define __BEEP_H
#include "sys.h"

#define BEEP PAout(12)

void BEEP_Init(void);
void BEEP_Flash(u16 time);

#endif
