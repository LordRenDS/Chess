#include "board_utils.h"

const std::array<bool, BoardUtils::NUMBER_SQUARES> BoardUtils::EIGHTH_RANK =
    initRow(0);
const std::array<bool, BoardUtils::NUMBER_SQUARES> BoardUtils::SEVENTH_RANK =
    initRow(8);
const std::array<bool, BoardUtils::NUMBER_SQUARES> BoardUtils::SIXTH_RANK =
    initRow(16);
const std::array<bool, BoardUtils::NUMBER_SQUARES> BoardUtils::FIFTH_RANK =
    initRow(24);
const std::array<bool, BoardUtils::NUMBER_SQUARES> BoardUtils::FOURTH_RANK =
    initRow(32);
const std::array<bool, BoardUtils::NUMBER_SQUARES> BoardUtils::THIRD_RANK =
    initRow(40);
const std::array<bool, BoardUtils::NUMBER_SQUARES> BoardUtils::SECOND_RANK =
    initRow(48);
const std::array<bool, BoardUtils::NUMBER_SQUARES> BoardUtils::FIRST_RANK =
    initRow(56);
const std::array<bool, BoardUtils::NUMBER_SQUARES> BoardUtils::FIRST_COLUMN =
    initRow(0);
const std::array<bool, BoardUtils::NUMBER_SQUARES> BoardUtils::SECOND_COLUMN =
    initColumn(1);
const std::array<bool, BoardUtils::NUMBER_SQUARES> BoardUtils::SEVENTH_COLUMN =
    initColumn(6);
const std::array<bool, BoardUtils::NUMBER_SQUARES> BoardUtils::EIGHT_COLUMN =
    initColumn(7);
const std::array<std::string, BoardUtils::NUMBER_SQUARES>
    BoardUtils::ALGEBRAIC_NOTATION = {
        "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8", "a7", "b7", "c7",
        "d7", "e7", "f7", "g7", "h7", "a6", "b6", "c6", "d6", "e6", "f6",
        "g6", "h6", "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5", "a4",
        "b4", "c4", "d4", "e4", "f4", "g4", "h4", "a3", "b3", "c3", "d3",
        "e3", "f3", "g3", "h3", "a2", "b2", "c2", "d2", "e2", "f2", "g2",
        "h2", "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"};
const std::map<std::string, int> BoardUtils::POSITION_TO_COORDINATE =
    initializePositionToCoordinate();

int BoardUtils::getCoordinateAtPosition(const std::string &position) {
    return POSITION_TO_COORDINATE.at(position);
}

std::string BoardUtils::getPositionAtCoordinate(int coordinate) {
    return ALGEBRAIC_NOTATION.at(coordinate);
}

bool BoardUtils::isValidSquareCoordinate(int coordinate) {
    return coordinate >= 0 && coordinate < NUMBER_SQUARES;
}

std::array<bool, 64> BoardUtils::initRow(int rowNumber) {
    std::array<bool, NUMBER_SQUARES> row{};
    do {
        row[rowNumber] = true;
        rowNumber++;
    } while (rowNumber % NUMBER_SQUARE_PER_ROW);
    return row;
}

std::array<bool, BoardUtils::NUMBER_SQUARES>
BoardUtils::initColumn(int colNumber) {
    std::array<bool, NUMBER_SQUARES> column{};
    do {
        column[colNumber] = true;
        colNumber += NUMBER_SQUARE_PER_ROW;
    } while (colNumber < NUMBER_SQUARES);
    return column;
}

std::map<std::string, int> BoardUtils::initializePositionToCoordinate() {
    std::map<std::string, int> positionToCoordinate;
    for (int i{0}; i < NUMBER_SQUARES; i++) {
        positionToCoordinate[ALGEBRAIC_NOTATION[i]] = i;
    }
    return positionToCoordinate;
}