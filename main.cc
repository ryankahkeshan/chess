#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "game.h"
using namespace std;

const string CPU1 = "computer[1]";
const string CPU2 = "computer[2]";
const string CPU3 = "computer[3]";
const string CPU4 = "computer[4]";
const vector<string> whitePieces{"P", "R", "N", "B", "Q", "K"};
const vector<string> blackPieces{"p", "r", "n", "b", "q", "k"};

// isAPiece(s, pieces, pieces2) returns true if s in in pieces or in pieces 2 
//  and false otherwise. pieces 2 is optional
bool isAPiece(const string& s, const vector<string>& pieces, 
              const vector<string>& pieces2 = {}) 
{
    for (const string& piece : pieces) { 
        if (piece == s) return true;
    }
    for (const string& piece : pieces2) { 
        if (piece == s) return true;
    }
    return false;
}

string toLowerCase(string s) {
    string copy = s;
    for (char& c : copy) {
        if (c >= 'A' && c <= 'Z') c = c + ('a' - 'A');
    }
    return copy;
}

bool isSquareValid(string s) {
    if ((s.length() != 2) || !(s[0] >= 'a' && s[0] <= 'z') || 
        !(s[1] >= '1' && s[1] <= '8')) {
            return false;
    }
    return true;
}


int main() {
    string cmd;
    unique_ptr<Game> g = make_unique<Game>();
    while (cin >> cmd) {
        if (cmd == "draw") g->setDrawLimit();
        if (g->isOver()) g->resetGame();
        if (cmd == "game") {
            string p1, p2;
            bool goodPlayers = false;
            while (!goodPlayers) {
                cin >> p1 >> p2;
                bool stillWrong = false;
                if (p1 != "human" && p1 != CPU1 && p1 != CPU2 && p1 != CPU3 &&
                     p1 != CPU4) {
                    cout << "Invalid player type for player 1" << endl;
                    stillWrong = true;
                }
                if (p2 != "human" && p2 != CPU1 && p2 != CPU2 && p2 != CPU3 &&
                     p2 != CPU4) {
                    cout << "Invalid player type for player 2" << endl;
                    stillWrong = true;
                }
                if (stillWrong) { 
                    cout << "A player type if one of: 'human' or 'computer[x]' where x = 1,2,3,4" << endl;
                } else goodPlayers = true;
            }
            g->startGame(p1, p2);
            cout << *g;
            if (g->isTiedGame()) cout << "Stalemate!" << endl;
            while (!g->isOver()) {
                if (g->getCurrentPlayer()->getType() == PlayerType::Computer) {
                    g->cpuMove();
                    cout << *g;
                    if (g->isOver()) {
                        if (g->isCheckMate()) { // There is mate
                            if (g->getWinningPlayer()->getColour() == Colour::White) {
                                cout << "Checkmate! White wins!" << endl;
                            } else cout << "Checkmate! Black wins!" << endl;
                        } else cout << "Stalemate!" << endl;
                    }
                    else if (g->isChecked()) {
                        if(g->whoIsChecked() == Colour::White) {
                            cout << "White is in check." << endl;
                        } else { cout << "Black is in check." << endl; }
                    }
                    continue;
                }
                if (!(cin >> cmd)) break;
                if (cmd == "resign") {
                    g->resign();
                    if (g->getWinningPlayer()->getColour() == Colour::White) {
                        cout << "White wins!" << endl;
                    } else cout << "Black wins!" << endl;
                    break;
                }
                else if (cmd == "move") {
                    string m1, m2, p;
                    cin >> m1 >> m2;
                    // Check that input is valid
                    if (!isSquareValid(m1) || !isSquareValid(m2)) {
                        cout << "Invalid squares" << endl;
                        continue;
                    }
                    // Is pawn promotion
                    if (g->isPawnPromotion(m1, m2)) {
                        cin >> p;
                        if (!isAPiece(p, whitePieces, blackPieces)) {
                            cout << "Invalid promotion piece" << endl;
                            continue;
                        }
                    } else p = "";
                    if(!(g->makeMove(m1, m2, p))) { cout << "Invalid move" << endl; continue; }
                    cout << *g;
                    if (g->isOver()) {
                        if (g->isCheckMate()) { // There is mate
                            if (g->getWinningPlayer()->getColour() == Colour::White) {
                                cout << "Checkmate! White wins!" << endl;
                            } else cout << "Checkmate! Black wins!" << endl;
                        } else cout << "Stalemate!" << endl;
                    }
                    else if (g->isChecked()) {
                        if(g->whoIsChecked() == Colour::White) {
                            cout << "White is in check." << endl;
                        } else { cout << "Black is in check." << endl; }
                    }
                    continue;
                } else if(!cin.eof()) {
                    cout << "Please use a valid input:" << endl;
                    cout << "  - To make a move: move [start] [end] (ex: move e2 e4)" << endl;
                    cout << "  - To resign the game: resgin" << endl;
                    continue;
                }
            }
        } else if (cmd == "setup") {
            g->useSetup();
            while (cin >> cmd) {
                if (cmd == "+") {
                    string piece, square;
                    cin >> piece >> square;
                    bool falseInput = false;
                    if (piece.length() != 1 || !isAPiece(piece, whitePieces, blackPieces)) {
                        cout << "Invalid piece" << endl;
                        falseInput = true;
                    }
                    if (!isSquareValid(square)) {
                        cout << "Invalid square" << endl;
                        falseInput = true;
                    }
                    if (falseInput) continue;  // Restart loop if input is false
                    g->addPiece(piece[0], square);
                    cout << *g;
                } else if (cmd == "-") {
                    string square;
                    cin >> square;
                    if (!isSquareValid(square)) { 
                        cout << "Invalid square" << endl; 
                        continue; 
                    }
                    g->removePiece(square);
                    cout << *g;
                } else if (cmd == "=") {
                    string colour;
                    cin >> colour;
                    string lowerColour = toLowerCase(colour);
                    if (lowerColour == "white" || lowerColour == "black") { 
                        g->setTurn(lowerColour);
                    } else cout << "Invalid colour" << endl;
                } else if (cmd == "done") {
                    // Check if valid
                    if (g->isValidSetup()) break;
                    else {
                        cout << "Invalid Setup: " << endl;
                        cout << "  - Each player must have exactly one king" << endl;
                        cout << "  - There should be no pawns on the first or last row" << endl;
                        continue;
                    }
                } else {
                    cout << "Please use a valid input:" << endl;
                    cout << "  - To add a piece: + [piece] [square] (ex: + K e1)" << endl;
                    cout << "  - To remove a piece: - [square] (ex: - e1)" << endl;
                    cout << "  - To make it colour's turn next go: = [colour]" << endl;
                    cout << "  - To leave setup mode: done" << endl;
                }
            }
        } else {
            cout << "Please use a valid input:" << endl;
            cout << "   - To start a new game: game [human or computer[1-4]]\
[human or computer[1-4]] (ex: game human computer[2])" << endl;
            cout << "   - To enter setup mode: setup" << endl;
        }
    }
    cout << "Final Score:" << endl;
    cout << "White: " << g->getWhiteScore() << endl;
    cout << "Black: " << g->getBlackScore() << endl;
}
