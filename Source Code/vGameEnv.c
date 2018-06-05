#include "vGameEnv.h"

//#include <stdio.h>
extern struct env vm;

void vSetEnv(struct env* e){
	copyEnv(e, &vm);
}

char vPlay(int player, int col){
	return play(&vm, player, col);
}

char vIsValid(int col){
	return validate(&vm, col);
}

char vGetStatus(void){
	return getStatus(&vm);
}

void vUndo(int col){
	undoGame(&vm, col);
}
