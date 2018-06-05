#ifndef ENV_H
#define ENV_H

#define CONT    0
#define P1_WON  1
#define P2_WON  2
#define TIE     3

struct env{
	int row_size;
	int col_size;
	int moves;
	char last[10];
	char map[10][10];
};

// initialize the game size
// in  : n > row size, m > col size
// out : void
void create_map(struct env* environment, int n, int m);

// validate playing in the colPos column
// in  : colPos > the column number to play in
// out : char   > "0 : invalid move, 1 : valid move"
char validate(struct env* environment, int colPos);

// apply the move given to the given player
// in  : playerNumber > "the number of the player making the move"
//       colPos       > "the column number to play in"
// out : char         > "0 : invalid move, 1 : move confirmed"
char play(struct env* environment, int playerNumber, int colPos);

// undo the guven move from the game
// in  : colPos       > "the column number to play in"
// out : void
void undoGame(struct env* environment, int colPos);

// gets the environment to be sent to the AI
// in  : void
// out : struct env (the environment)
void copyEnv(struct env* e1, struct env* e2);

// returns the cur satus of the game
// in  : void
// out : 0  > "continue"
//       1  > "P1"
//       2  > "P2"
//       3  > "TIE"
char getStatus(struct env* environment);

// helper functions

// helper function : to check for a horizontal run
// in  : void
// out : char > "0 : no result(continue), otherwise : player numebr who won"
char checkH(struct env* environment, int curRow, int curCol);

// helper function : to check for a Vertical run
// in  : void
// out : char > "0 : no result(continue), otherwise : player numebr who won"
char checkV(struct env* environment, int curRow, int curCol);

// helper function : to check for a Diagonal run
// in  : void
// out : char > "0 : no result(continue), otherwise : player numebr who won"
char checkD(struct env* environment, int curRow, int curCol);

// helper function : to check for a horizontal run
// in  : void
// out : char > "0 : no result(continue), otherwise : player numebr who won"
char checkTie(struct env* environment);

// helper of helper function : count the number of consequive same player
// in  : curRow > the current row
//       curCol > the current column
//       dr     > the change in row number
//       dc     > the change in column number
//       p      > the player number
// out : int    > the number of consequtive occurnaces of the same player p
int count(struct env* environment, int curRow, int CurCol, int dr, int dc, int p);


//void displayBoard(struct env* e);

char checkWiner(struct env* environment);

#endif
