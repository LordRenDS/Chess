#ifndef MOVE_H
#define MOVE_H
#include <memory>

class Figure;
class Board;

class Move {
  protected:
    Move() = default;
    const Figure *movedFigure{};
    const int coordinateToMove{};

  public:
    explicit Move(const Figure *figure, int coordinateToMove);
    virtual ~Move() = default;
    //
    int getCoordinateToMove() const;
    const Figure *getMovedFigure() const;
    //
    virtual std::unique_ptr<Board> execute(Board &board) const = 0;
};

class MajorMove : public Move {
  public:
    using Move::Move;
    MajorMove() = delete;
    ~MajorMove() override = default;
    //
    std::unique_ptr<Board> execute(Board &board) const override;
};

class AttackMove : public Move {
  protected:
    const Figure *attackFigure;
    AttackMove(const Figure *figure, const Figure *attackFigure,
               int coordinateToMove);

  public:
    AttackMove(const Figure *figure, const Figure *attackFigure);
    ~AttackMove() override = default;
    //
    const Figure *getAttackFigure() const;
    //
    std::unique_ptr<Board> execute(Board &board) const override = 0;
};

class MajorAttackMove : public AttackMove {
  public:
    using AttackMove::AttackMove;
    //
    std::unique_ptr<Board> execute(Board &board) const override;
};

class PawnAttackMove : public AttackMove {
  public:
    using AttackMove::AttackMove;
    //
    std::unique_ptr<Board> execute(Board &board) const override;
};

class PawnEnPassantAttackMove : public PawnAttackMove {
  public:
    explicit PawnEnPassantAttackMove(const Figure *pawn,
                                     const Figure *enPassantPawn,
                                     int coordinateToMove);
    std::unique_ptr<Board> execute(Board &board) const override;
};

class PawnMove : public Move {
  public:
    using Move::Move;
    PawnMove() = delete;
    //
    std::unique_ptr<Board> execute(Board &board) const override;
};

class PawnJump : public Move {
  public:
    using Move::Move;
    PawnJump() = delete;
    //
    std::unique_ptr<Board> execute(Board &board) const override;
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
    std::unique_ptr<Board> execute(Board &board) const override;
};

class CastleMove : public Move {
  private:
    const Figure *rook{};
    const int rookDestinationCoordinate{};

  public:
    CastleMove(const Figure *king, int kingDestCoord, const Figure *rook,
               int rookDestCoord);
    ~CastleMove() override = default;
    //
    std::unique_ptr<Board> execute(Board &board) const override = 0;
};

class KingSideCastleMove : public CastleMove {
  public:
    using CastleMove::CastleMove;
    //
    std::unique_ptr<Board> execute(Board &board) const override;
};

class QueenSideCastleMove : public CastleMove {
  public:
    using CastleMove::CastleMove;
    //
    std::unique_ptr<Board> execute(Board &board) const override;
};

#endif