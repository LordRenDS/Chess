#include "player.h"
#include "board.h"
#include "figure.h"
#include "figure_type.h"
#include "move.h"

Player::Player(King *king, std::vector<std::unique_ptr<Move>> &&legalMoves)
    : playerKing(king), legalMoves(std::move(legalMoves)) {
}

std::vector<Move *> Player::calculateAttackOnSquare(
    int coordinate, const std::vector<std::unique_ptr<Move>> &moves) {
    std::vector<Move *> attackMoves;
    for (const auto &move : moves)
        if (move->getCoordinateToMove() == coordinate)
            attackMoves.push_back(move.get());
    return attackMoves;
}

bool Player::hasAttackOnKing(
    const std::vector<std::unique_ptr<Move>> &opponentMoves) {
    std::vector<Move *> attacksOnKing{
        calculateAttackOnSquare(playerKing->getCoordinate(), opponentMoves)};
    return !(attacksOnKing.empty());
}

bool Player::hasAttackOnKing(
    const King *king, const std::vector<std::unique_ptr<Move>> &opponentMoves) {
    std::vector<Move *> attacksOnKing{
        calculateAttackOnSquare(king->getCoordinate(), opponentMoves)};
    return !(attacksOnKing.empty());
}

bool Player::hasEscapeMoves(Board &board, Player *opponent) {
    std::unique_ptr<Board> movedBoard;
    std::vector<std::unique_ptr<Move>> opponentMoves;
    for (auto &move : legalMoves) {
        movedBoard = move->execute(board);
        opponentMoves = movedBoard->calculateLegalMoves(opponent->getColor());
        if (!hasAttackOnKing(movedBoard->getKing(getColor()), opponentMoves))
            return true;
    }
    return false;
}

std::vector<std::unique_ptr<Move>> &Player::getLegalMoves() {
    return legalMoves;
}

std::vector<std::unique_ptr<Move>> Player::calculateAllLegalMoves(
    Board &board, const std::vector<std::unique_ptr<Move>> &opponentMoves) {
    auto moves(board.calculateLegalMoves(getColor()));
    auto castleMove(calculateCastleMoves(board, opponentMoves));
    for (auto &move : castleMove)
        moves.push_back(std::move(move));
    return moves;
}

MoveStatus Player::makeMove(Move *move, Board &board, Player *opponent) {
    std::unique_ptr<Board> movedBoard(move->execute(board));
    std::vector<std::unique_ptr<Move>> opponentMoves(
        movedBoard->calculateLegalMoves(opponent->getColor()));
    King *movedPlayerKing{movedBoard->getKing(getColor())};
    if (hasAttackOnKing(movedPlayerKing, opponentMoves))
        return MoveStatus::LEAVE_PLAYER_IN_CHEK;
    else {
        playerKing = movedPlayerKing;
        legalMoves = calculateAllLegalMoves(*movedBoard, opponentMoves);
        if (inCheck) {
            inCheck = false;
            playerKing->inCheck = false;
        }
        opponent->playerKing = movedBoard->getKing(opponent->getColor());
        opponent->legalMoves =
            opponent->calculateAllLegalMoves(*movedBoard, legalMoves);
        if (opponent->hasAttackOnKing(legalMoves)) {
            opponent->inCheck = true;
            opponent->playerKing->inCheck = true;
        }
        board = std::move(*movedBoard);
        return MoveStatus::DONE;
    }
}

bool Player::isInCheck() const {
    return inCheck;
}

bool Player::isInCheckMate(Board &board, Player *opponent) {
    return (inCheck && !hasEscapeMoves(board, opponent));
}

std::vector<std::unique_ptr<Move>> WhitePlayer::calculateCastleMoves(
    Board &board, const std::vector<std::unique_ptr<Move>> &opponentMoves) {
    std::vector<std::unique_ptr<Move>> castleMoves;
    if (playerKing->isFirstMove() && !inCheck) {
        if (!board.getSquare(61)->isSquareOccupied() &&
            !board.getSquare(62)->isSquareOccupied()) {
            Square *rookSquare{board.getSquare(63)};
            if (rookSquare->isSquareOccupied() &&
                rookSquare->getFigureOnSquare()->isFirstMove()) {
                if (calculateAttackOnSquare(61, opponentMoves).empty() &&
                    calculateAttackOnSquare(62, opponentMoves).empty() &&
                    rookSquare->getFigureOnSquare()->getFigureType() ==
                        FigureType::ROOK)
                    castleMoves.push_back(std::make_unique<KingSideCastleMove>(
                        playerKing, 62, rookSquare->getFigureOnSquare(), 61));
            }
        }
        if (!board.getSquare(59)->isSquareOccupied() &&
            !board.getSquare(58)->isSquareOccupied() &&
            !board.getSquare(57)->isSquareOccupied()) {
            Square *rookSquare{board.getSquare(56)};
            if (rookSquare->isSquareOccupied() &&
                rookSquare->getFigureOnSquare()->isFirstMove()) {
                if (calculateAttackOnSquare(59, opponentMoves).empty() &&
                    calculateAttackOnSquare(58, opponentMoves).empty() &&
                    calculateAttackOnSquare(57, opponentMoves).empty() &&
                    rookSquare->getFigureOnSquare()->getFigureType() ==
                        FigureType::ROOK)
                    castleMoves.push_back(std::make_unique<KingSideCastleMove>(
                        playerKing, 58, rookSquare->getFigureOnSquare(), 59));
            }
        }
    }
    return castleMoves;
}

Color::ColorT WhitePlayer::getColor() const {
    return Color::ColorT::WHITE;
}

std::string WhitePlayer::getPlayerName() const {
    return "White player";
}

std::vector<std::unique_ptr<Move>> BlackPlayer::calculateCastleMoves(
    Board &board, const std::vector<std::unique_ptr<Move>> &opponentMoves) {
    std::vector<std::unique_ptr<Move>> castleMoves;
    if (playerKing->isFirstMove() && !inCheck) {
        if (!board.getSquare(5)->isSquareOccupied() &&
            !board.getSquare(6)->isSquareOccupied()) {
            Square *rookSquare{board.getSquare(7)};
            if (rookSquare->isSquareOccupied() &&
                rookSquare->getFigureOnSquare()->isFirstMove()) {
                if (calculateAttackOnSquare(5, opponentMoves).empty() &&
                    calculateAttackOnSquare(6, opponentMoves).empty() &&
                    rookSquare->getFigureOnSquare()->getFigureType() ==
                        FigureType::ROOK)
                    castleMoves.push_back(std::make_unique<KingSideCastleMove>(
                        playerKing, 5, rookSquare->getFigureOnSquare(), 6));
            }
        }
        if (!board.getSquare(1)->isSquareOccupied() &&
            !board.getSquare(2)->isSquareOccupied() &&
            !board.getSquare(3)->isSquareOccupied()) {
            Square *rookSquare{board.getSquare(0)};
            if (rookSquare->isSquareOccupied() &&
                rookSquare->getFigureOnSquare()->isFirstMove()) {
                if (calculateAttackOnSquare(1, opponentMoves).empty() &&
                    calculateAttackOnSquare(2, opponentMoves).empty() &&
                    calculateAttackOnSquare(3, opponentMoves).empty() &&
                    rookSquare->getFigureOnSquare()->getFigureType() ==
                        FigureType::ROOK)
                    castleMoves.push_back(std::make_unique<KingSideCastleMove>(
                        playerKing, 2, rookSquare->getFigureOnSquare(), 3));
            }
        }
    }
    return castleMoves;
}

Color::ColorT BlackPlayer::getColor() const {
    return Color::ColorT::BLACK;
}

std::string BlackPlayer::getPlayerName() const {
    return "Black player";
}
