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
        ->move(movedFigure->getCoordinate(), *movedBoard);
    return movedBoard;
}

std::unique_ptr<Board> MajoreMove::execute(Board &board) {
    return Move::execute(board);
}

Figure *AttackMove::getAttackFigure() {
    return attackFigure;
}

std::unique_ptr<Board> AttackMove::execute(Board &board) {
    std::unique_ptr<Board> movedBoard(std::make_unique<Board>(board));
    movedBoard->getSquare(attackFigure->getCoordinate())
        ->releaseFigure()
        .reset();
    movedBoard->getSquare(movedFigure->getCoordinate())
        ->getFigureOnSquare()
        ->move(movedFigure->getCoordinate(), *movedBoard);
    return movedBoard;
}

std::unique_ptr<Board> MajorAttacMove::execute(Board &board) {
    return AttackMove::execute(board);
}

std::unique_ptr<Board> PawnAttacMove::execute(Board &board) {
    return AttackMove::execute(board);
}

std::unique_ptr<Board> PawnMove::execute(Board &board) {
    return Move::execute(board);
}

std::unique_ptr<Board> PawnEnPassantAttacMove::execute(Board &board) {
    return AttackMove::execute(board);
}

std::unique_ptr<Board> PawnJump::execute(Board &board) {
    return Move::execute(board);
}

std::unique_ptr<Board> PawnPromotion::execute(Board &board) {
    //     std::unique_ptr<Board> movedBoard(std::make_unique<Board>(board));
    //     movedBoard = decoratedMove->execute(*movedBoard);
    std::unique_ptr<Board> movedBoard(decoratedMove->execute(board));
}
