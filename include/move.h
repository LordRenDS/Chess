#ifndef MOVE_H
#define MOVE_H
#include <memory>

class Figure;
class Board;

class Move {
  protected:
    Move() = default;
    Figure *movedFigure{};
    int coordinateToMove{};

  public:
    Move(Figure *figure, int coordinateToMove);
    virtual ~Move() = default;
    //
    int getCoordinateToMove();
    Figure *getMovedFigure();
    //
    virtual std::unique_ptr<Board> execute(Board &board) = 0;
};

class MajoreMove : public Move {
  public:
    std::unique_ptr<Board> execute(Board &board) override;
};

class AttackMove : public Move {
  protected:
    Figure *attackFigure;

  public:
    Figure *getAttackFigure();
    //
    std::unique_ptr<Board> execute(Board &board) override = 0;
};

class MajorAttacMove : public AttackMove {
  public:
    std::unique_ptr<Board> execute(Board &board) override;
};

class PawnAttacMove : public AttackMove {
  public:
    std::unique_ptr<Board> execute(Board &board) override;
};

class PawnEnPassantAttacMove : public PawnAttacMove {
  public:
    std::unique_ptr<Board> execute(Board &board) override;
};

class PawnMove : public Move {
  public:
    std::unique_ptr<Board> execute(Board &board) override;
};

class PawnJump : public Move {
  public:
    std::unique_ptr<Board> execute(Board &board) override;
};

class PawnPromotion : public Move {
  private:
    std::unique_ptr<Figure> promotionFigure;
    std::unique_ptr<Move> decoratedMove;

  public:
    PawnPromotion(std::unique_ptr<Move> decoratedMove);
    //
    void setPromotionFigure(std::unique_ptr<Figure> figure);
    //
    std::unique_ptr<Board> execute(Board &board) override;
};

class CastleMove : public Move {
  private:
    Figure *rook{};

  public:
    CastleMove(Figure *king, Figure *rook);
    std::unique_ptr<Board> execute(Board &board) override = 0;
};

class KingSideCastleMove : public CastleMove {
  public:
    std::unique_ptr<Board> execute(Board &board) override;
};

class QueenSideCastleMove : public CastleMove {
  public:
    std::unique_ptr<Board> execute(Board &board) override;
};

#endif