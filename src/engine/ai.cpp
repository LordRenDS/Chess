#include "ai.h"
#include "board.h"
#include "cli.h"
#include "figure.h"
#include "move.h"
#include "player.h"
#include <memory>

Move *minimaxRoot(int depth, Table &table, bool isMax) {
    int bestScore{1000};
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
        Score score = minimax(depth - 1, *calculationTable, 10000, 10000, !isMax);
        if (score.white < bestScore) {
            bestScore = score.white;
            bestMove = move.get();
        }
    }
    return bestMove;
}

Score minimax(int depth, Table &table, int alpha, int beta, bool isMax) {
    if (!depth)
        return Score{table.getBoard()->evaluateBoard(Color::ColorT::WHITE),
                     table.getBoard()->evaluateBoard(Color::ColorT::BLACK)};
    Score score{};
    Score nextScore{};
    if (isMax) {
        score = {1000, -1000};
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
            nextScore =
                minimax(depth - 1, *calculationTable, alpha, beta, !isMax);
            score = score.white < nextScore.white ? score : nextScore;
//            alpha = std::min(alpha, score.white);
//            if (beta >= alpha)
//                return score;
        }
        return score;
    } else {
        score = {-1000, 1000};
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
            nextScore =
                minimax(depth - 1, *calculationTable, alpha, beta, !isMax);
            score = score.black < nextScore.black ? score : nextScore;
//            beta = std::min(beta, score.black);
//            if (beta >= alpha)
//                return score;
        }
        return score;
    }
}