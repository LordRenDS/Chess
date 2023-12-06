#ifndef COLOR_H
#define COLOR_H
#include <string>

// enum class Color {
//     BLACK,
//     WHITE
// };

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
};

#endif