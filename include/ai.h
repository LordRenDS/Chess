#ifndef CHESS_AI_H
#define CHESS_AI_H

class Move;
class Table;

struct Score {
    int white{};
    int black{};
};

Move *minimaxRoot(int depth, Table &table, bool isMax);

Score minimax(int depth, Table &table, int alpha, int beta, bool isMax);

#endif