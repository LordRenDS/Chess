#include "ai.h"
#include "board.h"
#include "cli.h"
#include "figure.h"
#include "move.h"
#include "player.h"
#include <memory>

Move *minimaxRoot(int depth, Table &table, bool isMax) {
    int bestScore{};
    Move *bestMove{};
    auto calculationTable(std::make_unique<Table>(table));
    for (auto &move : calculationTable->getBlackPlayer()->getLegalMoves()) {
        if (calculationTable->getBlackPlayer()->makeMove(
                move.get(), *calculationTable->getBoard(),
                calculationTable->getWhitePlayer()) ==
            MoveStatus::LEAVE_PLAYER_IN_CHEK)
            continue;
        Score score = minimax(depth - 1, *calculationTable, 1000, 1000, !isMax);
        if (score.white < bestScore) {
            bestScore = score.white;
            bestMove = move.get();
        }
    }
    for (auto &move : table.getBlackPlayer()->getLegalMoves()) {
        if (*bestMove == *move.get())
            return move.get();
    }
}

Score minimax(int depth, Table &table, int alpha, int beta, bool isMax) {
    if (!depth)
        return Score{table.getBoard()->evaluateBoard(Color::ColorT::WHITE),
                     table.getBoard()->evaluateBoard(Color::ColorT::BLACK)};
    auto calculationTable(std::make_unique<Table>(table));
    Score score{};
    Score nextScore{};
    if (isMax) {
        score = {100, -100};
        for (auto &move : calculationTable->getBlackPlayer()->getLegalMoves()) {
            if (calculationTable->getBlackPlayer()->makeMove(
                    move.get(), *calculationTable->getBoard(),
                    calculationTable->getWhitePlayer()) ==
                MoveStatus::LEAVE_PLAYER_IN_CHEK)
                continue;
            nextScore =
                minimax(depth - 1, *calculationTable, alpha, beta, !isMax);
            score = score.white < nextScore.white ? score : nextScore;
            alpha = std::min(alpha, score.white);
            if (beta >= alpha)
                return score;
        }
        return score;
    } else {
        score = {-100, 100};
        for (auto &move : calculationTable->getWhitePlayer()->getLegalMoves()) {
            if (calculationTable->getWhitePlayer()->makeMove(
                    move.get(), *calculationTable->getBoard(),
                    calculationTable->getBlackPlayer()) ==
                MoveStatus::LEAVE_PLAYER_IN_CHEK)
                continue;
            nextScore =
                minimax(depth - 1, *calculationTable, alpha, beta, !isMax);
            score = score.black < nextScore.black ? score : nextScore;
            beta = std::min(beta, score.black);
            if (beta >= alpha)
                return score;
        }
        return score;
    }
}