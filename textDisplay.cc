#include <memory>
#include "textDisplay.h"
using namespace std;

TextDisplay::TextDisplay() {
    // Print board from line 8 to line 1
    for (int i = BOARD_HEIGHT; i > 0; i--) {
        vector<char> line {static_cast<char>('0' + i), ' '};
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if ((i + j) % 2 == 0) line.emplace_back(' ');
            else line.emplace_back('_');
        }
        display.emplace_back(line);
    }
    // Print underboard line
    vector<char> empty;
    display.emplace_back(empty);
    vector<char> alphabet {' ', ' '};
    for (int i = 0; i < BOARD_WIDTH; i++) {
        alphabet.emplace_back(static_cast<char>('a' + i));
    }
    display.emplace_back(alphabet);
}

void TextDisplay::notify(Square &s) {
    // Square's coords
    int row = s.getRow();
    int column = static_cast<int>(s.getColumn() - 'a');
    // Get piece
    shared_ptr<Piece> occupant = s.getOccupant();
    // If empty return to default
    if (occupant == nullptr) {
        char tile = (row + column) % 2 == 0 ? ' ' : '_';
        this->display.at(BOARD_HEIGHT - row).at(2 + column) = tile;
        return;
    }
    // Get character to display
    char piece = occupant->getStringType()[0];
    // Add piece to display
    this->display.at(BOARD_HEIGHT - row).at(2 + column) = piece;
}

TextDisplay::~TextDisplay() { this->display.clear(); }

ostream &operator<<(ostream &out, const TextDisplay &td) {
    for (auto& line : td.display) {
        for (auto& ch : line) { out << ch; }
        out << endl;
    }
    return out;
}
