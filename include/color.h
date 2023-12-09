#ifndef COLOR_H
#define COLOR_H

#include "board_utils.h"
#include <string>

class Color {
  public:
    enum class ColorT {
        BLACK,
        WHITE
    };
    static std::string getColorName(ColorT color) {
        switch (color) {
            case ColorT::BLACK:
                return "b";
            case ColorT::WHITE:
                return "w";
        }
    };
    static int getDirection(ColorT color) {
        switch (color) {
            case ColorT::BLACK:
                return 1;
            case ColorT::WHITE:
                return -1;
        }
    };
    static int getOppositeDirection(ColorT color) {
        switch (color) {
            case ColorT::BLACK:
                return -1;
            case ColorT::WHITE:
                return 1;
        }
    }
    static bool isPawnPromotionSquare(ColorT color, int position) {
        switch (color) {
            case ColorT::BLACK:
                return BoardUtils::FIFTH_RANK[position];
            case ColorT::WHITE:
                return BoardUtils::EIGHTH_RANK[position];
        }
    }
};

#endif