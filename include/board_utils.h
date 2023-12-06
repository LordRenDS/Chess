#ifndef BOARD_UTILS_H
#define BOARD_UTILS_H
#include <array>
#include <map>
#include <string>

class BoardUtils {
  public:
    static const int NUMBER_SQUARES{64};
    static const int NUMBER_SQUARE_PER_ROW{8};
    static const std::array<bool, NUMBER_SQUARES> EIGHTH_RANK;
    static const std::array<bool, NUMBER_SQUARES> SEVENTH_RANK;
    static const std::array<bool, NUMBER_SQUARES> SIXTH_RANK;
    static const std::array<bool, NUMBER_SQUARES> FIFTH_RANK;
    static const std::array<bool, NUMBER_SQUARES> FOURTH_RANK;
    static const std::array<bool, NUMBER_SQUARES> THIRD_RANK;
    static const std::array<bool, NUMBER_SQUARES> SECOND_RANK;
    static const std::array<bool, NUMBER_SQUARES> FIRST_RANK;
    static const std::array<bool, NUMBER_SQUARES> FIRST_COLUMN;
    static const std::array<bool, NUMBER_SQUARES> SECOND_COLUMN;
    static const std::array<bool, NUMBER_SQUARES> SEVENTH_COLUMN;
    static const std::array<bool, NUMBER_SQUARES> EIGHT_COLUMN;
    static int getCoordinateAtPosition(std::string position);
    static std::string getPositionAtCoordinate(int coordinate);
    static bool isValidSquareCoordinate(int coordinate);

  private:
    static const std::array<std::string, BoardUtils::NUMBER_SQUARES>
        ALGEBRAIC_NOTATION;
    static const std::map<std::string, int> POSITION_TO_COORDINATE;
    static std::array<bool, BoardUtils::NUMBER_SQUARES> initRow(int rowNumber);
    static std::array<bool, BoardUtils::NUMBER_SQUARES>
    initColumn(int colNumber);
    static std::map<std::string, int> initializePositionToCoordinate();
};

#endif