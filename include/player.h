#ifndef PLAYER_H
#define PLAYER_H
#include "color.h"
#include "move_status.h"
#include <memory>
#include <vector>

class Move;
class Board;
class King;
class Table;

class Player {
  private:
    virtual std::vector<std::unique_ptr<Move>> calculateCastleMoves(
        Board &board,
        const std::vector<std::unique_ptr<Move>> &opponentMoves) = 0;

  protected:
    King *playerKing{};
    std::vector<std::unique_ptr<Move>> legalMoves;
    bool inCheck{};
    //
    // potential redundant square attack check in queen side castle move
    std::vector<Move *>
    calculateAttackOnSquare(int coordinate,
                            const std::vector<std::unique_ptr<Move>> &moves);
    bool
    hasAttackOnKing(const std::vector<std::unique_ptr<Move>> &opponentMoves);
    bool
    hasAttackOnKing(const King *king,
                    const std::vector<std::unique_ptr<Move>> &opponentMoves);
    bool hasEscapeMoves(Board &board, Player *opponent);

  public:
    explicit Player(King *king, std::vector<std::unique_ptr<Move>> &&legalMoves,
                    bool inCheck = false);
    Player(const Player &player);
    virtual ~Player() = default;
    //
    std::vector<std::unique_ptr<Move>> &getLegalMoves();
    virtual Color::ColorT getColor() const = 0;
    //
    MoveStatus makeMove(Move *move, Board &board, Player *opponent);
    std::vector<std::unique_ptr<Move>> calculateAllLegalMoves(
        Board &board, const std::vector<std::unique_ptr<Move>> &opponentMoves);
    bool isInCheck() const;
    bool isInCheckMate(Board &board, Player *opponent);
    virtual std::string getPlayerName() const = 0;
    //
    friend Table;
};

class WhitePlayer : public Player {
  private:
    std::vector<std::unique_ptr<Move>> calculateCastleMoves(
        Board &board,
        const std::vector<std::unique_ptr<Move>> &opponentMoves) override;

  public:
    using Player::Player;
    ~WhitePlayer() override = default;
    Color::ColorT getColor() const override;
    std::string getPlayerName() const override;
};

class BlackPlayer : public Player {
  private:
    std::vector<std::unique_ptr<Move>> calculateCastleMoves(
        Board &board,
        const std::vector<std::unique_ptr<Move>> &opponentMoves) override;

  public:
    using Player::Player;
    ~BlackPlayer() override = default;
    Color::ColorT getColor() const override;
    std::string getPlayerName() const override;
};

// class Bot : public Player {
//   private:
//     std::unique_ptr<Player> player;
//     //
//     std::vector<std::unique_ptr<Move>> calculateCastleMoves(
//         Board &board,
//         const std::vector<std::unique_ptr<Move>> &opponentMoves) override;
//
//   public:
//     using Player::Player;
//     ~Bot() override = default;
//     //
//     Color::ColorT getColor() const override;
//     std::string getPlayerName() const override;
// };

#endif