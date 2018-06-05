#ifndef CONTROLLER_H
#define CONTROLLER_H

//#include "Texas.h"

#include "Log.h"
#include "Connection.h"

#include "Display.h"
#include "Random.h"

#include "env.h"
#include "GameEnv.h"
#include "agent.h"

#include "tm4c123gh6pm.h"


#define num_rows 6
#define num_cols 7
#define changingSwitch 2
#define okSwitch 1

// calls all the needed intializations
void initializeAll(void);

// calls the splash
void start(void);

// home screen, mode selection
void loadHome(void);

// player selection screen (first or second)
void loadSelectPlayer(char isSingle);

// single player board, plays against AI
void loadSingleBoard(void);

// end screen, tells if the user wins or loses
// status: 21 -> player wins, 22 -> player loses, 23 -> tie
void loadEnd(char status);

// navigate to multi player screen
void loadMultiPlayer(void);

// slave waiting for connection from master
void loadSlaveWainting(void);

// shows the position of the user in case of being slave device (first, second)
void loadSlaveTurn(char userPosition);

// chooses whether the multi-player plays as AI or player
void loadMutliOptions(void);

// game board in case of multi player, plays against another user
void loadMultiBoard(char isMaster);

void PortF_Init(void);

// waits for input on buttons, returns 1 or 2 -> the pressed switch
char getButtonsInput(void);

// make a delay with the given time
void delay(unsigned long timeMillis);

// adapter from conroller format to communication format
char getCommunicationStatus(char status);

#endif
