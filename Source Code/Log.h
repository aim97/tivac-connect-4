#ifndef LOG_H
#define LOG_H

#include "UART.h"

void UART0_Init(void);
void UART0_OutChar(unsigned char);
void Log(char*,int);

#endif
