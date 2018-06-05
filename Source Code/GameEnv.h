#ifndef GAMEENV_H
#define GAMEENV_H

#include "env.h"

/**
it's used to reset game board
in  : size of board
out : void
*/
void startGame(int rz, int cz);


/**
it's used to apply a specific game to the game board
in  : the player to play, and the column he playes in
out : 1 SUCCESS, 0 INVALID MOVE
*/
char Play(int player, int col);

/**
it's used to get the current status of the game
in  : void
out : 0  > "continue"
      1  > "P1"
      2  > "P2"
      3  > "TIE"
*/
char GetStatus(void);

/**
it's used to get the environment
in  : void
out : pointer to the envionment
*/
struct env* GetEnvironment(void);


/**
this function is used to check if playing in a specific position is valid
in  : column number to play in
out : 0> INVALID , 1> VALID
*/
char isValid(int col);

/**
this function returns a pointer to the map
in  : void
out : char** the map
*/
char** GetGame(void);

#endif
