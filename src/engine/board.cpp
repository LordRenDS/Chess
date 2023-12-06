#include "board.h"
// #include "figure.h"
#include <iostream>

Square::Square(int coordinate) : coordinate(coordinate) {
}

Square::Square(const Square &square)
    : coordinate(square.coordinate),
      figureOnSquare(square.figureOnSquare->clone()) {
}

int Square::getCoordinate() const {
    return coordinate;
}

Figure *Square::getFigureOnSquare() {
    return figureOnSquare.get();
}

void Square::setFigureOnSquare(std::unique_ptr<Figure> newFigure) {
    figureOnSquare = std::move(newFigure);
}

std::unique_ptr<Figure> Square::releaseFigure() {
    return std::move(figureOnSquare);
}

bool Square::isSquareOccupie() const {
    return figureOnSquare == nullptr ? false : true;
}

void Board::setFigureOnBoard(std::unique_ptr<Figure> figure) {
    board[figure->getCoordinate()]->setFigureOnSquare(std::move(figure));
}

Board::Board() {
    int cnt{};
    for (auto &square : board) {
        square = std::make_unique<Square>(cnt);
        cnt++;
    }
    // Black
    setFigureOnBoard(std::make_unique<Rook>(0, Color::ColorT::BLACK));
    setFigureOnBoard(std::make_unique<Knight>(1, Color::ColorT::BLACK));
    setFigureOnBoard(std::make_unique<Bishop>(2, Color::ColorT::BLACK));
    setFigureOnBoard(std::make_unique<Queen>(3, Color::ColorT::BLACK));
    setFigureOnBoard(std::make_unique<King>(4, Color::ColorT::BLACK));
    setFigureOnBoard(std::make_unique<Bishop>(5, Color::ColorT::BLACK));
    setFigureOnBoard(std::make_unique<Knight>(6, Color::ColorT::BLACK));
    setFigureOnBoard(std::make_unique<Rook>(7, Color::ColorT::BLACK));
    setFigureOnBoard(std::make_unique<Pawn>(8, Color::ColorT::BLACK));
    setFigureOnBoard(std::make_unique<Pawn>(9, Color::ColorT::BLACK));
    setFigureOnBoard(std::make_unique<Pawn>(10, Color::ColorT::BLACK));
    setFigureOnBoard(std::make_unique<Pawn>(11, Color::ColorT::BLACK));
    setFigureOnBoard(std::make_unique<Pawn>(12, Color::ColorT::BLACK));
    setFigureOnBoard(std::make_unique<Pawn>(13, Color::ColorT::BLACK));
    setFigureOnBoard(std::make_unique<Pawn>(14, Color::ColorT::BLACK));
    setFigureOnBoard(std::make_unique<Pawn>(15, Color::ColorT::BLACK));
    // White
    setFigureOnBoard(std::make_unique<Pawn>(48, Color::ColorT::WHITE));
    setFigureOnBoard(std::make_unique<Pawn>(49, Color::ColorT::WHITE));
    setFigureOnBoard(std::make_unique<Pawn>(50, Color::ColorT::WHITE));
    setFigureOnBoard(std::make_unique<Pawn>(51, Color::ColorT::WHITE));
    setFigureOnBoard(std::make_unique<Pawn>(52, Color::ColorT::WHITE));
    setFigureOnBoard(std::make_unique<Pawn>(53, Color::ColorT::WHITE));
    setFigureOnBoard(std::make_unique<Pawn>(54, Color::ColorT::WHITE));
    setFigureOnBoard(std::make_unique<Pawn>(55, Color::ColorT::WHITE));
    setFigureOnBoard(std::make_unique<Rook>(56, Color::ColorT::WHITE));
    setFigureOnBoard(std::make_unique<Knight>(57, Color::ColorT::WHITE));
    setFigureOnBoard(std::make_unique<Bishop>(58, Color::ColorT::WHITE));
    setFigureOnBoard(std::make_unique<Queen>(59, Color::ColorT::WHITE));
    setFigureOnBoard(std::make_unique<King>(60, Color::ColorT::WHITE));
    setFigureOnBoard(std::make_unique<Bishop>(61, Color::ColorT::WHITE));
    setFigureOnBoard(std::make_unique<Knight>(62, Color::ColorT::WHITE));
    setFigureOnBoard(std::make_unique<Rook>(63, Color::ColorT::WHITE));
}

Board::Board(const Board &board) {
    for (int i{}; i < BoardUtils::NUMBER_SQUARES; i++) {
        this->board[i] = std::make_unique<Square>(*board.board[i]);
    }
}

Board &Board::operator=(Board &&board) {
    if (&board == this)
        return *this;
    this->board = std::move(board.board);
    return *this;
}

std::vector<Figure *> Board::getActiveFigures() {
    std::vector<Figure *> figures;
    for (auto &square : board)
        if (square->isSquareOccupie())
            figures.push_back(square->getFigureOnSquare());
    return figures;
}

std::vector<Figure *> Board::getActiveFigures(Color::ColorT color) {
    std::vector<Figure *> figures;
    for (auto &square : board) {
        if (square->isSquareOccupie() &&
            square->getFigureOnSquare()->getColor() == color)
            figures.push_back(square->getFigureOnSquare());
    }
    return figures;
}

Square *Board::getSquare(int coordinate) {
    return board[coordinate].get();
}

/*
    a    b    c    d    e    f    g    h
- .---------------------------------------.
8 | bR |bKn | bB | bQ | bK | bB |bKn | bR |
- |----|----|----|----|----|----|----|----|
7 | bP | bP | bP | bP | bP | bP | bP | bP |
- |----|----|----|----|----|----|----|----|
6 |    |    |    |    |    |    |    |    |
- |----|----|----|----|----|----|----|----|
5 |    |    |    |    |    |    |    |    |
- |----|----|----|----|----|----|----|----|
4 |    |    |    |    |    |    |    |    |
- |----|----|----|----|----|----|----|----|
3 |    |    |    |    |    |    |    |    |
- |----|----|----|----|----|----|----|----|
2 | wP | wP | wP | wP | wP | wP | wP | wP |
- |----|----|----|----|----|----|----|----|
1 | wR |wKn | wB | wQ | bK | wB |wKn | wR |
- ^---------------------------------------^
*/
void Board::printBoard() const {
    std::cout << "    a    b    c    d    e    f    g    h\n"
                 "- .---------------------------------------.\n";
    int row{8};
    for (int i{}; i < BoardUtils::NUMBER_SQUARES; i++) {
        if (i % BoardUtils::NUMBER_SQUARE_PER_ROW == 0) {
            std::cout << std::to_string(row) + " |";
            row--;
        }
        if (board[i]->getFigureOnSquare() != nullptr) {
            if (board[i]->getFigureOnSquare()->getFigureType() !=
                FigureType::KNIGHT) {
                std::cout << " " +
                                 board[i]->getFigureOnSquare()->getFigureName();
            } else {
                std::cout << board[i]->getFigureOnSquare()->getFigureName();
            }
        } else {
            std::cout << "   ";
        }
        if ((i + 1) % BoardUtils::NUMBER_SQUARE_PER_ROW == 0) {
            std::cout << " |\n";
            if (row != 0)
                std::cout << "- |----|----|----|----|----|----|----|----|\n";
            else
                std::cout << "- ^---------------------------------------^\n";
        } else {
            std::cout << " |";
        }
    }
}