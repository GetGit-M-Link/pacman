#ifndef CHARACTERS_H
#define CHARACTERS_H

#include <ConsoleWindow.h>
#include <QElapsedTimer>
/**
 * @todo write docs
 */

struct coordinates{
    int x;
    int y;
    coordinates(int x, int y);
      
};
int rrnd(int min_value, int max_value);
bool operator ==(const coordinates &lhs, const coordinates &rhs);
enum move_direction {
    directionUp, directionDown, directionLeft, directionRight, directionNone
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
    coordinates Move(const char& pressedKey, const std::vector<move_direction> &possibleDirections);
    
};
class Ghost : public Meeple{
public:
    virtual ~Ghost() = default;
    Ghost() = default;
    bool isParkedOnDot;
    virtual coordinates Move(const std::vector<move_direction> &possibleDirections, Pacman &pacman, const bool &pillActive) = 0;
    char symbol;
    bool isActive;
    
};
class StupidGhost : public Ghost{
    
public:
    StupidGhost(int x, int y);
    coordinates Move(const std::vector<move_direction> &possibleDirections, Pacman &pacman, const bool &pillActive) override;
};
class NotAsStupidGhost : public Ghost{
    int everySecondMove;
public:
    NotAsStupidGhost(int x, int y);
    coordinates Move(const std::vector<move_direction> &possibleDirections, Pacman &pacman, const bool &pillActive) override;
};


#endif // CHARACTERS_H
