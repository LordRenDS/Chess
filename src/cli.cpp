#include "cli.h"
#include "board.h"
#include "figure.h"
#include "move.h"
#include "player.h"
#include <iostream>
#include <limits>

Table::Table() : board(std::make_unique<Board>()) {
    setGameMode();
    setPlayers();
}

Board *Table::getBoard() {
    return board.get();
}

void Table::setBoard(std::unique_ptr<Board> board) {
    this->board = std::move(board);
}

WhitePlayer *Table::getWhitePlayer() {
    return whitePlayer.get();
}

BlackPlayer *Table::getBlackPlayer() {
    return blackPlayer.get();
}

void Table::startGame() {
    setGameMode();
    Player *currentPlayer{whitePlayer.get()};
    while (true) {
        Move *move{getPlayerTurn(currentPlayer)};
        Player *opponentPlayer{getOpponent(currentPlayer)};
        if (currentPlayer->makeMove(move, *board, opponentPlayer) ==
            MoveStatus::LEAVE_PLAYER_IN_CHEK) {
            std::cout << "You are in check, please, save the King\n";
            continue;
        };
        if (opponentPlayer->isInCheckMate(*board, opponentPlayer)) {
            std::cout << currentPlayer->getPlayerName() << " wins!\n";
            break;
        }
        currentPlayer = opponentPlayer;
    }
}

void Table::setGameMode() {
    std::cout << "Game mode:\n1.Player vs Player;\n2.PLayer vs Bot;\nSelect "
                 "game mode(number): ";
    while (true) {
        int input{};
        std::cin >> input;
        switch (input) {
            case 1:
                gameMode = GameMode::PLAYERS;
                break;
            case 2:
                gameMode = GameMode::BOT;
                break;
            default:
                std::cout << "Wrong input, please repeat\n";
                break;
        }
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else if (static_cast<int>(gameMode)) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            setPlayers();
            break;
        }
    }
}

void Table::setPlayers() {
    switch (gameMode) {
        case GameMode::PLAYERS:
            whitePlayer = std::make_unique<WhitePlayer>(*this->board);
            blackPlayer = std::make_unique<BlackPlayer>(*this->board);
            break;
        case GameMode::BOT:
            // todo init mod with Bot
            break;
    }
}

Move *Table::getPlayerTurn(Player *currentPlayer) {
    std::cout << "Select the figure: ";
    std::string input;
    int figureCoordinate{};
    while (true) {
        try {
            std::getline(std::cin, input);
            figureCoordinate = BoardUtils::getCoordinateAtPosition(input);
            if (board->getSquare(figureCoordinate)->isSquareOccupie() &&
                board->getSquare(figureCoordinate)
                        ->getFigureOnSquare()
                        ->getColor() != currentPlayer->getColor()) {
                std::cout << "It's not your figure, please, repeat: ";
                continue;
            }
            break;
        } catch (const std::out_of_range &ex) {
            std::cout << "Wrong input, please, repeat: ";
        }
    }
    std::cout << "Select destination square: ";
    int destinationCoordinate{};
    while (true) {
        try {
            std::getline(std::cin, input);
            destinationCoordinate = BoardUtils::getCoordinateAtPosition(input);
            for (const auto &move : currentPlayer->getLegalMoves()) {
                if (move->getMovedFigure() == board->getSquare(figureCoordinate)
                                                  ->getFigureOnSquare() &&
                    move->getCoordinateToMove() == destinationCoordinate)
                    return move.get();
            }
            std::cout << "Wrong destination square, please, repeat: ";
        } catch (const std::out_of_range &ex) {
            std::cout << "Wrong input, please, repeat: ";
        }
    }
}

Player *Table::getOpponent(const Player *player) {
    if (player->getColor() == Color::ColorT::BLACK)
        return whitePlayer.get();
    else
        return blackPlayer.get();
}
