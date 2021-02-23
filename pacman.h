#ifndef PACMAN_H
#define PACMAN_H

#include <ConsoleWindow.h>
#include <QElapsedTimer>
#include "characters.h"

/**
 * @todo write docs
 */

enum move_direction;
enum state_of_game {
    gameOver, theGameIsOn, gameWon,
};

 
class PacmanWindow : public ConsoleWindow
{
    int cycle = 3;
    int currentCycle = 0;
    Pacman player;
    std::vector<coordinates> ghosts_original_pos;
    std::vector<Ghost*> ghosts;
    state_of_game gameState;
    int levelMaxPoints;
    std::vector<std::vector<char>> levelMap;
    std::vector<std::vector<char>> Parsemap(int &pointCount);
    void Initialize();
    std::string timeNeeded;
    QElapsedTimer timer;
    void writeHeader();
    void checkIfWin();
    std::vector<move_direction> GetPossibleDirections(const Meeple* meeple);
    void MoveGhosts();
    void Cleanup();
  
    
 protected:
    virtual void onRefresh() override;
    virtual void onKeyPress() override;
 public:
    PacmanWindow();
    
};

#endif // PACMAN_H
