#ifndef VGAMEENV_H
#define VGAMEENV_H

#include "env.h"

/**
it's sets the environment of the AI agent with the game environment
in  : pointer to actual game board
out : void
*/
void vSetEnv(struct env* e);
/**
it applies some game for some player in the game
in  : the player who plays it, and at at which column
out : 0 > MOVE SUCCESS, 1 > MOVE FAILED
*/
char vPlay(int palyer, int col);
/**
it checks if a specific game is valid
in  : the column to check if we can play in
out : 0 > VALID, 1 > INVALID
*/
char vIsValid(int col);
/**
it gets the status of the game
in  : void
out : 0  > "continue"
      1  > "P1"
      2  > "P2"
      3  > "TIE"
*/
char vGetStatus(void);
/**
undo the latest move in a give column
in  : the column number
out : void
*/
void vUndo(int col);

#endif
