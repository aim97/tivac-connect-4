#ifndef AGENT_H
#define AGENT_H

#include "env.h"

char nextMove(struct env* env);

char getMove(struct env* vm, int depth);
int mini_max(struct env* p, int depth, char isMax, int alpha, int beta);

#endif
