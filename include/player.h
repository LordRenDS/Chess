#ifndef PLAYER_H
#define PLAYER_H
#include "color.h"
#include "move_status.h"
#include <memory>
#include <vector>

// todo
class Move;
class Board;

class Player {
  private:
    std::vector<std::unique_ptr<Move>> calculateLegalMoves(Board &board) const;
    std::vector<Move *>
    calculateAttackOnSquare(int coordinate,
                            const std::vector<std::unique_ptr<Move>> &moves);
    bool
    hasAttackOnKing(Board &board,
                    const std::vector<std::unique_ptr<Move>> &opponentMoves);
    bool hasEscapeMoves(Board &board, Player *opponent);

  protected:
    std::vector<std::unique_ptr<Move>> legalMoves;
    bool inCheck{};

  public:
    Player(Board &board);
    virtual ~Player() = default;
    //
    std::vector<std::unique_ptr<Move>> &getLegalMoves();
    virtual Color::ColorT getColor() const = 0;
    //
    MoveStatus makeMove(Move *move, Board &board, Player *opponent);
    bool isInCheckMate(Board &board, Player *opponent);
    std::string getPlayerName();
};

class WhitePlayer : public Player {
  public:
    using Player::Player;
    Color::ColorT getColor() const override;
};

class BlackPlayer : public Player {
  public:
    using Player::Player;
    Color::ColorT getColor() const override;
};

class Bot : public Player {};

// int main(){
// 	WhitePlayer p();
// }
#endif