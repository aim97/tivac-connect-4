#ifndef CONNECTION_C
#define CONNECTION_C

#include "Connection.h"
#include "tm4c123gh6pm.h"
//#include "UART.h"

//------------UART_Init------------//
void UART2_Init(void){
  SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART2; // activate UART2
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOD; // activate port D
  UART2_CTL_R &= ~UART_CTL_UARTEN;      // disable UART
  UART2_IBRD_R = 43;                    // IBRD = int(80,000,000 / (16 * 115200)) = int(43.402778)
  UART2_FBRD_R = 26;                    // FBRD = round(0.402778 * 64) = 26
                                        // 8 bit word length (no parity bits, one stop bit, FIFOs)
  UART2_LCRH_R = 0x70; /*(UART_LCRH_WLEN_8|UART_LCRH_FEN);*/
  UART2_CTL_R |= UART_CTL_UARTEN;       // enable UART
  GPIO_PORTD_AFSEL_R |= 0xc0;           // enable alt funct on PD6,PD7 12==1100 0000
  GPIO_PORTD_DEN_R |= 0xc0;             // enable digital I/O on PD6,Pd7
  //GPIO_PORTD_PCTL_R |=0xc0;
  GPIO_PORTD_PCTL_R = 0x11000000;
  GPIO_PORTD_AMSEL_R &= ~0xc0;          // disable analog functionality on PD6,PD7
}

//------------inChar------------//
unsigned char inChar(void){
	while((UART2_FR_R & UART_FR_RXFE) == 1);
	return((unsigned char)(UART2_DR_R & 0xFF));
}
//------------outChar------------//
void outChar(unsigned char data){
	while((UART2_FR_R & UART_FR_TXFF) == 1);
	UART2_DR_R = data;
}

//-----------Waiting 0.5 sec For any input -------------//
unsigned char WaitingInput(/*unsigned long msec*/){
	unsigned long volatile time = 10 * 128897;
	while(time){
		time--;
		if((UART2_FR_R & UART_FR_RXFE) == 0)
			return((unsigned char)(UART2_DR_R & 0xFF));
	}
	return 255;
}


//-----------Master----------// 

void mHandShake(void){
	do{
		outChar(17);
    }while(WaitingInput() != 200);
}

void mPlayerSelect(unsigned char Player){
	do{
		outChar(Player);
    }while(WaitingInput() != 200);
}

void mPlay(unsigned char masterPos ,unsigned char Status){
	do{
		outChar(masterPos);
		outChar(Status);
	}while(WaitingInput() != 200);
}

unsigned char mWaitSlave(){
	return inChar();
}

void mSendStatus(char Status){
  do{
  	outChar(Status);
  }while(WaitingInput() != 200);
}

//------------Slave--------------//

void sHandShake(){
	while(inChar() != 17);
	outChar(200);
}

unsigned char sPlaySelect(){
	unsigned char in;
	in = inChar();
	outChar(200);
	if(in == 31) return 2;
	if(in == 32) return 1;
	return 255;
}

unsigned char sWaitMaster(){
	unsigned char masterPos, status, value;
	masterPos = inChar();
	status = inChar();
	outChar(200);
	value = status - 20;
	value |= (masterPos << 4);
	return value;
}

unsigned char sPlay(unsigned char slavePos){
	unsigned char status ;
	outChar(slavePos);
	status = inChar();
	outChar(200);
	return status ;
}

#endif
