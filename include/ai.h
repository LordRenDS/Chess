#ifndef CHESS_AI_H
#define CHESS_AI_H

class Move;
class Table;

class AI {
  public:
    static Move *minimaxRoot(int depth, Table &table, bool isMax);
    static int minimax(int depth, Table &table, int alpha, int beta,
                       bool isMax);
};

#endif