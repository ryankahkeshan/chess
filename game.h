#ifndef __GAME_H__
#define __GAME_H__
#include <iostream>
#include <string>
#include <memory>
#include "board.h"
#include "player.h"
#include "constants.h"
class Board;
class Player;

enum class Colour {White, Black};

class Game {
  std::unique_ptr<Player> p1;
  std::unique_ptr<Player> p2;
  std::unique_ptr<Board> gameBoard;
  std::vector<std::shared_ptr<Piece>> enPassantPawns;
  bool isResigned = false;
  float whiteScore = 0;
  float blackScore = 0;
  bool setupUsed = false;
  void defaultSetUp();
  int noCaptureMoves = 0;
  bool drawLimit = false;
  Colour startTurn = Colour::White;
  public:
    Game();  // Default constructor

    void resetGame();  // To reset the game when one finishes
    void startGame(std::string p1, std::string p2);  // Starts the game with players p1 and p2
    void useSetup(); // Enters setup mode

    bool isOver() const;
    bool isCheckMate() const;  // Returns true if game is over, false otherwise
    // If only two kings or there are no moves for one player (and draw limit if on)
    bool isTiedGame() const;
    void setDrawLimit();  // Set draw limit on (50 by default)


    bool isChecked() const;  // Returns true if someone is in check, false otherwise
    Colour whoIsChecked();  // Ouputs when someone is checked

    void resign();  // Outputs when someone resigns

    float getWhiteScore() const;  // Returns score of white players throughout games
    float getBlackScore() const;  // Returns score of black players throughout games

    const std::unique_ptr<Player>& getCurrentPlayer() const;
    const std::unique_ptr<Player>& getNextPlayer() const;
    const std::unique_ptr<Player>& getWinningPlayer() const; // Only call if game is over is certain
    
    bool isPawnPromotion(std::string m1, std::string m2);
    bool makeMove(std::string m1, std::string m2, std::string p);
    void cpuMove();

    void addPiece(char piece, std::string square);
    void removePiece(std::string square);

    bool isValidSetup();

    void setTurn(std::string colour);

    friend std::ostream &operator<<(std::ostream &out, const Game &g);
};

#endif
