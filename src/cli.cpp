#include "cli.h"
#include "board.h"
#include "figure.h"
#include "move.h"
#include "player.h"
#include <format>
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
    Player *currentPlayer{whitePlayer.get()};
    board->printBoard();
    while (true) {
        std::cout << std::format("{} turn.\n", currentPlayer->getPlayerName());
        Move *move{getPlayerTurn(currentPlayer)};
        Player *opponentPlayer{getOpponent(currentPlayer)};
        if (currentPlayer->makeMove(move, *board, opponentPlayer) ==
            MoveStatus::LEAVE_PLAYER_IN_CHEK) {
            std::cout << "You are still in check, please, save the King!\n";
            continue;
        }
        if (opponentPlayer->isInCheckMate(*board, currentPlayer)) {
            std::cout << currentPlayer->getPlayerName() << " wins!\n";
            break;
        }
        if (opponentPlayer->isInCheck())
            std::cout << std::format(
                "{} are in check, please, save the King!\n",
                opponentPlayer->getPlayerName());
        currentPlayer = opponentPlayer;
        board->printBoard();
    }
}

void Table::setGameMode() {
    std::cout << "Select game mode:\n1.Player vs Player;\n2.PLayer vs "
                 "Bot;\nInput(number): ";
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
                std::cout << "Wrong input, please repeat!\n";
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
        using enum Color::ColorT;
        case GameMode::PLAYERS:
            whitePlayer = std::make_unique<WhitePlayer>(
                board->getKing(WHITE), board->calculateLegalMoves(WHITE));
            blackPlayer = std::make_unique<BlackPlayer>(
                board->getKing(BLACK), board->calculateLegalMoves(BLACK));
            break;
        case GameMode::BOT:
            // todo init mod with Bot
            break;
    }
}

Move *Table::getPlayerTurn(Player *currentPlayer) {
    std::string input;
    int figureCoordinate{};
    while (true) {
        std::cout << "Select the figure: ";
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
    int destinationCoordinate{};
    while (true) {
        std::cout << "Select destination square (input '0' to select another "
                     "figure): ";
        try {
            std::getline(std::cin, input);
            if (input == "0")
                return getPlayerTurn(currentPlayer);
            destinationCoordinate = BoardUtils::getCoordinateAtPosition(input);
            for (const auto &move : currentPlayer->getLegalMoves()) {
                if (move->getMovedFigure() == board->getSquare(figureCoordinate)
                                                  ->getFigureOnSquare() &&
                    move->getCoordinateToMove() == destinationCoordinate) {
                    if (auto pawnPromotion(
                            dynamic_cast<PawnPromotion *>(move.get()));
                        pawnPromotion)
                        pawnPromotion->setPromotionFigure(
                            getPromoteFigure(pawnPromotion));
                    return move.get();
                }
            }
            std::cout << "Wrong destination square, please, repeat!\n";
        } catch (const std::out_of_range &ex) {
            std::cout << "Wrong input, please, repeat!\n";
        }
    }
}

Player *Table::getOpponent(const Player *player) {
    if (player->getColor() == Color::ColorT::BLACK)
        return whitePlayer.get();
    else
        return blackPlayer.get();
}

std::unique_ptr<Figure> Table::getPromoteFigure(const Move *move) const {
    std::cout << "Select the figure for pawn promotion:\n(Q)ueen, (Kn)ight, "
                 "(R)ook, (B)ishop\nInput:";
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
