#include "board.h"
#include "cli.h"
#include "figure.h"
#include "move.h"
#include "player.h"
#include <iostream>

int main() {
    auto table(std::make_unique<Table>());
    table->startGame();
    std::cin.get();
    return 0;
}