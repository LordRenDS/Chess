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
    Figure(const Figure &figure) = default;
    virtual ~Figure() = default;
    //
    int getCoordinate() const;
    FigureType getFigureType() const;
    Color::ColorT getColor() const;
    //
    virtual std::vector<std::unique_ptr<Move>>
    calculateLegalMoves(Board &board) = 0;
    void
    move(int coordinate, Board &board); // todo make virtual and impl for pawn(set EnPassant)
    bool isFirstMove() const;
    virtual std::string getFigureName() = 0;
    virtual std::unique_ptr<Figure> clone() const = 0;
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
    calculateLegalMoves(Board &board) override;
    bool isCastled() const;
    bool isInCheck() const;
    std::string getFigureName() override;
    std::unique_ptr<Figure> clone() const override;
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
    calculateLegalMoves(Board &board) override;
    std::string getFigureName() override;
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
    calculateLegalMoves(Board &board) override;
    std::string getFigureName() override;
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
    calculateLegalMoves(Board &board) override;
    std::string getFigureName() override;
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
    calculateLegalMoves(Board &board) override;
    std::string getFigureName() override;
    std::unique_ptr<Figure> clone() const override;
};

class Pawn : public Figure {
  private:
    static constexpr int CANDIDATE_MOVE_COORDINATES[]{8, 16, 7, 9};
    bool enPassant{};
    bool isHasEnPassantMove(Board &board, int enPassantCoordinate) const;

  public:
    explicit Pawn(int coordinate, Color::ColorT color);
    Pawn(const Pawn &pawn) = default;
    ~Pawn() override = default;
    //
    void setEnPassant(bool enPassant);
    //
    std::vector<std::unique_ptr<Move>>
    calculateLegalMoves(Board &board) override;
    std::string getFigureName() override;
    std::unique_ptr<Figure> clone() const override;
    bool isEnPassant() const;
};

#endif