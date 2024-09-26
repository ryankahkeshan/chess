#include "player.h"
using namespace std;

Player::Player(Colour c, Colour start) : colour{c}, turn{c == start} {}

void Player::removePiece(shared_ptr<Piece> p) {
    for (auto it = pieces.begin(); it != pieces.end(); it++) {
        if (*it == p) {
            pieces.erase(it);
            break;
        }
    }
}

bool Player::isChecked() const { return inCheck; }

void Player::setChecked() { inCheck = true; }

void Player::removedChecked() { inCheck = false; }

bool Player::isMated() const { return isMate; }

void Player::setMate() { isMate = true; }

bool Player::isTurn() const { return turn; }

void Player::updateTurn() { turn = !turn; }

void Player::addPiece(shared_ptr<Piece> p) { 
    pieces.emplace_back(p); 
    if (p->getType() == PieceType::King) king = p;
}

vector<shared_ptr<Piece>>& Player::getPieces() { return pieces; }

void Player::resign() { resigned = true; }

bool Player::hasResigned() { return resigned; }

Colour Player::getColour() const { return colour; }

shared_ptr<Piece>& Player::getKing() { return king; }

bool Player::attacksSquare(Square& s, const unique_ptr<Board>&b, const unique_ptr<Player>& opp) const {
    for (auto& piece : pieces) {
        if (piece->getType() == PieceType::King) {  // Find if square is adjacent manually
            int row = piece->getLocation().getRow();
            char column = piece->getLocation().getColumn();
            if (abs(s.getRow() - row) <= 1 && 
                static_cast<int>(abs(s.getColumn() - column)) <= 1) {
                return true;
            } 
            continue;
        }
        vector<shared_ptr<Square>> moves = piece->findMoves(b, opp);
        for (auto& move : moves) {
            // If pawn skip over the forward non-attacking moves
            if (piece->getType() == PieceType::Pawn) {
                int row = piece->getLocation().getRow();
                char column = piece->getLocation().getColumn();
                int moveRow = move->getRow();
                if (move->getColumn() == column) {
                    if (getColour() == Colour::White) {
                        if (moveRow == row + 1 || moveRow == row + 2) continue;
                    } else {
                        if (moveRow == row - 1 || moveRow == row - 2) continue;
                    }
                }
            }
            if (move->getRow() == s.getRow() && move->getColumn() == s.getColumn()) {
                return true;
            }
        }
    }
    return false;
}
