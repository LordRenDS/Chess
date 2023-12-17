#ifndef BOARD_H
#define BOARD_H
#include "board_utils.h"
#include "color.h"
#include <array>
#include <memory>
#include <vector>

class Figure;
class King;
class Pawn;
class Move;

class Square {
  private:
    const int coordinate;
    std::unique_ptr<Figure> figureOnSquare;

  public:
    explicit Square(int coordinate);
    Square(const Square &square);
    ~Square() = default;
    //
    int getCoordinate() const;
    Figure *getFigureOnSquare();
    const Figure *getFigureOnSquare() const;
    void setFigureOnSquare(std::unique_ptr<Figure> newFigure);
    //
    std::unique_ptr<Figure> releaseFigure();
    bool isSquareOccupied() const;
};

class Board {
  private:
    std::array<std::unique_ptr<Square>, BoardUtils::NUMBER_SQUARES> board;
    Pawn *enPassantPawn{};
    //
    void setFigureOnBoard(std::unique_ptr<Figure> figure);

  public:
    explicit Board();
    Board(const Board &board);
    ~Board() = default;
    //
    void setEnPassantPawn(Pawn *pawn);
    Pawn *getEnPassantPawn();
    Board &operator=(Board &&board) noexcept;
    //
    std::vector<Figure *> getActiveFigures();
    std::vector<Figure *> getActiveFigures(Color::ColorT color);
    Square *getSquare(int coordinate);
    const Square *getSquare(int coordinate) const;
    std::vector<std::unique_ptr<Move>>
    calculateLegalMoves(Color::ColorT playerColor);
    King *getKing(Color::ColorT color);
    void printBoard() const;
};

#endif