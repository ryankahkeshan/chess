#include <string>
#include <iostream>
#include "game.h"
#include "human.h"
#include "computer.h"
#include "levelOne.h"
#include "levelTwo.h"
#include "levelThree.h"
#include "levelFour.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"
#include "restrictedPiece.h"
using namespace std;

Game::Game() { gameBoard = make_unique<Board>(); }

void Game::resetGame() {
    isResigned = false;
    setupUsed = false;
    gameBoard->clear();
    noCaptureMoves = 0;
}

void Game::defaultSetUp() {
    // Add pawns
    for (char i = 'a'; i < static_cast<char>('a' + BOARD_WIDTH); i++) {
        addPiece('P', string(1, i) + "2");
        addPiece('p', string(1, i) + "7");
    }
    // Add rooks
    addPiece('R', "a1"); addPiece('R', "h1");
    addPiece('r', "a8"); addPiece('r', "h8");
    // Add knights
    addPiece('N', "b1"); addPiece('N', "g1");
    addPiece('n', "b8"); addPiece('n', "g8");
    // Add bishop
    addPiece('B', "c1"); addPiece('B', "f1");
    addPiece('b', "c8"); addPiece('b', "f8");
    // Add queens
    addPiece('Q', "d1");
    addPiece('q', "d8");
    // Add kings
    addPiece('K', "e1");
    addPiece('k', "e8");
}

void Game::startGame(string p1, string p2) {
    if (p1 == "human") this->p1 = make_unique<Human>(Colour::White, startTurn);
    else {
        if (p1 == "computer[1]") this->p1 = make_unique<LevelOne>(Colour::White, startTurn);
        else if (p1 == "computer[2]") this->p1 = make_unique<LevelTwo>(Colour::White, startTurn);
        else if (p1 == "computer[3]") this->p1 = make_unique<LevelThree>(Colour::White, startTurn);
        else if (p1 == "computer[4]") this->p1 = make_unique<LevelFour>(Colour::White, startTurn);
    }
    if (p2 == "human") this->p2 = make_unique<Human>(Colour::Black, startTurn);
    else {
        if (p2 == "computer[1]") this->p2 = make_unique<LevelOne>(Colour::Black, startTurn);
        else if (p2 == "computer[2]") this->p2 = make_unique<LevelTwo>(Colour::Black, startTurn);
        else if (p2 == "computer[3]") this->p2 = make_unique<LevelThree>(Colour::Black, startTurn);
        else if (p2 == "computer[4]") this->p2 = make_unique<LevelFour>(Colour::Black, startTurn);
    }
    // Add pieces if needed
    if (!setupUsed) defaultSetUp();
    // Assign pieces
    gameBoard->assignPieces(this->p1, this->p2);
    if (isTiedGame()) {
        whiteScore += 0.5;
        blackScore += 0.5;
    }
}

void Game::useSetup() { setupUsed = true; }

bool Game::isOver() const {
    if (!(p1 && p2)) return false;  // Make sure game has started before checking
    return isCheckMate() || isTiedGame() || isResigned;
}

bool Game::isCheckMate() const { return p1->isMated() || p2->isMated(); }

bool Game::isTiedGame() const {
    if (p1->isChecked() || p2->isChecked()) return false;  // No tie if in check
    // If 50 moves without a capture
    if (drawLimit && noCaptureMoves >= 2 * DRAW_GAME) return true;
    // If only two kings
    if (p1->getPieces().size() == 1 && p2->getPieces().size() == 1) return true;
    // if no moves for one player
    vector<shared_ptr<Piece>>& currPieces = getCurrentPlayer()->getPieces();
    for (auto& piece : currPieces) {
        if (piece->findMoves(gameBoard, getNextPlayer()).size() > 0) {
            return false;
        }
    }
    return true;
}

void Game::setDrawLimit() { drawLimit = true; }

bool Game::isChecked() const {
    if (p1->isChecked() || p2->isChecked()) return true;
    else return false;
}

Colour Game::whoIsChecked() {
    if (p1->isChecked()) return p1->getColour();
    else return p2->getColour();
}

void Game::resign() {
    isResigned = true;
    getCurrentPlayer()->resign();
    if (getWinningPlayer()->getColour() == Colour::White) whiteScore++;
    else blackScore++;
}

float Game::getWhiteScore() const { return whiteScore; }

float Game::getBlackScore() const { return blackScore; }

const unique_ptr<Player>& Game::getCurrentPlayer() const {
    if (p1->isTurn()) return p1;
    else return p2;
}

const unique_ptr<Player>& Game::getNextPlayer() const {
    if (p1->isTurn()) return p2;
    else return p1;
}

const unique_ptr<Player>& Game::getWinningPlayer() const {
    if (p1->isMated() || p1->hasResigned()) return p2;
    else return p1;
}

bool Game::isPawnPromotion(string m1, string m2) {
    Square& s1 = gameBoard->getSquare(m1), s2 = gameBoard->getSquare(m2);
    shared_ptr<Piece> s1Piece = s1.getOccupant(), s2Piece = s2.getOccupant();
    if (s1Piece == nullptr) return false;
    if (s1Piece->getType() == PieceType::Pawn) {
        // For white
        if (s1Piece->getColour() == Colour::White && s1.getRow() == 7 &&
            s2.getRow() == 8) {
                // Make sure the piece is white
                if (s2Piece == nullptr) return true;
                else if (s2Piece->getColour() != Colour::White) return true;
            }
        // For black
        if (s1Piece->getColour() == Colour::Black && s1.getRow() == 2 &&
            s2.getRow() == 1) {
                // Make sure the piece is black
                if (s2Piece == nullptr) return true;
                else if (s2Piece->getColour() != Colour::Black) return true;
            }
    }
    return false;
}

bool Game::makeMove(string m1, string m2, string p = "") {
    // Check that m1 is valid input
    if (m1.length() != 2 || 
        !(m1[0] >= 'a' && m1[0] <= static_cast<char>('a' + BOARD_WIDTH - 1)) ||
        !(m1[1] >= '1' && m1[1] <= static_cast<char>('1' + BOARD_HEIGHT - 1)))
    {
        return false;
    }
    // Check that m2 is valid input
    if (m2.length() != 2 ||
        !(m2[0] >= 'a' && m2[0] <= static_cast<char>('a' + BOARD_WIDTH - 1)) ||
        !(m2[1] >= '1' && m2[1] <= static_cast<char>('1' + BOARD_HEIGHT - 1)))
    {
        return false;
    }
    Square& s1 = gameBoard->getSquare(m1);
    Square& s2 = gameBoard->getSquare(m2);
    auto s1Piece = s1.getOccupant();
    // Make sure there is a piece on s1 and that move is legal
    if ((!s1.isOccupied()) || (!s1Piece->canMove(s1, s2, getCurrentPlayer(), getNextPlayer(), gameBoard))) {
        return false;
    }
    if (p != "") {
        // If promoted piece is of the good colour
        if (p.length() == 1 &&
            ((s1Piece->getColour() == Colour::White && (p[0] >= 'A' && p[0] <= 'Z'))||
            (s1Piece->getColour() == Colour::Black && (p[0] >= 'a' && p[0] <= 'z'))) &&
            (p[0] == 'b' || p[0] == 'B' || p[0] == 'r' || p[0] == 'R' ||
             p[0] == 'n' || p[0] == 'N' || p[0] == 'q' || p[0] == 'Q'))
        {
            // If s2 has a piece
            vector<shared_ptr<Piece>>& oppPieces = getNextPlayer()->getPieces();
            for (auto& piece : oppPieces) {
                if (piece == s2.getOccupant()) getNextPlayer()->removePiece(piece);
            }
            // Promote pawn
            addPiece(p[0], m2);
            gameBoard->assignPieces(p1, p2);
            // Remove pawn
            s1.removeOccupant(true);
        } else return false;
    } else if (s2.getOccupant() != nullptr) { // A move resulted in a capture
        s1Piece->move(s1, s2, getCurrentPlayer(), getNextPlayer(), gameBoard);
        noCaptureMoves = 0;
    } else {   // A move without a capture
        s1Piece->move(s1, s2, getCurrentPlayer(), getNextPlayer(), gameBoard);
        noCaptureMoves++;
    }
    // Remove from enPassantPawns
    for (auto it = enPassantPawns.begin(); it != enPassantPawns.end(); it++) {
        dynamic_pointer_cast<Pawn>(*it)->falsifyEnPassant();
    }

    // Make all pawns that moved un-en-passant-able
    vector<shared_ptr<Piece>>& p1Pieces = p1->getPieces();
    for (auto it = p1Pieces.begin(); it != p1Pieces.end(); it++) {
        if ((*it)->getType() == PieceType::Pawn &&
            dynamic_pointer_cast<Pawn>(*it)->canEnPassant()) {
            enPassantPawns.emplace_back(dynamic_pointer_cast<Pawn>(*it));
        }
    }
    vector<shared_ptr<Piece>>& p2Pieces = p2->getPieces();
    for (auto it = p2Pieces.begin(); it != p2Pieces.end(); it++) {
        if ((*it)->getType() == PieceType::Pawn &&
            dynamic_pointer_cast<Pawn>(*it)->canEnPassant()) {
            enPassantPawns.emplace_back(dynamic_pointer_cast<Pawn>(*it));
        }
    }

    // Check for checks
    // Get opponents king's square (current because turns have updated)
    shared_ptr<Piece>& oppKing = getNextPlayer()->getKing();
    int row = oppKing->getLocation().getRow();
    char column = oppKing->getLocation().getColumn();
    // See if any move from all of currrent's pieces is attacking the 
    //   opponent's king's square
    vector<shared_ptr<Piece>>& currPieces = getCurrentPlayer()->getPieces();
    for (auto& piece : currPieces) {
        vector<shared_ptr<Square>> moves = piece->findMoves(gameBoard, getNextPlayer());
        for (auto move : moves) {
            if (move->getRow() == row && move->getColumn() == column) {
                getNextPlayer()->setChecked();
                break;
            }
        }
        if (isChecked()) break;
    }
    // Check for checkmate
    bool isDone = true;
    bool checked = isChecked(); // Cancel the removal of checks in canMove
    vector<shared_ptr<Piece>>& oppPieces = getNextPlayer()->getPieces();
    for (auto& piece : oppPieces) { // Check if there is at least one legal move
        int row = piece->getLocation().getRow();
        char column = piece->getLocation().getColumn();
        vector<shared_ptr<Square>> moves = piece->findMoves(gameBoard, getCurrentPlayer());
        for (auto& move : moves) {
            if (piece->canMove(gameBoard->getSquare(row, column),
                gameBoard->getSquare(move->getRow(), move->getColumn()),
                getNextPlayer(), getCurrentPlayer(), gameBoard)) {
                    isDone = false;
                    break;
            }
        }
        if (!isDone) break;  // No need to keep on looping, the game is not done
    }
    if (checked) getNextPlayer()->setChecked();
    if (isDone) {  // No legal moves
        if (getNextPlayer()->isChecked()) { // Checkmate
            getNextPlayer()->setMate();
            if (getCurrentPlayer()->getColour() == Colour::White) {
                whiteScore++;
            } else blackScore++;
        } 
    }

    // Update turns
    const unique_ptr<Player>& curr = getCurrentPlayer();
    const unique_ptr<Player>& next = getNextPlayer();
    curr->updateTurn();
    next->updateTurn();

    // Update scores if tied
    if (isTiedGame()) { whiteScore += 0.5; blackScore += 0.5; }

    return true;
}

void Game::cpuMove() {
    if (getCurrentPlayer()->getType() != PlayerType::Computer) return;
    vector<string> args = getCurrentPlayer()->cpuMove(gameBoard, getNextPlayer());
    makeMove(args.at(0), args.at(1));
}

void Game::addPiece(char piece, string square) {
    Colour c;
    shared_ptr<Piece> newPiece;
    // Determine colour of piece
    if (piece >= 'A' && piece <= 'Z') c = Colour::White; 
    else c = Colour::Black;
    unique_ptr<Player>& owner = (piece >= 'A' && piece <= 'Z') ? p1 : p2;
    // Determine piece type
    if (piece == 'p' || piece == 'P') newPiece = make_shared<Pawn>(c, owner);
    else if (piece == 'r' || piece == 'R') newPiece = make_shared<Rook>(c, owner);
    else if (piece == 'b' || piece == 'B') newPiece = make_shared<Bishop>(c, owner);
    else if (piece == 'n' || piece == 'N') newPiece = make_shared<Knight>(c, owner);
    else if (piece == 'q' || piece == 'Q') newPiece = make_shared<Queen>(c, owner);
    else if (piece == 'k' || piece == 'K') newPiece = make_shared<King>(c, owner);
    // Add piece to square
    Square& s = gameBoard->getSquare(square);
    if (s.isOccupied()) s.removeOccupant(true);
    s.setOccupant(newPiece, true);
    // Add square to piece
    newPiece->setLocation(gameBoard->getSquarePointer(square));
}

void Game::removePiece(string square) {
    Square& s = gameBoard->getSquare(square);
    // Remove square from piece
    s.getOccupant()->removeLocation();
    // Remove pice from square
    s.removeOccupant(true);
}

bool Game::isValidSetup() { return gameBoard->isValidBoard(); }

void Game::setTurn(string colour) {
    if (colour == "white") startTurn = Colour::White;
    else if (colour == "black") startTurn = Colour::Black;
}

ostream &operator<<(ostream &out, const Game &g) {
    return out << *(g.gameBoard);
}
