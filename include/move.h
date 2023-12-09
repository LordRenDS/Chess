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
    explicit Move(Figure *figure, int coordinateToMove);
    virtual ~Move() = default;
    //
    int getCoordinateToMove() const;
    Figure *getMovedFigure();
    const Figure *getMovedFigure() const;
    //
    virtual std::unique_ptr<Board> execute(Board &board) = 0;
};

class MajoreMove : public Move {
  public:
    using Move::Move;
    MajoreMove() = delete;
    //
    std::unique_ptr<Board> execute(Board &board) override;
};

class AttackMove : public Move {
  protected:
    Figure *attackFigure;

  public:
    AttackMove(Figure *figure, Figure *attackFigure);
    ~AttackMove() override = default;
    //
    Figure *getAttackFigure();
    const Figure *getAttackFigure() const;
    //
    std::unique_ptr<Board> execute(Board &board) override = 0;
};

class MajorAttackMove : public AttackMove {
  public:
    using AttackMove::AttackMove;
    //
    std::unique_ptr<Board> execute(Board &board) override;
};

class PawnAttackMove : public AttackMove {
  public:
    using AttackMove::AttackMove;
    //
    std::unique_ptr<Board> execute(Board &board) override;
};

class PawnEnPassantAttackMove : public PawnAttackMove {
  public:
    using PawnAttackMove::PawnAttackMove;
    //
    std::unique_ptr<Board> execute(Board &board) override;
};

class PawnMove : public Move {
  public:
    using Move::Move;
    PawnMove() = delete;
    //
    std::unique_ptr<Board> execute(Board &board) override;
};

class PawnJump : public Move {
  public:
    using Move::Move;
    //
    std::unique_ptr<Board> execute(Board &board) override; // todo EnPassant set
};

class PawnPromotion : public Move {
  private:
    std::unique_ptr<Figure> promotionFigure;
    std::unique_ptr<Move> decoratedMove;

  public:
    explicit PawnPromotion(std::unique_ptr<Move> decoratedMove);
    ~PawnPromotion() override = default;
    //
    void setPromotionFigure(std::unique_ptr<Figure> figure);
    //
    std::unique_ptr<Board> execute(Board &board) override;
};

class CastleMove : public Move {
  private:
    Figure *rook{};
    int rookDestinationCoordinate{};

  public:
    CastleMove(Figure *king, int kingDestCoord, Figure *rook,
               int rookDestCoord);
    ~CastleMove() override = default;
    //
    std::unique_ptr<Board> execute(Board &board) override = 0;
};

class KingSideCastleMove : public CastleMove {
  public:
    using CastleMove::CastleMove;
    //
    std::unique_ptr<Board> execute(Board &board) override;
};

class QueenSideCastleMove : public CastleMove {
  public:
    using CastleMove::CastleMove;
    //
    std::unique_ptr<Board> execute(Board &board) override;
};

#endif