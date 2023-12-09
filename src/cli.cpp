#include "cli.h"
#include "board.h"
#include "figure.h"
#include "move.h"
#include "player.h"
#include <iostream>
#include <limits>

Table::Table() {
    setGameMode();
    setPlayers();
}

Board *Table::getBoard() {
    return board.get();
}

const Board *Table::getBoard() const {
    return board.get();
}

void Table::setBoard(std::unique_ptr<Board> board) {
    this->board = std::move(board);
}

WhitePlayer *Table::getWhitePlayer() {
    return whitePlayer.get();
}

const WhitePlayer *Table::getWhitePlayer() const {
    return whitePlayer.get();
}

BlackPlayer *Table::getBlackPlayer() {
    return blackPlayer.get();
}

const BlackPlayer *Table::getBlackPlayer() const {
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
            if (!board->getSquare(figureCoordinate)->isSquareOccupied()) {
                std::cout
                    << "Empty square, please, select square with figure!\n";
                continue;
            } else if (board->getSquare(figureCoordinate)
                           ->getFigureOnSquare()
                           ->getColor() != currentPlayer->getColor()) {
                std::cout << "It's not your figure, please, repeat!\n";
                continue;
            }
            break;
        } catch (const std::out_of_range &ex) {
            std::cout << "Wrong input, please, repeat!\n";
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
                    move->getCoordinateToMove() == destinationCoordinate) {
                    auto pawnPromotion(
                        dynamic_cast<PawnPromotion *>(move.get()));
                    if (pawnPromotion)
                        pawnPromotion->setPromotionFigure(
                            getPromoteFigure(pawnPromotion));
                    return move.get();
                }
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

std::unique_ptr<Figure> Table::getPromoteFigure(Move *move) const {
    std::cout << "Select the figure for pawn promotion:\n(Q)ueen, (Kn)ight, "
                 "(R)ook, (B)ishop:";
    std::string input;
    int coordinate{move->getCoordinateToMove()};
    Color::ColorT color{move->getMovedFigure()->getColor()};
    while (true) {
        std::getline(std::cin, input);
        if (input == "Q")
            return std::make_unique<Queen>(coordinate, color);
        else if (input == "Kn")
            return std::make_unique<Knight>(coordinate, color);
        else if (input == "R")
            return std::make_unique<Rook>(coordinate, color);

        else if (input == "B")
            return std::make_unique<Bishop>(coordinate, color);
        else
            std::cout << "Wrong input, please, repeat!\n";
    }
}
