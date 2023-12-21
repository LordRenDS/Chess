#include "move.h"
#include "board.h"
#include "figure.h"

Move::Move(const Figure *figure, int coordinateToMove)
    : movedFigure(figure), coordinateToMove(coordinateToMove) {
}

int Move::getCoordinateToMove() const {
    return coordinateToMove;
}

const Figure *Move::getMovedFigure() const {
    return movedFigure;
}

std::unique_ptr<Board> Move::execute(Board &board) const {
    std::unique_ptr<Board> movedBoard(std::make_unique<Board>(board));
    movedBoard->getSquare(movedFigure->getCoordinate())
        ->getFigureOnSquare()
        ->move(coordinateToMove, *movedBoard);
    if (movedBoard->getEnPassantPawn())
        movedBoard->setEnPassantPawn(nullptr);
    return movedBoard;
}

bool Move::equals(const Move &other) const {
    return this == &other || (typeid(*this) == typeid(other) &&
                              movedFigure->equals(*other.movedFigure) &&
                              coordinateToMove == other.coordinateToMove);
}

std::unique_ptr<Board> MajorMove::execute(Board &board) const {
    return Move::execute(board);
}

std::unique_ptr<Move> MajorMove::clone() const {
    return std::make_unique<MajorMove>(*this);
}

AttackMove::AttackMove(const Figure *figure, const Figure *attackFigure)
    : Move(figure, attackFigure->getCoordinate()), attackFigure(attackFigure) {
}

AttackMove::AttackMove(const Figure *figure, const Figure *attackFigure,
                       int coordinateToMove)
    : Move(figure, coordinateToMove), attackFigure(attackFigure) {
}

const Figure *AttackMove::getAttackFigure() const {
    return attackFigure;
}
std::unique_ptr<Board> AttackMove::execute(Board &board) const {
    // std::unique_ptr<Board> movedBoard(std::make_unique<Board>(board));
    // movedBoard->getSquare(attackFigure->getCoordinate())
    //     ->releaseFigure()
    //     .reset();
    // movedBoard->getSquare(movedFigure->getCoordinate())
    //     ->getFigureOnSquare()
    //     ->move(coordinateToMove, *movedBoard);
    // return movedBoard;
    return Move::execute(board);
}

bool AttackMove::equals(const Move &other) const {
    if (this == &other)
        return true;
    try {
        auto &otherAttack{dynamic_cast<const AttackMove &>(other)};
        return (Move::equals(other) &&
                attackFigure->equals(*otherAttack.attackFigure));
    } catch (std::bad_cast &e) {
        return false;
    }
}

std::unique_ptr<Board> MajorAttackMove::execute(Board &board) const {
    return AttackMove::execute(board);
}

std::unique_ptr<Move> MajorAttackMove::clone() const {
    return std::make_unique<MajorAttackMove>(*this);
}

std::unique_ptr<Board> PawnAttackMove::execute(Board &board) const {
    return AttackMove::execute(board);
}

std::unique_ptr<Move> PawnAttackMove::clone() const {
    return std::make_unique<PawnAttackMove>(*this);
}

std::unique_ptr<Board> PawnMove::execute(Board &board) const {
    return Move::execute(board);
}

std::unique_ptr<Move> PawnMove::clone() const {
    return std::make_unique<PawnMove>(*this);
}

PawnEnPassantAttackMove::PawnEnPassantAttackMove(const Figure *pawn,
                                                 const Figure *enPassantPawn,
                                                 int coordinateToMove)
    : PawnAttackMove(pawn, enPassantPawn, coordinateToMove) {
}

std::unique_ptr<Board> PawnEnPassantAttackMove::execute(Board &board) const {
    auto movedBoard(PawnAttackMove::execute(board));
    movedBoard->getSquare(attackFigure->getCoordinate())->releaseFigure();
    return movedBoard;
}

std::unique_ptr<Move> PawnEnPassantAttackMove::clone() const {
    return std::make_unique<PawnEnPassantAttackMove>(*this);
}

std::unique_ptr<Board> PawnJump::execute(Board &board) const {
    auto movedBoard(Move::execute(board));
    movedBoard->setEnPassantPawn(dynamic_cast<Pawn *>(
        movedBoard->getSquare(coordinateToMove)->getFigureOnSquare()));
    return movedBoard;
}

std::unique_ptr<Move> PawnJump::clone() const {
    return std::make_unique<PawnJump>(*this);
}

PawnPromotion::PawnPromotion(std::unique_ptr<Move> decoratedMove)
    : Move(decoratedMove->getMovedFigure(),
           decoratedMove->getCoordinateToMove()),
      decoratedMove(std::move(decoratedMove)),
      promotionFigure(
          std::make_unique<Queen>(coordinateToMove, movedFigure->getColor())) {
}

PawnPromotion::PawnPromotion(const PawnPromotion &pawnPromotion)
    : Move(pawnPromotion),
      promotionFigure(pawnPromotion.promotionFigure
                          ? pawnPromotion.promotionFigure->clone()
                          : nullptr),
      decoratedMove(pawnPromotion.decoratedMove->clone()) {
}

void PawnPromotion::setPromotionFigure(std::unique_ptr<Figure> figure) {
    promotionFigure = std::move(figure);
}

std::unique_ptr<Board> PawnPromotion::execute(Board &board) const {
    std::unique_ptr<Board> movedBoard(decoratedMove->execute(board));
    movedBoard->getSquare(decoratedMove->getCoordinateToMove())
        ->setFigureOnSquare(promotionFigure->clone());
    return movedBoard;
}

std::unique_ptr<Move> PawnPromotion::clone() const {
    return std::make_unique<PawnPromotion>(*this);
}

bool PawnPromotion::equals(const Move &other) const {
    if (this == &other)
        return true;
    try {
        auto &otherPromotion = dynamic_cast<const PawnPromotion &>(other);
        return (Move::equals(other) &&
                otherPromotion.promotionFigure->equals(*promotionFigure) &&
                otherPromotion.decoratedMove->equals(*decoratedMove));
    } catch (std::bad_cast &e) {
        return false;
    }
}

CastleMove::CastleMove(const Figure *king, int kingDestCoord,
                       const Figure *rook, int rookDestCoord)
    : Move(king, kingDestCoord), rook(rook),
      rookDestinationCoordinate(rookDestCoord) {
}

std::unique_ptr<Board> CastleMove::execute(Board &board) const {
    //    std::unique_ptr<Board> movedBoard(std::make_unique<Board>(board));
    //    std::unique_ptr<Figure> movedRook(
    //        movedBoard->getSquare(rook->getCoordinate())->releaseFigure());
    //    movedBoard->getSquare(movedFigure->getCoordinate())
    //        ->getFigureOnSquare()
    //        ->move(coordinateToMove, *movedBoard);
    //    movedRook->move(rookDestinationCoordinate, *movedBoard);
    auto movedBoard(Move::execute(board));
    movedBoard->getSquare(rook->getCoordinate())
        ->getFigureOnSquare()
        ->move(rookDestinationCoordinate, *movedBoard);
    return movedBoard;
}

bool CastleMove::equals(const Move &other) const {
    if (this == &other)
        return true;
    try {
        auto &otherCastle = dynamic_cast<const CastleMove &>(other);
        return (Move::equals(other) && rook->equals(*otherCastle.rook) &&
                rookDestinationCoordinate ==
                    otherCastle.rookDestinationCoordinate);
    } catch (std::bad_cast &e) {
        return false;
    }
}

std::unique_ptr<Board> KingSideCastleMove::execute(Board &board) const {
    return CastleMove::execute(board);
}

std::unique_ptr<Move> KingSideCastleMove::clone() const {
    return std::make_unique<KingSideCastleMove>(*this);
}

std::unique_ptr<Board> QueenSideCastleMove::execute(Board &board) const {
    return CastleMove::execute(board);
}

std::unique_ptr<Move> QueenSideCastleMove::clone() const {
    return std::make_unique<QueenSideCastleMove>(*this);
}
