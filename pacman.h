#ifndef PACMAN_H
#define PACMAN_H

#include <ConsoleWindow.h>
#include <QElapsedTimer>

/**
 * @todo write docs
 */

struct coordinates{
    int x;
    int y;
    coordinates(int x, int y){
        this->x = x;
        this->y = y;
    }
    
};
enum move_direction {
    directionUp, directionDown, directionLeft, directionRight, directionNone
};
enum state_of_game {
    gameOver, theGameIsOn, gameWon,
};
class Meeple{
public:
     coordinates position = coordinates(0,0);
     void SetPosition(int x, int y); 
     move_direction direction; 
     
};
class Pacman : public Meeple{
public:
    int points;
    Pacman();
    void EatPoint();
    coordinates Move(const char& pressedKey);
    
};
class StupidGhost : public Meeple{
public:
    StupidGhost(int x, int y);
    bool isParkedOnDot;
    coordinates Move(const std::vector<move_direction> &possibleDirections);
};
 
class PacmanWindow : public ConsoleWindow
{
    int cycle = 3;
    int currentCycle = 0;
    Pacman player;
    std::vector<StupidGhost> stupid_ghosts;
    state_of_game gameState;
    int levelMaxPoints;
    std::vector<std::vector<char>> levelMap;
    std::vector<std::vector<char>> Parsemap(int &pointCount);
    void Initialize();
    std::string timeNeeded;
    QElapsedTimer timer;
    void writeHeader();
    void checkIfWin();
    std::vector<move_direction> GetPossibleDirections(const StupidGhost &ghost);
    void MoveGhosts();
  
    
 protected:
    virtual void onRefresh() override;
    virtual void onKeyPress() override;
 public:
    PacmanWindow();
    
};

#endif // PACMAN_H
