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
enum ghost_direction {
    directionUp, directionDown, directionLeft, directionRight,
};
enum state_of_game {
    gameOver, theGameIsOn, gameWon
};
class Pacman{
public:
    coordinates position = coordinates(0,0);
    int points;
    Pacman();
    void SetPosition(int x, int y); 
    void EatPoint();
    
};
class PacmanWindow : public ConsoleWindow
{
    Pacman player;
    state_of_game gameState;
    int levelMaxPoints;
    std::vector<std::vector<char>> levelMap;
    std::vector<std::vector<char>> Parsemap(int &pointCount);
    void Initialize();
    std::string timeNeeded;
    QElapsedTimer timer;
    
 protected:
    virtual void onRefresh() override;
    virtual void onKeyPress() override;
 public:
    PacmanWindow();
    
};

#endif // PACMAN_H
