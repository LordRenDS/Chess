#include "figure.h"
#include "board.h"
#include "board_utils.h"
#include "figure_type.h"
#include "move.h"

Figure::Figure(int coordinate, Color::ColorT color, FigureType figureType)
    : coordinate(coordinate), color(color), figureType(figureType) {
}

int Figure::getCoordinate() const {
    return coordinate;
}

FigureType Figure::getFigureType() const {
    return figureType;
}

Color::ColorT Figure::getColor() const {
    return color;
};

void Figure::move(int coordinate, Board &board) {
    board.getSquare(coordinate)
        ->setFigureOnSquare(board.getSquare(this->coordinate)->releaseFigure());
    this->coordinate = coordinate;
    if (firstMove)
        firstMove = false;
}

bool Figure::isFirstMove() const {
    return firstMove;
};

bool King::isFirstColumnExclusion(int currentPosition, int candidateOffset) {
    return BoardUtils::FIRST_COLUMN[currentPosition] &&
           (candidateOffset == -9 || candidateOffset == -1 ||
            candidateOffset == 7);
}

bool King::isEightColumnExclusion(int currentPosition, int candidateOffset) {
    return BoardUtils::EIGHT_COLUMN[currentPosition] &&
           (candidateOffset == -7 || candidateOffset == 1 ||
            candidateOffset == 9);
}

King::King(int coordinate, Color::ColorT color)
    : Figure(coordinate, color, FigureType::KING) {
}

std::vector<std::unique_ptr<Move>>
King::calculateLegalMoves(Board &board) const {
    std::vector<std::unique_ptr<Move>> legalMoves;
    for (auto &currentCandidateOffset : CANDIDATE_MOVE_COORDINATES) {
        if (isFirstColumnExclusion(coordinate, currentCandidateOffset) ||
            isEightColumnExclusion(coordinate, currentCandidateOffset)) {
            continue;
        }
        const int candidateDestinationCoordinate{coordinate +
                                                 currentCandidateOffset};
        if (BoardUtils::isValidSquareCoordinate(
                candidateDestinationCoordinate)) {
            Square *candidateDestinationSquare =
                board.getSquare(candidateDestinationCoordinate);
            if (!candidateDestinationSquare->isSquareOccupied()) {
                legalMoves.push_back(std::make_unique<MajorMove>(
                    this, candidateDestinationCoordinate));
            } else {
                Figure *figureAtDestination{
                    candidateDestinationSquare->getFigureOnSquare()};
                const Color::ColorT figureColor{
                    figureAtDestination->getColor()};
                if (this->color != figureColor) {
                    legalMoves.push_back(std::make_unique<MajorAttackMove>(
                        this, figureAtDestination));
                }
            }
        }
    }
    return legalMoves;
}

bool King::isCastled() const {
    return castled;
}

bool King::isInCheck() const {
    return inCheck;
}

std::string King::getFigureName() const {
    return Color::getColorName(this->color) + "K";
}

std::unique_ptr<Figure> King::clone() const {
    return std::make_unique<King>(*this);
}

Queen::Queen(int coordinate, Color::ColorT color)
    : Figure(coordinate, color, FigureType::QUEEN) {
}

bool Queen::isFirstColumnExclusion(int currentPosition, int candidateOffset) {
    return BoardUtils::FIRST_COLUMN[currentPosition] &&
           (candidateOffset == -9 || candidateOffset == -1 ||
            candidateOffset == 7);
}

bool Queen::isEightColumnExclusion(int currentPosition, int candidateOffset) {
    return BoardUtils::EIGHT_COLUMN[currentPosition] &&
           (candidateOffset == -7 || candidateOffset == 1 ||
            candidateOffset == 9);
}

std::vector<std::unique_ptr<Move>>
Queen::calculateLegalMoves(Board &board) const {
    std::vector<std::unique_ptr<Move>> legalMoves;
    for (int currentCandidateOffset : CANDIDATE_MOVE_VECTOR_COORDINATES) {
        int candidateDestinationCoordinate{coordinate};
        while (BoardUtils::isValidSquareCoordinate(
            candidateDestinationCoordinate)) {
            if (isFirstColumnExclusion(candidateDestinationCoordinate,
                                       currentCandidateOffset) ||
                isEightColumnExclusion(candidateDestinationCoordinate,
                                       currentCandidateOffset)) {
                break;
            }
            candidateDestinationCoordinate += currentCandidateOffset;
            if (BoardUtils::isValidSquareCoordinate(
                    candidateDestinationCoordinate)) {
                Square *candidateDestinationSquare =
                    board.getSquare(candidateDestinationCoordinate);
                if (!candidateDestinationSquare->isSquareOccupied()) {
                    legalMoves.push_back(std::make_unique<MajorMove>(
                        this, candidateDestinationCoordinate));
                } else {
                    Figure *figureAtDestination{
                        candidateDestinationSquare->getFigureOnSquare()};
                    Color::ColorT figureColor{figureAtDestination->getColor()};
                    if (color != figureColor) {
                        legalMoves.push_back(std::make_unique<MajorAttackMove>(
                            this, figureAtDestination));
                    }
                    break;
                }
            }
        }
    }
    return legalMoves;
}

std::string Queen::getFigureName() const {
    return Color::getColorName(this->color) + "Q";
}

std::unique_ptr<Figure> Queen::clone() const {
    return std::make_unique<Queen>(*this);
}

Rook::Rook(int coordinate, Color::ColorT color)
    : Figure(coordinate, color, FigureType::ROOK) {
}

bool Rook::isFirstColumnExclusion(int currentPosition, int candidateOffset) {
    return BoardUtils::FIRST_COLUMN[currentPosition] && (candidateOffset == -1);
}

bool Rook::isEightColumnExclusion(int currentPosition, int candidateOffset) {
    return BoardUtils::EIGHT_COLUMN[currentPosition] && (candidateOffset == 1);
}

std::vector<std::unique_ptr<Move>>
Rook::calculateLegalMoves(Board &board) const {
    std::vector<std::unique_ptr<Move>> legalMoves;
    for (int currentCandidateOffset : CANDIDATE_MOVE_VECTOR_COORDINATES) {
        int candidateDestinationCoordinate{coordinate};
        while (BoardUtils::isValidSquareCoordinate(
            candidateDestinationCoordinate)) {
            if (isFirstColumnExclusion(candidateDestinationCoordinate,
                                       currentCandidateOffset) ||
                isEightColumnExclusion(candidateDestinationCoordinate,
                                       currentCandidateOffset)) {
                break;
            }
            candidateDestinationCoordinate += currentCandidateOffset;
            if (BoardUtils::isValidSquareCoordinate(
                    candidateDestinationCoordinate)) {
                Square *candidateDestinationSquare =
                    board.getSquare(candidateDestinationCoordinate);
                if (!candidateDestinationSquare->isSquareOccupied()) {
                    legalMoves.push_back(std::make_unique<MajorMove>(
                        this, candidateDestinationCoordinate));
                } else {
                    Figure *figureAtDestination{
                        candidateDestinationSquare->getFigureOnSquare()};
                    Color::ColorT figureColor{figureAtDestination->getColor()};
                    if (color != figureColor) {
                        legalMoves.push_back(std::make_unique<MajorAttackMove>(
                            this, figureAtDestination));
                    }
                    break;
                }
            }
        }
    }
    return legalMoves;
}

std::string Rook::getFigureName() const {
    return Color::getColorName(this->color) + "R";
}

std::unique_ptr<Figure> Rook::clone() const {
    return std::make_unique<Rook>(*this);
}

Knight::Knight(int coordinate, Color::ColorT color)
    : Figure(coordinate, color, FigureType::KNIGHT) {
}

bool Knight::isFirstColumnExclusion(int currentPosition, int candidateOffset) {
    return BoardUtils::FIRST_COLUMN[currentPosition] &&
           ((candidateOffset == -17) || (candidateOffset == -10) ||
            (candidateOffset == 6) || (candidateOffset == 15));
}

bool Knight::isSecondColumnExclusion(int currentPosition, int candidateOffset) {
    return BoardUtils::SECOND_COLUMN[currentPosition] &&
           ((candidateOffset == -10) || (candidateOffset == 6));
}

bool Knight::isSeventhColumnExclusion(int currentPosition,
                                      int candidateOffset) {
    return BoardUtils::SEVENTH_COLUMN[currentPosition] &&
           ((candidateOffset == -6) || (candidateOffset == 10));
}

bool Knight::isEightColumnExclusion(int currentPosition, int candidateOffset) {
    return BoardUtils::EIGHT_COLUMN[currentPosition] &&
           ((candidateOffset == -15) || (candidateOffset == -6) ||
            (candidateOffset == 10) || (candidateOffset == 17));
}

std::vector<std::unique_ptr<Move>>
Knight::calculateLegalMoves(Board &board) const {
    std::vector<std::unique_ptr<Move>> legalMoves;
    for (auto &currentCandidateOffset : CANDIDATE_MOVE_COORDINATES) {
        if (isFirstColumnExclusion(coordinate, currentCandidateOffset) ||
            isSecondColumnExclusion(coordinate, currentCandidateOffset) ||
            isSeventhColumnExclusion(coordinate, currentCandidateOffset) ||
            isEightColumnExclusion(coordinate, currentCandidateOffset)) {
            continue;
        }
        const int candidateDestinationCoordinate{coordinate +
                                                 currentCandidateOffset};
        if (BoardUtils::isValidSquareCoordinate(
                candidateDestinationCoordinate)) {
            Square *candidateDestinationSquare{
                board.getSquare(candidateDestinationCoordinate)};
            if (!candidateDestinationSquare->isSquareOccupied()) {
                legalMoves.push_back(std::make_unique<MajorMove>(
                    this, candidateDestinationCoordinate));
            } else {
                Figure *figureAtDestination{
                    candidateDestinationSquare->getFigureOnSquare()};
                Color::ColorT figureColor{figureAtDestination->getColor()};
                if (color != figureColor) {
                    legalMoves.push_back(std::make_unique<MajorAttackMove>(
                        this, figureAtDestination));
                }
            }
        }
    }
    return legalMoves;
}

std::string Knight::getFigureName() const {
    return Color::getColorName(this->color) + "Kn";
}

std::unique_ptr<Figure> Knight::clone() const {
    return std::make_unique<Knight>(*this);
}

Bishop::Bishop(int coordinate, Color::ColorT color)
    : Figure(coordinate, color, FigureType::BISHOP) {
}

bool Bishop::isFirstColumnExclusion(int currentPosition, int candidateOffset) {
    return BoardUtils::FIRST_COLUMN[currentPosition] &&
           (candidateOffset == -9 || candidateOffset == 7);
}

bool Bishop::isEightColumnExclusion(int currentPosition, int candidateOffset) {
    return BoardUtils::EIGHT_COLUMN[currentPosition] &&
           (candidateOffset == -7 || candidateOffset == 9);
}

std::vector<std::unique_ptr<Move>>
Bishop::calculateLegalMoves(Board &board) const {
    std::vector<std::unique_ptr<Move>> legalMoves;
    for (int currentCandidateOffset : CANDIDATE_MOVE_VECTOR_COORDINATES) {
        int candidateDestinationCoordinate{coordinate};
        while (BoardUtils::isValidSquareCoordinate(
            candidateDestinationCoordinate)) {
            if (isFirstColumnExclusion(candidateDestinationCoordinate,
                                       currentCandidateOffset) ||
                isEightColumnExclusion(candidateDestinationCoordinate,
                                       currentCandidateOffset)) {
                break;
            }
            candidateDestinationCoordinate += currentCandidateOffset;
            if (BoardUtils::isValidSquareCoordinate(
                    candidateDestinationCoordinate)) {
                Square *candidateDestinationSquare =
                    board.getSquare(candidateDestinationCoordinate);
                if (!candidateDestinationSquare->isSquareOccupied()) {
                    legalMoves.push_back(std::make_unique<MajorMove>(
                        this, candidateDestinationCoordinate));
                } else {
                    Figure *figureAtDestination{
                        candidateDestinationSquare->getFigureOnSquare()};
                    Color::ColorT figureColor{figureAtDestination->getColor()};
                    if (color != figureColor) {
                        legalMoves.push_back(std::make_unique<MajorAttackMove>(
                            this, figureAtDestination));
                    }
                    break;
                }
            }
        }
    }
    return legalMoves;
}

std::string Bishop::getFigureName() const {
    return Color::getColorName(this->color) + "B";
}

std::unique_ptr<Figure> Bishop::clone() const {
    return std::make_unique<Bishop>(*this);
}

Pawn::Pawn(int coordinate, Color::ColorT color)
    : Figure(coordinate, color, FigureType::PAWN) {
}

bool Pawn::isHasEnPassantMove(Board &board, int enPassantCoordinate) const {
    Square *enPassantSquare{board.getSquare(
        Color::getOppositeDirection(color) * BoardUtils::NUMBER_SQUARE_PER_ROW +
        enPassantCoordinate)};
    if (enPassantSquare->isSquareOccupied() &&
        enPassantSquare->getFigureOnSquare()->getColor() != color) {
        const Pawn *enPassantPawn{
            dynamic_cast<Pawn *>(enPassantSquare->getFigureOnSquare())};
        if (enPassantPawn && enPassantPawn == board.getEnPassantPawn())
            return true;
    }
    return false;
}

std::vector<std::unique_ptr<Move>>
Pawn::calculateLegalMoves(Board &board) const {
    std::vector<std::unique_ptr<Move>> legalMoves;
    for (auto &currentCandidateOffset : CANDIDATE_MOVE_COORDINATES) {
        const int candidateDestinationCoordinate{
            coordinate + (currentCandidateOffset * Color::getDirection(color))};
        if (!BoardUtils::isValidSquareCoordinate(
                candidateDestinationCoordinate)) {
            continue;
        }
        if (currentCandidateOffset == 8 &&
            !board.getSquare(candidateDestinationCoordinate)
                 ->isSquareOccupied()) {
            if (Color::isPawnPromotionSquare(color,
                                             candidateDestinationCoordinate)) {
                legalMoves.push_back(
                    std::make_unique<PawnPromotion>(std::make_unique<PawnMove>(
                        this, candidateDestinationCoordinate)));
            } else {
                legalMoves.push_back(std::make_unique<PawnMove>(
                    this, candidateDestinationCoordinate));
            }
        } else if (currentCandidateOffset == 16 && isFirstMove() &&
                   ((BoardUtils::SEVENTH_RANK[coordinate] &&
                     color == Color::ColorT::BLACK) ||
                    (BoardUtils::SECOND_RANK[coordinate] &&
                     color == Color::ColorT::WHITE))) {
            const int behindCandidateDestinationCoordinate{
                coordinate + (Color::getDirection(color) * 8)};
            if (!board.getSquare(behindCandidateDestinationCoordinate)
                     ->isSquareOccupied() &&
                !board.getSquare(candidateDestinationCoordinate)
                     ->isSquareOccupied()) {
                legalMoves.push_back(std::make_unique<PawnJump>(
                    this, candidateDestinationCoordinate));
            }
        } else if (currentCandidateOffset == 7 &&
                   !((BoardUtils::EIGHT_COLUMN[coordinate] &&
                      color == Color::ColorT::WHITE) ||
                     (BoardUtils::FIRST_COLUMN[coordinate] &&
                      color == Color::ColorT::BLACK))) {
            if (board.getSquare(candidateDestinationCoordinate)
                    ->isSquareOccupied()) {
                Figure *figureOnCandidate{
                    board.getSquare(candidateDestinationCoordinate)
                        ->getFigureOnSquare()};
                if (color != figureOnCandidate->getColor()) {
                    if (Color::isPawnPromotionSquare(
                            color, candidateDestinationCoordinate)) {
                        legalMoves.push_back(std::make_unique<PawnPromotion>(
                            std::make_unique<PawnAttackMove>(
                                this, figureOnCandidate)));
                    } else {
                        legalMoves.push_back(std::make_unique<PawnAttackMove>(
                            this, figureOnCandidate));
                    }
                }
            } else if (isHasEnPassantMove(board,
                                          candidateDestinationCoordinate)) {
                Figure *figureOnCandidate =
                    board
                        .getSquare(coordinate +
                                   Color::getOppositeDirection(color))
                        ->getFigureOnSquare();
                legalMoves.push_back(std::make_unique<PawnEnPassantAttackMove>(
                    this, figureOnCandidate,
                        candidateDestinationCoordinate));
            }
        } else if (currentCandidateOffset == 9 &&
                   !((BoardUtils::EIGHT_COLUMN[coordinate] &&
                      color == Color::ColorT::BLACK) ||
                     (BoardUtils::FIRST_COLUMN[coordinate] &&
                      color == Color::ColorT::WHITE))) {
            if (board.getSquare(candidateDestinationCoordinate)
                    ->isSquareOccupied()) {
                Figure *figureOnCandidate =
                    board.getSquare(candidateDestinationCoordinate)
                        ->getFigureOnSquare();
                if (color != figureOnCandidate->getColor())
                    if (Color::isPawnPromotionSquare(
                            color, candidateDestinationCoordinate)) {
                        legalMoves.push_back(std::make_unique<PawnPromotion>(
                            std::make_unique<PawnAttackMove>(
                                this, figureOnCandidate)));
                    } else {
                        legalMoves.push_back(std::make_unique<PawnAttackMove>(
                            this, figureOnCandidate));
                    }
            } else if (isHasEnPassantMove(board,
                                          candidateDestinationCoordinate)) {
                Figure *figureOnCandidate =
                    board
                        .getSquare(coordinate -
                                   Color::getOppositeDirection(color))
                        ->getFigureOnSquare();
                legalMoves.push_back(std::make_unique<PawnEnPassantAttackMove>(
                    this, figureOnCandidate, candidateDestinationCoordinate));
            }
        }
    }
    return legalMoves;
}

std::string Pawn::getFigureName() const {
    return Color::getColorName(this->color) + "P";
}

std::unique_ptr<Figure> Pawn::clone() const {
    return std::make_unique<Pawn>(*this);
}
