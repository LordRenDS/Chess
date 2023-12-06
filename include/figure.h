#ifndef FIGURE_H
#define FIGURE_H
#include "color.h"
#include "figure_type.h"
#include <memory>
#include <vector>

class Move;
class Board;

class Figure {
  protected:
    int coordinate{};
    Color::ColorT color;
    bool firstMove{true};
    FigureType figureType;

  public:
    explicit Figure(int coordinate, Color::ColorT color, FigureType figureType);
    explicit Figure(const Figure &figure) = default;
    virtual ~Figure() = default;
    //
    int getCoordinate();
    FigureType getFigureType();
    Color::ColorT getColor() const;
    //
    virtual std::vector<std::unique_ptr<Move>>
    calculateLegalMoves(const Board &board) = 0;
    virtual void move(int coordinate, Board &board);
    bool isFirstMove() const;
    virtual std::string getFigureName() = 0;
    virtual std::unique_ptr<Figure> clone() const = 0;
};

class King : public Figure {
  private:
    static const int CANDIDATE_MOVE_COORDINATES[];
    bool castled{};
    bool check{};
    static bool isFirstColumnExclusion(int currentPosition,
                                       int candidateOffset);
    static bool isEightColumnExclusion(int currentPosition,
                                       int candidateOffset);

  public:
    explicit King(int coordinate, Color::ColorT color);
    explicit King(const King &king) = default;
    virtual ~King() = default;
    //
    std::vector<std::unique_ptr<Move>>
    calculateLegalMoves(const Board &board) override;
    bool isCastled();
    std::string getFigureName() override;
    std::unique_ptr<Figure> clone() const override;
};

class Queen : public Figure {
  public:
    explicit Queen(int coordinate, Color::ColorT color);
    explicit Queen(const Queen &queen) = default;
    virtual ~Queen() = default;
    //
    std::vector<std::unique_ptr<Move>>
    calculateLegalMoves(const Board &board) override;
    std::string getFigureName() override;
    std::unique_ptr<Figure> clone() const override;
};

class Rook : public Figure {
  public:
    explicit Rook(int coordinate, Color::ColorT color);
    explicit Rook(const Rook &rook) = default;
    virtual ~Rook() = default;
    //
    std::vector<std::unique_ptr<Move>>
    calculateLegalMoves(const Board &board) override;
    std::string getFigureName() override;
    std::unique_ptr<Figure> clone() const override;
};

class Knight : public Figure {
  public:
    explicit Knight(int coordinate, Color::ColorT color);
    explicit Knight(const Knight &knight) = default;
    virtual ~Knight() = default;
    //
    std::vector<std::unique_ptr<Move>>
    calculateLegalMoves(const Board &board) override;
    std::string getFigureName() override;
    std::unique_ptr<Figure> clone() const override;
};

class Bishop : public Figure {
  public:
    explicit Bishop(int coordinate, Color::ColorT color);
    explicit Bishop(const Bishop &bishop) = default;
    virtual ~Bishop() = default;
    //
    std::vector<std::unique_ptr<Move>>
    calculateLegalMoves(const Board &board) override;
    std::string getFigureName() override;
    std::unique_ptr<Figure> clone() const override;
};

class Pawn : public Figure {
  private:
    bool enPassant{};

  public:
    explicit Pawn(int coordinate, Color::ColorT color);
    explicit Pawn(const Pawn &pawn) = default;
    virtual ~Pawn() = default;
    //
    std::vector<std::unique_ptr<Move>>
    calculateLegalMoves(const Board &board) override;
    std::string getFigureName() override;
    std::unique_ptr<Figure> clone() const override;
    bool isEnPassant();
};

#endif