#include "board.h"

int main() {
    // std::unique_ptr<Table> table(std::make_unique<Table>());
    std::unique_ptr<Board> board(std::make_unique<Board>());
    board->printBoard();
    return 0;
}
