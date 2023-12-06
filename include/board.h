#ifndef BOARD_H
#define BOARD_H
#include "board_utils.h"
#include "color.h"
#include <array>
#include <memory>
#include <vector>

#include "figure.h"
// class Figure;

class Square {
  private:
    int coordinate{-1};
    std::unique_ptr<Figure> figureOnSquare;

  public:
    explicit Square(int coordinate);
    explicit Square(const Square &square);
    ~Square() = default;
    //
    int getCoordinate() const;
    Figure *getFigureOnSquare();
    void setFigureOnSquare(std::unique_ptr<Figure> newFigure);
    //
    std::unique_ptr<Figure> releaseFigure();
    bool isSquareOccupie() const;
};

class Board {
  private:
    std::array<std::unique_ptr<Square>, BoardUtils::NUMBER_SQUARES> board;
    void setFigureOnBoard(std::unique_ptr<Figure> figure);

  public:
    explicit Board();
    explicit Board(const Board &board);
    ~Board() = default;
    //
    Board &operator=(Board &&board);
    //
    std::vector<Figure *> getActiveFigures();
    std::vector<Figure *> getActiveFigures(Color::ColorT color);
    Square *getSquare(int coordinate);
    void printBoard() const;
};

// class Move {};

// class MajoreMove : public Move {};

// class PawnMove : public Move {};

// class PawnJump : public Move {};

// class PawnPromotion : public Move {};

// class AttackMove : public Move {};

// class MajorAttackMove : public AttackMove {};

// class PawnAttackMove : public AttackMove {};

// class PawnEnPassantAttackMove : public PawnAttackMove {};

// class CastleMove : public Move {};

// class KingSideCastleMove : public CastleMove {};

// class QueenSideCastleMove : public CastleMove {};

#endif