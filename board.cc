#include "board.h"
using namespace std;

Board::Board() {
    td = make_shared<TextDisplay>();
    gd = make_shared<GraphicalDisplay>();
    for (int i = 1; i <= BOARD_HEIGHT; i++) {
        vector<shared_ptr<Square>> vec;
        for (char j = 'a'; j <= static_cast<char>('a' + BOARD_WIDTH - 1); j++) {
            shared_ptr<Square> s = make_shared<Square>();
            s->setCoords(i, j);
            if ((i+static_cast<int>(j - 'a')) % 2 == 0) s->setColour(Colour::White);
            else s->setColour(Colour::Black);
            s->attach(td);
            s->attach(gd);
            vec.emplace_back(move(s));
        }
        board.emplace_back(move(vec));
    }
}

void Board::assignPieces(unique_ptr<Player>& p1, unique_ptr<Player>& p2) {
    for (const auto& row : board) {
        for (const auto& square : row) {
            const auto& piece = square->getOccupant();
            if (piece == nullptr) continue;
            else if (piece->getColour() == Colour::White) p1->addPiece(piece);
            else p2->addPiece(piece);
        }
    }
}

void Board::clear() {
    for (auto& row : board) {
        for (auto& square : row) {
            if (square->getOccupant()) {
                unique_ptr<Player>& owner = square->getOccupant()->getOwner();
                owner->removePiece(square->getOccupant());
                square->removeOccupant(true);
            }
        }
    }
}

bool Board::isValidBoard() {
    int numWhiteKings = 0;
    int numBlackKings = 0;
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (auto& square : board.at(i)) {
            if (square->getOccupant() == nullptr) continue;
            PieceType pt = square->getOccupant()->getType();
            // No pawns on first & last rows
            if ((i == 0 || i == BOARD_HEIGHT - 1) && pt == PieceType::Pawn) {
                return false;
            }
            // Count kings
            if (pt == PieceType::King) {
                if (square->getOccupant()->getColour() == Colour::White) numWhiteKings++;
                else numBlackKings++;
            }
        }
    }
    if (numWhiteKings == 1 && numBlackKings == 1) return true;
    else return false;
}

bool Board::isPathEmpty(Square& s1, Square& s2) const {
    // Destructure
    int s1Row = s1.getRow(), s2Row = s2.getRow();
    char s1Col = s1.getColumn(), s2Col = s2.getColumn();
    // If same square
    if (s1Row == s2Row && s1Col == s2Col) return false;
    // Get small coords
    int smallRow = s1Row < s2Row ? s1Row : s2Row;
    int bigRow = smallRow == s1Row ? s2Row : s1Row;
    char smallCol = s1Col < s2Col ? s1Col : s2Col;
    char bigCol = smallCol == s1Col ? s2Col : s1Col;
    // If vertical line
    if (s1Col == s2Col) {
        for (int i = smallRow + 1; i < bigRow; i++) {
            if (getSquare(i, s1Col).getOccupant() != nullptr) return false;
        }
    }
    // If horizontal lne
    if (s1Row == s2Row) {
        for (char c = smallCol + 1; c < bigCol; c++) {
            if (getSquare(s1Row, c).getOccupant() != nullptr) return false;
        }
    }
    // If diagonal
    if (abs(s1Col - s2Col) == abs(s1Row - s2Row)) {
        if (s1Col > s2Col && s1Row < s2Row) {   // Top-left diagonal
            char c = s1Col - 1;
            for (int i = s1Row + 1; i < s2Row; i++) {
                if (getSquare(i, c).getOccupant() != nullptr) return false;
                c--;
            }
        } else if (s1Col > s2Col && s1Row > s2Row) {   // Bottom-left diagonal
            char c = s1Col - 1;
            for (int i = s1Row - 1; i > s2Row; i--) {
                if (getSquare(i, c).getOccupant() != nullptr) return false;
            }
        } else if (s1Row < s2Row) {   // Top-right diagonal
            char c = s1Col + 1;
            for (int i = s1Row + 1; i < s2Row; i++) {
                if (getSquare(i, c).getOccupant() != nullptr) return false;
            }
        } else {   // Bottom-right diagonal
            char c = s1Col + 1;
            for (int i = s1Row - 1; i > s2Row; i--) {
                if (getSquare(i, c).getOccupant() != nullptr) return false;
            }
        }
    }
    return true;
}

Square& Board::getSquare(string s) {
    // Convert coordinantes to indexes 
    int letter = static_cast<int>(s[0] - 'a');
    int number = static_cast<int>(s[1] - '1');
    // Find square
    return *(board.at(number).at(letter));
}

shared_ptr<Square> Board::getSquarePointer(string s) {
    // Convert coordinantes to indexes 
    int letter = static_cast<int>(s[0] - 'a');
    int number = static_cast<int>(s[1] - '1');
    // Find square
    return board.at(number).at(letter);
}

Square& Board::getSquare(int r, char c) const {
    return *(board.at(r - 1).at(static_cast<int>(c - 'a')));
}

shared_ptr<Square> Board::getSquarePointer(int r, char c) {
    return board.at(r - 1).at(static_cast<int>(c - 'a'));
}

Square& Board::getAboveSquare(Square& s) {
    int row = s.getRow();
    char col = s.getColumn();
    if (row == 8) throw out_of_range("No square above");
    else return getSquare(row + 1, col);
}

Square& Board::getBelowSquare(Square& s) {
    int row = s.getRow();
    char col = s.getColumn();
    if (row == 1) throw out_of_range("No square below");
    else return getSquare(row - 1, col);
}

Square& Board::getLeftSquare(Square& s) {
    int row = s.getRow();
    char col = s.getColumn();
    if (col == 'a') throw out_of_range("No square to the left");
    else return getSquare(row, static_cast<char>(col - 1));
}

Square& Board::getRightSquare(Square& s) {
    int row = s.getRow();
    char col = s.getColumn();
    if (col == 'h') throw out_of_range("No square to the right");
    else return getSquare(row, static_cast<char>(col + 1));
}

Square& Board::getTopLeftSquare(Square& s) {
    int row = s.getRow();
    char col = s.getColumn();
    if (row == 8 || col == 'a') throw out_of_range("No square to the top-left");
    else return getSquare(row + 1, static_cast<char>(col - 1));
}

Square& Board::getTopRightSquare(Square& s) {
    int row = s.getRow();
    char col = s.getColumn();
    if (row == 8 || col == 'h') throw out_of_range("No square to the top-right");
    else return getSquare(row + 1, static_cast<char>(col + 1));
}

Square& Board::getBottomLeftSquare(Square& s) {
    int row = s.getRow();
    char col = s.getColumn();
    if (row == 1 || col == 'a') throw out_of_range("No square to the bottom-left");
    else return getSquare(row - 1, static_cast<char>(col - 1));
}

Square& Board::getBottomRightSquare(Square& s) {
    int row = s.getRow();
    char col = s.getColumn();
    if (row == 1 || col == 'h') throw out_of_range("No square to the bottom-right");
    else return getSquare(row - 1, static_cast<char>(col + 1));
}

vector<shared_ptr<Square>> Board::verticalMoves(Square& start, int limit, bool upOnly, bool downOnly) {
    vector<shared_ptr<Square>> moves;
    if (start.getOccupant() == nullptr) return moves;
    // Upwards direction
    if (!downOnly) {
        int upCount = 0;
        shared_ptr<Square> up = make_shared<Square>(start);
        while (up && upCount < limit) {
            try {
                up = make_shared<Square>(getAboveSquare(*up));
                if (up->getOccupant() == nullptr) moves.emplace_back(up);
                else if (upOnly) break;
                else { moves.emplace_back(up); break;}

                upCount++;
            } catch (const out_of_range& e) { up = nullptr; break; }
        }
    }
    // Downwards direction
    if (!upOnly) {
        int downCount = 0;
        shared_ptr<Square> down = make_shared<Square>(start);
        while (down && downCount < limit) {
            try {
                down = make_shared<Square>(getBelowSquare(*down));
                if (down->getOccupant() == nullptr) moves.emplace_back(down);
                else if (downOnly) break;
                else {moves.emplace_back(down); break; }
                downCount++;
            } catch (const out_of_range& e) { down = nullptr; break; }
        }
    }
    return moves;
}

vector<shared_ptr<Square>> Board::horizontalMoves(Square& start, int limit) {
    vector<shared_ptr<Square>> moves;
    if (start.getOccupant() == nullptr) return moves;
    // Right direction
    int rightCount = 0;
    shared_ptr<Square> right = make_shared<Square>(start);
    while (right && rightCount < limit) {
        try {
            right = make_shared<Square>(getRightSquare(*right));
            if (right->getOccupant() == nullptr) moves.emplace_back(right);
            else { moves.emplace_back(right); break; }
            rightCount++;
        } catch (const out_of_range& e) { right = nullptr; break; }
    }
    // Left direction
    int leftCount = 0;
    shared_ptr<Square> left = make_shared<Square>(start);
    while (left && leftCount < limit) {
        try {
            left = make_shared<Square>(getLeftSquare(*left));
            if (left->getOccupant() == nullptr) moves.emplace_back(left);
            else { moves.emplace_back(left); break; }
            leftCount++;
        } catch (const out_of_range& e) { left = nullptr; break; }
    }
    return moves;
}

vector<shared_ptr<Square>> Board::diagonalMoves(Square& start, int limit) {
    vector<shared_ptr<Square>> moves;
    if (start.getOccupant() == nullptr) return moves;
    // Top-left direction
    int topLeftCount = 0;
    shared_ptr<Square> topLeft = make_shared<Square>(start);
    while (topLeft && topLeftCount < limit) {
        try {
            topLeft = make_shared<Square>(getTopLeftSquare(*topLeft));
            if (topLeft->getOccupant() == nullptr) moves.emplace_back(topLeft);
            else { moves.emplace_back(topLeft); break; }
            topLeftCount++;
        } catch (const out_of_range& e) { topLeft = nullptr; break; }
    }
    // Top-right direction
    int topRightCount = 0;
    shared_ptr<Square> topRight = make_shared<Square>(start);
    while (topRight && topRightCount < limit) {
        try {
            topRight = make_shared<Square>(getTopRightSquare(*topRight));
            if (topRight->getOccupant() == nullptr) moves.emplace_back(topRight);
            else { moves.emplace_back(topRight); break; }
            topRightCount++;
        } catch (const out_of_range& e) { topRight = nullptr; break; }
    }
    // Bottom-left direction
    int bottomLeftCount = 0;
    shared_ptr<Square> bottomLeft = make_shared<Square>(start);
    while (bottomLeft && bottomLeftCount < limit) {
        try {
            bottomLeft = make_shared<Square>(getBottomLeftSquare(*bottomLeft));
            if (bottomLeft->getOccupant() == nullptr) moves.emplace_back(bottomLeft);
            else { moves.emplace_back(bottomLeft); break; }
            bottomLeftCount++;
        } catch (const out_of_range& e) { bottomLeft = nullptr; break; }
    }
    // Bottom-rigth direction
    int bottomRightCount = 0;
    shared_ptr<Square> bottomRight = make_shared<Square>(start);
    while (bottomRight && bottomRightCount < limit) {
        try {
            bottomRight = make_shared<Square>(getBottomRightSquare(*bottomRight));
            if (bottomRight->getOccupant() == nullptr) moves.emplace_back(bottomRight);
            else { moves.emplace_back(bottomRight); break; }
            bottomRightCount++;
        } catch (const out_of_range& e) { bottomRight = nullptr; break; }
    }
    return moves;
}

vector<shared_ptr<Square>> Board::knightMoves(Square& start) {
    vector<shared_ptr<Square>> moves;
    if (start.getOccupant() == nullptr) return moves;
    int row = start.getRow();
    char col = start.getColumn();
    // Top-left
    if (row <= 6 && col >= 'b') {
        Square& s = getSquare(row + 2, static_cast<char>(col - 1));
            moves.emplace_back(make_shared<Square>(s));
    }
    // Top-right
    if (row <= 6 && col <= 'g') {
        Square& s = getSquare(row + 2, static_cast<char>(col + 1));
            moves.emplace_back(make_shared<Square>(s));
    }
    // Upper-left
    if (row <= 7 && col >= 'c') {
        Square& s = getSquare(row + 1, static_cast<char>(col - 2));
            moves.emplace_back(make_shared<Square>(s));
    }
    // Upper-right
    if (row <= 7 && col <= 'f') {
        Square& s = getSquare(row + 1, static_cast<char>(col + 2));
            moves.emplace_back(make_shared<Square>(s));
    }
    // Lower-left
    if (row >= 2 && col >= 'c') {
        Square& s = getSquare(row - 1, static_cast<char>(col - 2));
            moves.emplace_back(make_shared<Square>(s));
    }
    // Lower-rigth
    if (row >= 2 && col <= 'f') {
        Square& s = getSquare(row - 1, static_cast<char>(col + 2));
            moves.emplace_back(make_shared<Square>(s));
    }
    // Bottom-left
    if (row >= 3 && col >= 'b') {
        Square& s = getSquare(row - 2, static_cast<char>(col - 1));
            moves.emplace_back(make_shared<Square>(s));
    }
    // Bottom-right
    if (row >= 3 && col <= 'g') {
        Square& s = getSquare(row - 2, static_cast<char>(col + 1));
            moves.emplace_back(make_shared<Square>(s));
    }
    return moves;
}

ostream &operator<<(ostream &out, const Board &b) {
    return out << *(b.td) << endl;
}
