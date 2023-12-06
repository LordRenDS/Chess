#ifndef CLI_H
#define CLI_H
#include <memory>

class Board;
class Player;
class WhitePlayer;
class BlackPlayer;
class Move;
enum class GameMode {
    PLAYERS = 1,
    BOT = 2
};

class Table {
  private:
    std::unique_ptr<Board> board;
    std::unique_ptr<WhitePlayer> whitePlayer;
    std::unique_ptr<BlackPlayer> blackPlayer;
    GameMode gameMode{};
    void setGameMode();
    void setPlayers();
    Move *getPlayerTurn(Player *currentPlayer);
    Player *getOpponent(const Player *player);

  public:
    Table();
    ~Table() = default;
    //
    Board *getBoard();
    void setBoard(std::unique_ptr<Board> board);
    WhitePlayer *getWhitePlayer();
    BlackPlayer *getBlackPlayer();
    //
    void startGame();
};

#endif