#include "GameEnv.h"
//#include <stdio.h>

struct env environment;

void startGame(int rz, int cz){
	create_map(&environment, rz, cz);
}

char Play(int player, int col){
	return play(&environment, player, col);
}

char GetStatus(void){
	return getStatus(&environment);
}

struct env* GetEnvironment(void){
	return &environment;
}

char isValid(int col){
	return validate(&environment, col);
}

char** GetGame(void){
	return (char **)environment.map;
}
