#ifndef PACMAN_H
#define PACMAN_H

#include "characters.h"
#include <ConsoleWindow.h>
#include <QElapsedTimer>
#include <memory>


/**
 * @todo write docs
 */

enum move_direction;
enum state_of_game {
    gameOver, theGameIsOn, gameWon,
};
enum on_screen { menu, game};
 
class PacmanWindow : public ConsoleWindow
{
    int cycle = 3;
    int currentCycle = 0;
    Pacman player;
    std::vector<coordinates> ghosts_original_pos;
    std::vector<std::unique_ptr<Ghost>> ghosts;
    state_of_game gameState;
    on_screen currentScreen;
    int levelMaxPoints;
    void paintLevel();
    std::vector<std::vector<char>> levelMap;
    std::vector<std::vector<char>> Parsemap(int &pointCount);
    std::string levelFile;
    void Initialize();
    int timeNeeded;
    int bestTime;
    QElapsedTimer timer;
    void writeHeader();
    void writeFooter();
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
