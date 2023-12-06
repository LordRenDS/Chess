#include "player.h"
#include "board.h"
#include "cli.h"
#include "figure.h"
#include "move.h"

Player::Player(Board &board) : legalMoves(calculateLegalMoves(board)) {
}

std::vector<std::unique_ptr<Move>>
Player::calculateLegalMoves(Board &board) const {
    std::vector<std::unique_ptr<Move>> moves;
    for (auto &figure : board.getActiveFigures(getColor())) {
        auto calculated{figure->calculateLegalMoves(board)};
        for (auto &move : calculated)
            moves.push_back(std::move(move));
    }
    return moves;
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
    Board &board, const std::vector<std::unique_ptr<Move>> &opponentMoves) {
    std::vector<Figure *> playerFigures{board.getActiveFigures(getColor())};
    Figure *playerKing{};
    for (auto figure : playerFigures)
        if (figure->getFigureType() == FigureType::KING) {
            playerKing = figure;
            break;
        }
    std::vector<Move *> attaksOnKing{
        calculateAttackOnSquare(playerKing->getCoordinate(), opponentMoves)};
    return (!attaksOnKing.size()) ? false : true;
}

bool Player::hasEscapeMoves(Board &board, Player *opponent) {
    std::unique_ptr<Board> movedBoard;
    std::vector<std::unique_ptr<Move>> opponentMoves;
    for (auto &move : legalMoves) {
        movedBoard = move->execute(board);
        opponentMoves = opponent->calculateLegalMoves(*movedBoard);
        if (!hasAttackOnKing(*movedBoard, opponentMoves))
            return true;
    }
    return false;
}

std::vector<std::unique_ptr<Move>> &Player::getLegalMoves() {
    return legalMoves;
}

MoveStatus Player::makeMove(const Move *move, Board &board, Player *opponent) {
    std::unique_ptr<Board> movedBoard{move->execute(board)};
    std::vector<std::unique_ptr<Move>> opponentMoves{
        opponent->calculateLegalMoves(*movedBoard)};
    if (hasAttackOnKing(*movedBoard, opponentMoves))
        return MoveStatus::LEAVE_PLAYER_IN_CHEK;
    else {
        legalMoves = calculateLegalMoves(*movedBoard);
        opponent->legalMoves = std::move(opponentMoves);
        if (opponent->hasAttackOnKing(*movedBoard, legalMoves))
            opponent->inCheck = true;
        board = std::move(*movedBoard);
        return MoveStatus::DONE;
    }
}

bool Player::isInCheckMate(Board &board, Player *opponent) {
    return (inCheck && !hasEscapeMoves(board, opponent)) ? true : false;
}

std::string Player::getPlayerName() {
    return (getColor() == Color::ColorT::BLACK) ? "Black player"
                                                : "White player";
}

Color::ColorT WhitePlayer::getColor() const {
    return Color::ColorT::WHITE;
}

Color::ColorT BlackPlayer::getColor() const {
    return Color::ColorT::BLACK;
}
