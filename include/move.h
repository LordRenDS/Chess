#ifndef MOVE_H
#define MOVE_H
#include <memory>

class Figure;
class Board;

class Move {
  protected:
    const Figure *movedFigure{};
    const int coordinateToMove{};
    //
    Move() = default;

  public:
    explicit Move(const Figure *figure, int coordinateToMove);
    Move(const Move &move) = default;
    virtual ~Move() = default;
    //
    int getCoordinateToMove() const;
    const Figure *getMovedFigure() const;
    //
    virtual std::unique_ptr<Board> execute(Board &board) const = 0;
    virtual std::unique_ptr<Move> clone() const = 0;
    virtual bool equals(const Move &other) const;
};

class MajorMove : public Move {
  public:
    using Move::Move;
    MajorMove() = delete;
    ~MajorMove() override = default;
    //
    std::unique_ptr<Board> execute(Board &board) const override;
    std::unique_ptr<Move> clone() const override;
};

class AttackMove : public Move {
  protected:
    const Figure *attackFigure;
    AttackMove(const Figure *figure, const Figure *attackFigure,
               int coordinateToMove);

  public:
    AttackMove(const Figure *figure, const Figure *attackFigure);
    AttackMove(const AttackMove &attackMove) = default;
    ~AttackMove() override = default;
    //
    const Figure *getAttackFigure() const;
    //
    std::unique_ptr<Board> execute(Board &board) const override = 0;
    std::unique_ptr<Move> clone() const override = 0;
    bool equals(const Move &other) const override;
};

class MajorAttackMove : public AttackMove {
  public:
    using AttackMove::AttackMove;
    ~MajorAttackMove() override = default;
    //
    std::unique_ptr<Board> execute(Board &board) const override;
    std::unique_ptr<Move> clone() const override;
};

class PawnAttackMove : public AttackMove {
  public:
    using AttackMove::AttackMove;
    ~PawnAttackMove() override = default;
    //
    std::unique_ptr<Board> execute(Board &board) const override;
    std::unique_ptr<Move> clone() const override;
};

class PawnEnPassantAttackMove : public PawnAttackMove {
  public:
    explicit PawnEnPassantAttackMove(const Figure *pawn,
                                     const Figure *enPassantPawn,
                                     int coordinateToMove);
    ~PawnEnPassantAttackMove() override = default;
    //
    std::unique_ptr<Board> execute(Board &board) const override;
    std::unique_ptr<Move> clone() const override;
};

class PawnMove : public Move {
  public:
    using Move::Move;
    PawnMove() = delete;
    ~PawnMove() override = default;
    //
    std::unique_ptr<Board> execute(Board &board) const override;
    std::unique_ptr<Move> clone() const override;
};

class PawnJump : public Move {
  public:
    using Move::Move;
    PawnJump() = delete;
    ~PawnJump() override = default;
    //
    std::unique_ptr<Board> execute(Board &board) const override;
    std::unique_ptr<Move> clone() const override;
};

class PawnPromotion : public Move {
  private:
    std::unique_ptr<Figure> promotionFigure;
    std::unique_ptr<Move> decoratedMove;

  public:
    explicit PawnPromotion(std::unique_ptr<Move> decoratedMove);
    PawnPromotion(const PawnPromotion &pawnPromotion);
    ~PawnPromotion() override = default;
    //
    void setPromotionFigure(std::unique_ptr<Figure> figure);
    //
    std::unique_ptr<Board> execute(Board &board) const override;
    std::unique_ptr<Move> clone() const override;
    bool equals(const Move &other) const override;
};

class CastleMove : public Move {
  private:
    const Figure *rook{};
    const int rookDestinationCoordinate{};

  public:
    CastleMove(const Figure *king, int kingDestCoord, const Figure *rook,
               int rookDestCoord);
    CastleMove(const CastleMove &castleMove) = default;
    ~CastleMove() override = default;
    //
    std::unique_ptr<Board> execute(Board &board) const override = 0;
    std::unique_ptr<Move> clone() const override = 0;
    bool equals(const Move &other) const override;
};

class KingSideCastleMove : public CastleMove {
  public:
    using CastleMove::CastleMove;
    ~KingSideCastleMove() override = default;
    //
    std::unique_ptr<Board> execute(Board &board) const override;
    std::unique_ptr<Move> clone() const override;
};

class QueenSideCastleMove : public CastleMove {
  public:
    using CastleMove::CastleMove;
    ~QueenSideCastleMove() override = default;
    //
    std::unique_ptr<Board> execute(Board &board) const override;
    std::unique_ptr<Move> clone() const override;
};

#endif