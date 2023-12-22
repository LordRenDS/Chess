#include "ai.h"
#include "board.h"
#include "cli.h"
#include "figure.h"
#include "move.h"
#include "player.h"
#include <memory>

Move *AI::minimaxRoot(int depth, Table &table, bool isMax) {
    int bestScore{-10000};
    Move *bestMove{};
    for (auto &move : table.getBlackPlayer()->getLegalMoves()) {
        auto calculationTable(std::make_unique<Table>(table));
        Move *mirrorMove{};
        for (auto &calculationMove :
             calculationTable->getBlackPlayer()->getLegalMoves()) {
            if (calculationMove->equals(*move))
                mirrorMove = calculationMove.get();
        }
        if (calculationTable->getBlackPlayer()->makeMove(
                mirrorMove, *calculationTable->getBoard(),
                calculationTable->getWhitePlayer()) ==
            MoveStatus::LEAVE_PLAYER_IN_CHEK)
            continue;
        int score =
            minimax(depth - 1, *calculationTable, -10000, 10000, !isMax);
        if (score >= bestScore) {
            bestScore = score;
            bestMove = move.get();
        }
    }
    return bestMove;
}

int AI::minimax(int depth, Table &table, int alpha, int beta, bool isMax) {
    if (!depth)
        return -table.getBoard()->evaluateBoard();
    int score{};
    int nextScore{};
    if (isMax) {
        score = -10000;
        for (auto &move : table.getBlackPlayer()->getLegalMoves()) {
            auto calculationTable(std::make_unique<Table>(table));
            Move *mirrorMove{};
            for (auto &calculationMove :
                 calculationTable->getBlackPlayer()->getLegalMoves()) {
                if (calculationMove->equals(*move))
                    mirrorMove = calculationMove.get();
            }
            if (calculationTable->getBlackPlayer()->makeMove(
                    move.get(), *calculationTable->getBoard(),
                    calculationTable->getWhitePlayer()) ==
                MoveStatus::LEAVE_PLAYER_IN_CHEK)
                continue;
            score = std::max(score, minimax(depth - 1, *calculationTable, alpha,
                                            beta, !isMax));
            alpha = std::max(alpha, score);
            if (beta <= alpha)
                return score;
        }
        return score;
    } else {
        score = 10000;
        for (auto &move : table.getWhitePlayer()->getLegalMoves()) {
            auto calculationTable(std::make_unique<Table>(table));
            Move *mirrorMove{};
            for (auto &calculationMove :
                 calculationTable->getWhitePlayer()->getLegalMoves()) {
                if (calculationMove->equals(*move))
                    mirrorMove = calculationMove.get();
            }
            if (calculationTable->getWhitePlayer()->makeMove(
                    move.get(), *calculationTable->getBoard(),
                    calculationTable->getBlackPlayer()) ==
                MoveStatus::LEAVE_PLAYER_IN_CHEK)
                continue;
            score = std::min(score, minimax(depth - 1, *calculationTable, alpha,
                                            beta, !isMax));
            beta = std::min(beta, score);
            if (beta <= alpha)
                return score;
        }
        return score;
    }
}