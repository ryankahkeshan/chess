#include <string>
#include "graphicalDisplay.h"
#include "constants.h"
using namespace std;

GraphicalDisplay::GraphicalDisplay() {
    window = make_unique<Xwindow>();
    for (int i = BOARD_HEIGHT; i >= 0; i--) {
        int y = (BOARD_HEIGHT - i) * SQUARE_SIZE + 40;
        if (i != 0) {
            // Add numbers on the left side
            window->drawString(15, y + 40, to_string(i));
            // Left-side border
            window->fillRectangle(36, y, 4, SQUARE_SIZE);
            // Right-side border
            window->fillRectangle(BOARD_WIDTH * SQUARE_SIZE + 40, y, 4, SQUARE_SIZE);
        }

        for (int j = 0; j < BOARD_WIDTH; j++) {
            int x = j * SQUARE_SIZE + 40;
            if (i == BOARD_HEIGHT) {
                // Top border
                window->fillRectangle(x, 36, SQUARE_SIZE, 4);
            } else if (i == 1) {
                window->fillRectangle(x, BOARD_HEIGHT * SQUARE_SIZE + 40, SQUARE_SIZE, 4);
            }

            if (i == 0) {  // Add letters to bottom row
                string s{static_cast<char>(j + 'a')};
                 window->drawString(x + 40, y + 25, s);
            } else if ((i+j)%2 == 0) {  // White square
                window->fillRectangle(x, y, SQUARE_SIZE, SQUARE_SIZE, 0);
            } else {  // Blue square
                window->fillRectangle(x, y, SQUARE_SIZE, SQUARE_SIZE, 4);
            }
        }
    }
}

void GraphicalDisplay::notify(Square &s) {
    // Start of square
    int x = static_cast<int>(s.getColumn() - 'a') * SQUARE_SIZE + 40;
    int y = GRAPHICAL_DISPLAY_HEIGHT - s.getRow() * SQUARE_SIZE;
    // If piece left
    if (s.getOccupant() == nullptr) {
        // Draw over
        int colour = s.getColour() == Colour::White ? 0 : 4;
        window->fillRectangle(x, y - 40, SQUARE_SIZE, SQUARE_SIZE, colour);
        return;
    }
    // Get starting coordinates of the cell's rectangle
    string piece = s.getOccupant()->getStringType();
    window->drawString(x + 38, y + 8, piece);
}

GraphicalDisplay::~GraphicalDisplay() {}
