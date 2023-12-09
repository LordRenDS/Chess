#ifndef CLI_H
#define CLI_H
#include "figure_type.h"
#include <memory>

class Board;
class Player;
class WhitePlayer;
class BlackPlayer;
class Move;
class Figure;

enum class GameMode {
    PLAYERS = 1,
    BOT = 2
};

class Table {
  private:
    std::unique_ptr<Board> board{std::make_unique<Board>()};
    std::unique_ptr<WhitePlayer> whitePlayer;
    std::unique_ptr<BlackPlayer> blackPlayer;
    GameMode gameMode{};
    void setGameMode();
    void setPlayers();
    Move *getPlayerTurn(Player *currentPlayer);
    std::unique_ptr<Figure> getPromoteFigure(Move *move) const;
    Player *getOpponent(const Player *player);

  public:
    Table();
    ~Table() = default;
    //
    Board *getBoard();
    const Board *getBoard() const;
    void setBoard(std::unique_ptr<Board> board);
    WhitePlayer *getWhitePlayer();
    const WhitePlayer *getWhitePlayer() const;
    BlackPlayer *getBlackPlayer();
    const BlackPlayer *getBlackPlayer() const;
    //
    void startGame();
};

#endif