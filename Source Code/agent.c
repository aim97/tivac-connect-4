#include "agent.h"
#include "stdio.h"

#define min(a, b) ((a>b)?b:a)
#define max(a, b) ((a>b)?a:b)

#define MAX 100000

char nextMove(struct env* e){
	struct env p;
	copyEnv(e, &p);
	return getMove(&p, 8);
}

char getMove(struct env* p, int depth){
	int x, player = (p->moves & 1) + 1, bestScore, curScore, move,
		alpha = -MAX,
		beta  =  MAX;

	bestScore = -MAX;
	for(x = 0;x < p->col_size;x++){
		if(validate(p, x)){
			play(p, player, x);
			curScore  = mini_max(p, depth, 0, alpha, beta);
			undoGame(p, x);
			//printf("score of %d is %d while best score is %d\n", x, curScore, bestScore);
			if(curScore > bestScore){
				bestScore = curScore;
				alpha     = max(bestScore, alpha);
				move      = x;
				//printf("move is %d\n", move);
				if(beta <= alpha)break;
			}
		}
	}
	return move;
}


int mini_max(struct env* p, int depth, char isMax, int alpha, int beta){
	int x, status, player = (p->moves & 1) + 1, bestScore, curScore;

	// score evaluation
	status = getStatus(p);
	//printf("status %d, isMAx %d, depth %d\n", status, isMax, depth);
	if(status == P1_WON || status == P2_WON) return ((isMax)?-depth : depth);
	else if(status == TIE) return 0;

	
	// base case
	if(!depth)return 0;

	if(isMax){
		bestScore = -MAX;
		for(x = 0;x < p->col_size;x++){
			if(validate(p, x)){
				play(p, player, x);
				curScore  = mini_max(p, depth - 1, 0, alpha, beta);
				undoGame(p, x);
				bestScore = max(bestScore, curScore);
				alpha     = max(bestScore, alpha);
				if(beta <= alpha)break;
			}
		}
	}else{
		bestScore = MAX;
		for(x = 0;x < p->col_size;x++){
			if(validate(p, x)){
				play(p, player, x);
				curScore  = mini_max(p, depth - 1, 1, alpha, beta);

				//printf("board after playing in %d\n", x);
				//displayBoard(p);
				undoGame(p, x);
				//printf("undo with score %d\n", curScore);
				//displayBoard(p);

				bestScore = min(bestScore, curScore);
				beta      = min(beta, bestScore);
				if(beta <= alpha)break;
			}
		}
	}
	return bestScore;
}
