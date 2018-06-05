#ifndef CONTROLLER_C
#define CONTROLLER_C

#include "controller.h"

char myPosition, otherPosition;
char lastPos;
char isMultiAI;

// calls all the needed intializations
void initializeAll(){
	PortF_Init();
	UART2_Init();
	//UART0_Init();
	Nokia5110_Init();
	Random_Init(1);
}

// in main do:
// start();
// while(1) loadHome();

// calls the splash
void start(){
	displaySplash();
	delay(20);
}

// home screen, mode selection
void loadHome(){
	char selection = 0, input;
	while (1) {
		displayHome(selection);
		input = getButtonsInput();
		if(input == changingSwitch) {
			selection = (selection + 1) % 2;
		} else if (input == okSwitch) {
			if(selection == 0) {
				loadSelectPlayer(1);
			} else {
				loadMutliOptions();
				//loadMultiPlayer();
			}
			break;
		}
	}
}

// player selection screen (first or second)
void loadSelectPlayer(char isSingle){
	char selection = 0, input;	// selection: 0 -> first player, 1 -> second player
	while (1) {
		displayPlayer(selection);
		input = getButtonsInput();
		if(input == changingSwitch) {
			selection = (selection + 1) % 2;
		} else if (input == okSwitch) {
			myPosition = selection + 1;
			otherPosition = (selection + 1) % 2 + 1;
			if(isSingle) {
				loadSingleBoard();
			} else {
				mPlayerSelect(selection + 31);
				loadMultiBoard(1);
			}
			break;
		}
	}
}

// end screen, tells if the user wins or loses
// status: 1 -> player wins, 2 -> player loses, 3 -> tie
void loadEnd(char status){
	
	//show the last game for 2 seconds, then show the end maessage
	displayBoard((char(*)[10])GetGame(), -1, myPosition);
	delay(80);

	displayGameEnd(status);
	getButtonsInput();
}

// navigate to multi player screen
void loadMultiPlayer(){
	char selection = 0, input;
	while (1) {
		displayMulti(selection);
		input = getButtonsInput();
		if(input == changingSwitch) {
			selection = (selection + 1) % 2;
		} else if (input == okSwitch) {
			if(!selection) {
				//selected master
				mHandShake();
				loadSelectPlayer(0);
			} else {
				// selected slave
				loadSlaveWainting();
			}
			break;
		}
	}
}

// slave waiting for connection from master
void loadSlaveWainting(){
	char position;

	waiting(0);	// show waiting connection
	sHandShake();

	waiting(1); // show waiting player selection
	position = sPlaySelect();

	myPosition = position;
	otherPosition = position ^ 3;

	loadSlaveTurn(position);
}

// shows the position of the user in case of being slave device (first, second)
void loadSlaveTurn(char userPosition){
	slaveTurn(userPosition - 1);
	getButtonsInput();
}

char playPlayerTurn(){
	char selection = lastPos, input;
	while (1) {
		displayBoard((char(*)[10])GetGame(), selection, myPosition);
		input = getButtonsInput();
		if(input == changingSwitch) {
			selection = (selection + 1) % num_cols;
			lastPos = selection;
		} else if (input == okSwitch && isValid(selection)) {
			Play(myPosition, selection);
			return selection;
		}
	}
}

void loadMutliOptions(){
	char selection = 0, input;
	isMultiAI = 0;

	while (1) {
		displayMultiOptions(selection);
		input = getButtonsInput();
		if(input == changingSwitch) {
			selection = (selection + 1) % 2;
		} else if (input == okSwitch) {
			isMultiAI = !selection;
			loadMultiPlayer();
			break;
		}
	}
	
}

// single player board, plays against AI
void loadSingleBoard(){
	char currentPos = 1, status;

	startGame(num_rows, num_cols);

	while(1){
		if(currentPos == myPosition){
			playPlayerTurn();
		}else{
			Play(otherPosition, nextMove(GetEnvironment()));
		}
		displayBoard((char(*)[10])GetGame(), -1, myPosition);

		status = getCommunicationStatus(GetStatus());

		if(status != 20){
			loadEnd(status);
			return;
		}

		currentPos ^= 3;
	}

}

// game board in case of multi player, plays against another user
void loadMultiBoard(char isMaster){
	char currentPos = 1, col, status, data;

	startGame(num_rows, num_cols);

	while(1) {	
		if(currentPos == myPosition) {
			if(isMultiAI){
				col = nextMove(GetEnvironment());
				Play(myPosition, col);
			} else  {
				col = playPlayerTurn();
			}
			
			if(isMaster) {
				status = getCommunicationStatus(GetStatus());
				mPlay(col, status);
			} else {
				status = sPlay(col);
			}
		} else {
			if(isMaster){
				col = mWaitSlave();
				if(!isValid(col)){
					status = -1;
					currentPos ^= 3;
				}else{ 
					Play(otherPosition, col);
					status = GetStatus();
				}
				status = getCommunicationStatus(status);
				mSendStatus(status);
			} else {
				data = sWaitMaster();
				status = (data & 0x0F) + 20;
				col = (data & 0xF0) >> 4;
				Play(otherPosition, col);
			}
		}

		displayBoard((char(*)[10])GetGame(), -1, myPosition);

		if(status != 20) {
			loadEnd(status);
			return;
		}

		currentPos ^= 3;

	}

}

void PortF_Init(void){
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
	delay = SYSCTL_RCGC2_R;           // delay   
	GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
	GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
	GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
	GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
	GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
	GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
	GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
	GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}

// waits for input on buttons, returns 1 or 2 -> the pressed switch
char getButtonsInput(){
	char sw1, sw2;
	while(GPIO_PORTF_DATA_R & 0x10 && GPIO_PORTF_DATA_R & 0x01);
	sw1 = !(GPIO_PORTF_DATA_R & 0x10);
	sw2 = !(GPIO_PORTF_DATA_R & 0x01);
	delay(10);
	if(sw1) return okSwitch;
	if(sw2) return changingSwitch;
	return -1;
}

// make a delay with the given time
void delay(unsigned long timeMillis){
	unsigned long volatile time = timeMillis * 128897;
	while(time) time--;
}

// adapter from conroller format to communication format
char getCommunicationStatus(char status){
	if(!status) return 20;
	if(status == 3) return 23;
	if(status == -1) return 24;
	if(status == myPosition) return 21;
	return 22;
}

#endif
