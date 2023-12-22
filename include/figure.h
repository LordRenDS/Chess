#ifndef FIGURE_H
#define FIGURE_H
#include "color.h"
#include <memory>
#include <vector>

enum class FigureType;
class Move;
class Board;
class Player;

class Figure {
  protected:
    int coordinate{};
    Color::ColorT color;
    bool firstMove{true};
    FigureType figureType;
    int value{};

  public:
    explicit Figure(int coordinate, Color::ColorT color, FigureType figureType,
                    int value);
    Figure(const Figure &figure) = default;
    virtual ~Figure() = default;
    //
    int getCoordinate() const;
    FigureType getFigureType() const;
    Color::ColorT getColor() const;
    int getValue() const;
    //
    virtual std::vector<std::unique_ptr<Move>>
    calculateLegalMoves(Board &board) const = 0;
    void
    move(int coordinate, Board &board);
    bool isFirstMove() const;
    virtual std::string getFigureName() const = 0;
    virtual std::unique_ptr<Figure> clone() const = 0;
    virtual bool equals(const Figure &other) const;
};

class King : public Figure {
  private:
    static constexpr int CANDIDATE_MOVE_COORDINATES[]{-9, -8, -7, -1,
                                                      1,  7,  8,  9};
    bool castled{};
    bool inCheck{};
    static bool isFirstColumnExclusion(int currentPosition,
                                       int candidateOffset);
    static bool isEightColumnExclusion(int currentPosition,
                                       int candidateOffset);

  public:
    explicit King(int coordinate, Color::ColorT color);
    King(const King &king) = default;
    ~King() override = default;
    //
    std::vector<std::unique_ptr<Move>>
    calculateLegalMoves(Board &board) const override;
    bool isCastled() const;
    bool isInCheck() const;
    std::string getFigureName() const override;
    std::unique_ptr<Figure> clone() const override;
    bool equals(const Figure &other) const override;
    //
    friend Player;
};

class Queen : public Figure {
  private:
    static constexpr int CANDIDATE_MOVE_VECTOR_COORDINATES[]{-9, -8, -7, -1,
                                                             1,  7,  8,  9};
    static bool isFirstColumnExclusion(int currentPosition,
                                       int candidateOffset);
    static bool isEightColumnExclusion(int currentPosition,
                                       int candidateOffset);

  public:
    explicit Queen(int coordinate, Color::ColorT color);
    Queen(const Queen &queen) = default;
    ~Queen() override = default;
    //
    std::vector<std::unique_ptr<Move>>
    calculateLegalMoves(Board &board) const override;
    std::string getFigureName() const override;
    std::unique_ptr<Figure> clone() const override;
};

class Rook : public Figure {
  private:
    static constexpr int CANDIDATE_MOVE_VECTOR_COORDINATES[]{-8, -1, 1, 8};
    static bool isFirstColumnExclusion(int currentPosition,
                                       int candidateOffset);
    static bool isEightColumnExclusion(int currentPosition,
                                       int candidateOffset);

  public:
    explicit Rook(int coordinate, Color::ColorT color);
    Rook(const Rook &rook) = default;
    ~Rook() override = default;
    //
    std::vector<std::unique_ptr<Move>>
    calculateLegalMoves(Board &board) const override;
    std::string getFigureName() const override;
    std::unique_ptr<Figure> clone() const override;
};

class Knight : public Figure {
  private:
    static constexpr int CANDIDATE_MOVE_COORDINATES[]{-17, -15, -10, -6,
                                                      6,   10,  15,  17};
    static bool isFirstColumnExclusion(int currentPosition,
                                       int candidateOffset);
    static bool isSecondColumnExclusion(int currentPosition,
                                        int candidateOffset);
    static bool isSeventhColumnExclusion(int currentPosition,
                                         int candidateOffset);
    static bool isEightColumnExclusion(int currentPosition,
                                       int candidateOffset);

  public:
    explicit Knight(int coordinate, Color::ColorT color);
    Knight(const Knight &knight) = default;
    ~Knight() override = default;
    //
    std::vector<std::unique_ptr<Move>>
    calculateLegalMoves(Board &board) const override;
    std::string getFigureName() const override;
    std::unique_ptr<Figure> clone() const override;
};

class Bishop : public Figure {
  private:
    static constexpr int CANDIDATE_MOVE_VECTOR_COORDINATES[]{-9, -7, 7, 9};
    static bool isFirstColumnExclusion(int currentPosition,
                                       int candidateOffset);
    static bool isEightColumnExclusion(int currentPosition,
                                       int candidateOffset);

  public:
    explicit Bishop(int coordinate, Color::ColorT color);
    Bishop(const Bishop &bishop) = default;
    ~Bishop() override = default;
    //
    std::vector<std::unique_ptr<Move>>
    calculateLegalMoves(Board &board) const override;
    std::string getFigureName() const override;
    std::unique_ptr<Figure> clone() const override;
};

class Pawn : public Figure {
  private:
    static constexpr int CANDIDATE_MOVE_COORDINATES[]{8, 16, 7, 9};
    bool isHasEnPassantMove(Board &board, int enPassantCoordinate) const;

  public:
    explicit Pawn(int coordinate, Color::ColorT color);
    Pawn(const Pawn &pawn) = default;
    ~Pawn() override = default;
    //
    std::vector<std::unique_ptr<Move>>
    calculateLegalMoves(Board &board) const override;
    std::string getFigureName() const override;
    std::unique_ptr<Figure> clone() const override;
};

#endif