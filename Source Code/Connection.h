#ifndef CONNECTION_H
#define CONNECTION_H

void UART2_Init(void);
unsigned char inChar(void);
void outChar(unsigned char);
unsigned char WaitingInput(void);


//----Master----//
void mHandShake(void);
void mPlayerSelect(unsigned char);
void mPlay(unsigned char,unsigned char);
unsigned char mWaitSlave(void);
void mSendStatus(char);


//----Slave----//
void sHandShake(void);
unsigned char sPlaySelect(void);
unsigned char sWaitMaster(void);
unsigned char sPlay(unsigned char );

//#include "Connection.c"
#endif
