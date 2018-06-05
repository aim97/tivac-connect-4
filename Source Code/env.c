#include "env.h"
//#include "stdio.h"

//extern struct env environment;

// initialize the game size
// in  : n > row size, m > col size
// out : void
void create_map(struct env* e, int n, int m){
	//printf("creating the map ...\n");
	int i, j;
	e->row_size = n;//printf("row size set to %d\n", e->row_size);
	e->col_size = m;//printf("col size set to %d\n", e->col_size);
	//e->lastGame = -1;printf("last game played is %d\n", e->lastGame);
	e->moves = 0;//printf("number of moves so far is %d\n", e->moves);
	for(i = 0 ; i < m ; ++i) e->last[i] = 0;
	for(i = 0;i < n;i++){
		for(j = 0;j < m;j++){
			e->map[i][j] = 0;
		}
	}
	//printf("map created\n");
}

// validate playing in the colPos column
// in  : colPos > the column number to play in
// out : char   > "0 : invalid move, 1 : valid move"
char validate(struct env* environment, int colPos){
	int cz = environment->col_size,
		rz = environment->row_size;
	if(colPos >= cz) return 0;
	if(environment->last[colPos] >= rz) return 0;
	return 1;
} 

// apply the move given to the given player
// in  : playerNumber > "the number of the player making the move"
//       colPos       > "the column number to play in"
// out : char         > "0 : invalid move, 1 : move confirmed"
char play(struct env* environment, int playerNumber, int colPos){
	int row = environment->last[colPos];
	if(validate(environment, colPos)){
		environment->map[row][colPos] = playerNumber;
		environment->last[colPos]++;
		environment->moves++;
		return 1;
	}else
		return 0;
}

// undo the guven move from the game
// in  : colPos       > "the column number to play in"
// out : void
void undoGame(struct env* environment, int colPos){
	int row;
	if(environment->last[colPos]){
		row = --(environment->last[colPos]);
		environment->moves--;
		environment->map[row][colPos] = 0;
	}
}


char getStatus(struct env* environment){
	int winningPlayer = 0;
		
	/*
	// checkH
	winningPlayer = checkH(environment, curRow, curCol);
	//printf("winning player is %d\n", winningPlayer);
	if(winningPlayer)return winningPlayer;
	
	// checkV
	winningPlayer = checkV(environment, curRow, curCol);
	//printf("winning player is %d\n", winningPlayer);
	if(winningPlayer)return winningPlayer;
	
	// checkD
	winningPlayer = checkD(environment, curRow, curCol);
	//printf("winning player is %d\n", winningPlayer);
	if(winningPlayer)return winningPlayer;
	*/
	winningPlayer = checkWiner(environment);
	if(winningPlayer)return winningPlayer;
	//check tie
	winningPlayer = checkTie(environment);
	//printf("winning player is %d\n", winningPlayer);
	
	if(winningPlayer)return TIE;
	return CONT; // continue
}

char checkWiner(struct env* environment){
	int i, j, k, run = 0;
	int dx[] = { 1, 0, 1, 1};
	int dy[] = {-1, 1, 0, 1};
	for(i = 0;i < environment->row_size;i++){
		for(j = 0;j < environment->col_size;j++){
			for(k = 0;k < 4;k++){
				if(environment->map[i][j]){
					run = count(environment, i, j, dx[k], dy[k], environment->map[i][j]);
					if(run >= 4) return environment->map[i][j];
				}
			}
		}
	}
	return 0;
}

// helper function : to check for a horizontal run
// in  : curRow > the current row
//       curCol > the current column
// out : char > "0 : no result(continue), otherwise : player numebr who won"
char checkH(struct env* environment, int curRow, int curCol){
	int p  = environment->map[curRow][curCol];
	int cnt = count(environment, curRow, curCol, 0, 1, p) + count(environment, curRow, curCol, 0, -1, p) + 1;
	if(cnt >= 4)return p;
	return 0;
}

// helper function : to check for a Vertical run
// in  : curRow > the current row
//       curCol > the current column
// out : char > "0 : no result(continue), otherwise : player numebr who won"
char checkV(struct env* environment, int curRow, int curCol){
	int p  = environment->map[curRow][curCol];
	int cnt = count(environment, curRow, curCol, 1, 0, p) + count(environment, curRow, curCol, -1, 0, p) + 1;
	if(cnt >= 4)return p;
	return 0;
}

// helper function : to check for a Diagonal run
// in  : curRow > the current row
//       curCol > the current column
// out : char > "0 : no result(continue), otherwise : player numebr who won"
char checkD(struct env* environment, int curRow, int curCol){
	int p  = environment->map[curRow][curCol];
	
	int cnt1, cnt2;
	cnt1 = count(environment, curRow, curCol, 1, 1, p) + count(environment, curRow, curCol, -1, -1, p) + 1;
	cnt2 = count(environment, curRow, curCol, -1, 1, p) + count(environment, curRow, curCol, 1, -1, p) + 1;
	if(cnt1 >= 4 || cnt2 >= 4)return p;
	return 0;
}

// helper function : to check for a horizontal run
// in  : void
// out : char > "0 : no result(continue), otherwise : player numebr who won"
char checkTie(struct env* environment){
	// int rz = environment->row_size,
	//     cz = environment->col_size,
	// 	i;
	// for(i = 0;i < cz;i++){
	// 	if(environment->last[i] < rz)return 0;
	// }
	// return 1;
	return environment->moves == environment->row_size * environment->col_size;
}

// helper of helper function : count the number of consequive same player
// in  : curRow > the current row
//       curCol > the current column
//       dr     > the change in row number
//       dc     > the change in column number
//       p      > the player number
// out : int    > the number of consequtive occurnaces of the same player p
int count(struct env* environment, int curRow, int curCol, int dr, int dc, int p){
	int rz = environment->row_size,
		cz = environment->col_size,
		cnt = 0;

	while(curRow < rz && curCol < cz){
		if(environment->map[curRow][curCol] == p)cnt++;
		else break;
		curRow += dr;
		curCol += dc;
	}
	return cnt;
}

// gets the environment to be sent to the AI
// in  : void
// out : struct env (the environment)
void copyEnv(struct env* e1, struct env* e2){
	int i, j;
	e2->row_size = e1->row_size;
	e2->col_size = e1->col_size;
	e2->moves    = e1->moves;
	for(i = 0;i < e1->col_size;i++)e2->last[i] = e1->last[i];
	for(i = 0;i < e1->row_size;i++){
		for(j = 0;j < e1->col_size;j++){
			e2->map[i][j] = e1->map[i][j];
		}
	}
}


/*void displayBoard(struct env* e){
	int i, j;
	printf("rz : %d, cz : %d\n", e->row_size, e->col_size);
	printf("last state : \n");
	for(i = 0;i < e->col_size;i++)printf("%d | ", e->last[i]);
	printf("\nmap state : \n");
	for(i = 0;i < e->row_size;i++){
		for(j = 0;j < e->col_size;j++){
			printf("%d | ", e->map[i][j]);
		}
		printf("\n");
	}
}*/
