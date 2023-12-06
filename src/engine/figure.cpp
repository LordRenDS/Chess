#include "figure.h"
#include "board.h"
#include "board_utils.h"
#include "move.h"

Figure::Figure(int coordinate, Color::ColorT color, FigureType figureType)
    : coordinate(coordinate), color(color), figureType(figureType) {
}

int Figure::getCoordinate() {
    return coordinate;
}

FigureType Figure::getFigureType() {
    return figureType;
}

Color::ColorT Figure::getColor() const {
    return color;
};

void Figure::move(int coordinate, Board &board) {
    board.getSquare(coordinate)
        ->setFigureOnSquare(board.getSquare(this->coordinate)->releaseFigure());
    this->coordinate = coordinate;
}

bool Figure::isFirstMove() const {
    return firstMove;
};

const int King::CANDIDATE_MOVE_COORDINATES[] = {-9, -8, -7, -1, 1, 7, 8, 9};

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
King::calculateLegalMoves(const Board &board) {
    // std::vector<std::unique_ptr<Move>> legalMoves;
    // for (auto &currentCandidateOffset : CANDIDATE_MOVE_COORDINATES) {
    //     if (isFirstColumnExclusion(coordinate, currentCandidateOffset) ||
    //         isEightColumnExclusion(coordinate, currentCandidateOffset)) {
    //         continue;
    //     }
    //     const int candidateDestinationCoordinate{coordinate +
    //                                              currentCandidateOffset};
    //     if (BoardUtils::isValidSquareCoordinate(
    //             candidateDestinationCoordinate)) {
    //         const Square *candidateDestinationSquare =
    //             board.getSquare(candidateDestinationCoordinate);
    //         if (!candidateDestinationSquare->isSquareOccupie()) {
    //             legalMoves.push_back(std::make_unique<MajoreMove>());
    //         } else {
    //             const Figure *figureAtDestination{
    //                 candidateDestinationSquare->getFigureOnSquare()};
    //             const Color::ColorT figureColor{
    //                 figureAtDestination->getColor()};
    //             if (this->color != figureColor) {
    //                 legalMoves.push_back(std::make_unique<MajorAttacMove>());
    //             }
    //         }
    //     }
    // }
    // return legalMoves;
}

bool King::isCastled() {
    return castled;
}

std::string King::getFigureName() {
    return Color::getColorName(this->color) + "K";
}

std::unique_ptr<Figure> King::clone() const {
    return std::make_unique<King>(*this);
}

Queen::Queen(int coordinate, Color::ColorT color)
    : Figure(coordinate, color, FigureType::QUEEN) {
}

std::vector<std::unique_ptr<Move>>
Queen::calculateLegalMoves(const Board &board) {
    // todo
}

std::string Queen::getFigureName() {
    return Color::getColorName(this->color) + "Q";
}

std::unique_ptr<Figure> Queen::clone() const {
    return std::make_unique<Queen>(*this);
}

Rook::Rook(int coordinate, Color::ColorT color)
    : Figure(coordinate, color, FigureType::ROOK) {
}

std::vector<std::unique_ptr<Move>>
Rook::calculateLegalMoves(const Board &board) {
    // todo
}

std::string Rook::getFigureName() {
    return Color::getColorName(this->color) + "R";
}

std::unique_ptr<Figure> Rook::clone() const {
    return std::make_unique<Rook>(*this);
}

Knight::Knight(int coordinate, Color::ColorT color)
    : Figure(coordinate, color, FigureType::KNIGHT) {
}

std::vector<std::unique_ptr<Move>>
Knight::calculateLegalMoves(const Board &board) {
    // todo
}

std::string Knight::getFigureName() {
    return Color::getColorName(this->color) + "Kn";
}

std::unique_ptr<Figure> Knight::clone() const {
    return std::make_unique<Knight>(*this);
}

Bishop::Bishop(int coordinate, Color::ColorT color)
    : Figure(coordinate, color, FigureType::BISHOP) {
}

std::vector<std::unique_ptr<Move>>
Bishop::calculateLegalMoves(const Board &board) {
    // todo
}

std::string Bishop::getFigureName() {
    return Color::getColorName(this->color) + "B";
}

std::unique_ptr<Figure> Bishop::clone() const {
    return std::make_unique<Bishop>(*this);
}

Pawn::Pawn(int coordinate, Color::ColorT color)
    : Figure(coordinate, color, FigureType::PAWN) {
}

std::vector<std::unique_ptr<Move>>
Pawn::calculateLegalMoves(const Board &board) {
    // todo
}

std::string Pawn::getFigureName() {
    return Color::getColorName(this->color) + "P";
}

std::unique_ptr<Figure> Pawn::clone() const {
    return std::make_unique<Pawn>(*this);
}

bool Pawn::isEnPassant() {
    return enPassant;
}
