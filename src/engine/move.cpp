#include "move.h"
#include "board.h"
#include "figure.h"

Move::Move(Figure *figure, int coordinateToMove)
    : movedFigure(figure), coordinateToMove(coordinateToMove) {
}

int Move::getCoordinateToMove() {
    return coordinateToMove;
}

Figure *Move::getMovedFigure() {
    return movedFigure;
}

std::unique_ptr<Board> Move::execute(Board &board) {
    std::unique_ptr<Board> movedBoard(std::make_unique<Board>(board));
    movedBoard->getSquare(movedFigure->getCoordinate())
        ->getFigureOnSquare()
        ->move(coordinateToMove, *movedBoard);
    return movedBoard;
}

std::unique_ptr<Board> MajorMove::execute(Board &board) {
    return Move::execute(board);
}

Figure *AttackMove::getAttackFigure() {
    return attackFigure;
}

std::unique_ptr<Board> AttackMove::execute(Board &board) {
    // std::unique_ptr<Board> movedBoard(std::make_unique<Board>(board));
    // movedBoard->getSquare(attackFigure->getCoordinate())
    //     ->releaseFigure()
    //     .reset();
    // movedBoard->getSquare(movedFigure->getCoordinate())
    //     ->getFigureOnSquare()
    //     ->move(coordinateToMove, *movedBoard);
    // return movedBoard;
    return Move::execute(board);
}

std::unique_ptr<Board> MajorAttackMove::execute(Board &board) {
    return AttackMove::execute(board);
}

std::unique_ptr<Board> PawnAttackMove::execute(Board &board) {
    return AttackMove::execute(board);
}

std::unique_ptr<Board> PawnMove::execute(Board &board) {
    return Move::execute(board);
}

std::unique_ptr<Board> PawnEnPassantAttackMove::execute(Board &board) {
    return AttackMove::execute(board);
}

std::unique_ptr<Board> PawnJump::execute(Board &board) {
    return Move::execute(board);
}

PawnPromotion::PawnPromotion(std::unique_ptr<Move> decoratedMove)
    : decoratedMove(std::move(decoratedMove)) {
}

void PawnPromotion::setPromotionFigure(std::unique_ptr<Figure> figure) {
    promotionFigure = std::move(figure);
}

std::unique_ptr<Board> PawnPromotion::execute(Board &board) {
    std::unique_ptr<Board> movedBoard(decoratedMove->execute(board));
    movedBoard->getSquare(decoratedMove->getCoordinateToMove())
        ->setFigureOnSquare(std::move(promotionFigure));
    return movedBoard;
}

CastleMove::CastleMove(Figure *king, Figure *rook)
    : Move(king, rook->getCoordinate()), rook(rook) {
}

std::unique_ptr<Board> CastleMove::execute(Board &board) {
    std::unique_ptr<Board> movedBoard(std::make_unique<Board>(board));
    std::unique_ptr<Figure> movedRook(
        movedBoard->getSquare(coordinateToMove)->releaseFigure());
    movedBoard->getSquare(movedFigure->getCoordinate())
        ->getFigureOnSquare()
        ->move(coordinateToMove, *movedBoard);
    return movedBoard;
}

std::unique_ptr<Board> KingSideCastleMove::execute(Board &board) {
    return CastleMove::execute(board);
}

std::unique_ptr<Board> QueenSideCastleMove::execute(Board &board) {
    return CastleMove::execute(board);
}
