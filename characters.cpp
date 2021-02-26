#include "characters.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <QElapsedTimer>
#include <cstdlib>
#include <algorithm>

coordinates::coordinates(int x, int y){
        this->x = x;
        this->y = y;
    }
bool operator ==(const coordinates &lhs, const coordinates &rhs){
    if ((lhs.x == rhs.x)&&(lhs.y == rhs.y)){
        return 1;
    }
    else {
        return 0;
    }
} 
    
/// aus ConsoleDemo:
int rrnd(int min_value, int max_value)
{
    double v = std::rand();
    v /= (static_cast<double>(RAND_MAX) + 1.0);
    v *= (max_value - min_value);
    v += min_value;
    return static_cast<int>(v);
}

//Direction-Randomizer
move_direction rrndDirection(){
    int random = rrnd(0,3);
    switch(random){
        case 0:
            return directionUp;
        case 1:
            return directionDown;
        case 2:
            return directionLeft;
        case 3:
            return directionRight;
        default:
            return directionUp;
    }
    
}

Pacman::Pacman(){
   points = 0;
   direction = directionNone;
   
     
};
void Meeple::SetPosition(int x, int y)
{
    position = coordinates(x,y);
}
void Pacman::EatPoint()
{
    points++;
}
coordinates Pacman::Move(const char& pressedKey, const std::vector<move_direction> &possibleDirections)
{
    // Ansatz um das Drücken mehrer Tasten zu unterstützen(für Kurven) momentan bleibt Pacman aber stehen wenn man eine Taste loslässt
    bool up = false;
    bool down= false;
    bool left= false;
    bool right= false;
    for (move_direction direction : possibleDirections){
        if (direction == directionUp){up = true;}
        if (direction == directionDown){down = true;}
        if (direction == directionLeft){left = true;}
        if (direction == directionRight){right = true;}
    }
    coordinates movingtoPosition = this->position;
    if      (pressedKey == 3){
            if (up) { movingtoPosition = coordinates(movingtoPosition.x, (movingtoPosition.y - 1));}}
    if (pressedKey == 4){
             if (down){movingtoPosition = coordinates(movingtoPosition.x, (movingtoPosition.y + 1));}}
    if (pressedKey == 1){
             if (left) {movingtoPosition = coordinates((movingtoPosition.x - 1), movingtoPosition.y);}}
    if (pressedKey == 2){
             if (right){movingtoPosition = coordinates((movingtoPosition.x + 1), movingtoPosition.y);}}
    return movingtoPosition;
}
StupidGhost::StupidGhost(int x, int y){
    position = coordinates(x,y);
    isParkedOnDot = true;
    symbol = 'g';
    isActive = true;
}
NotAsStupidGhost::NotAsStupidGhost(int x, int y)
{
    
    position = coordinates(x,y);
    isParkedOnDot = true;
    everySecondMove = 0;
    symbol = 'G';
    isActive = true;
}
coordinates StupidGhost::Move(const std::vector<move_direction> &possibleDirections, Pacman &pacman, const bool& pillActive)
{
    direction = possibleDirections[rrnd(0,possibleDirections.size())];
    coordinates movingtoPosition = this->position;
    if (direction == directionUp){
         movingtoPosition = coordinates(this->position.x, (this->position.y - 1));}
    if (direction == directionDown){
        movingtoPosition = coordinates(this->position.x, (this->position.y + 1));}
    if (direction == directionLeft){
                movingtoPosition = coordinates((this->position.x - 1), this->position.y);}
    if (direction == directionRight){
        movingtoPosition = coordinates((this->position.x + 1), this->position.y);}
   
   
    return movingtoPosition;
}

coordinates NotAsStupidGhost::Move(const std::vector< move_direction >& possibleDirections, Pacman& pacman, const bool& pillActive)
{
    coordinates movingtoPosition = this->position;
    int up_down = 0;
    int left_right = 0;
    bool done = false;
    move_direction move = directionNone;
    std::vector<move_direction> closerOrAwayFromPackman;
    if (everySecondMove == 0){
                everySecondMove++;
                up_down = pacman.position.y - this->position.y;
                left_right = pacman.position.x - this->position.x;
                
                if (!pillActive){
                if (left_right < 0) {closerOrAwayFromPackman.push_back(directionLeft);}
                else if (left_right > 0) {closerOrAwayFromPackman.push_back(directionRight);}
                if (up_down < 0) {closerOrAwayFromPackman.push_back(directionUp);}
                else if (up_down > 0) {closerOrAwayFromPackman.push_back(directionDown);}
                }
                else  if (pillActive){
                if (left_right > 0) {closerOrAwayFromPackman.push_back(directionLeft);}
                else if (left_right < 0) {closerOrAwayFromPackman.push_back(directionRight);}
                else if (left_right == 0) {closerOrAwayFromPackman.push_back(directionRight);closerOrAwayFromPackman.push_back(directionLeft);}
                if (up_down > 0) {closerOrAwayFromPackman.push_back(directionUp);}
                else if (up_down < 0) {closerOrAwayFromPackman.push_back(directionDown);}
                else if (up_down == 0) {closerOrAwayFromPackman.push_back(directionDown);closerOrAwayFromPackman.push_back(directionUp);}
                }
                
                for (move_direction direction : closerOrAwayFromPackman){
                    for (move_direction possible : possibleDirections){
                        if (direction == possible){move = direction;done = true;break;}
                        if (done) {break;}
                    }
                }
     }
     else {
         everySecondMove++;
          if (everySecondMove == 2){ everySecondMove = 0; } 
    }
     
    if (move == directionLeft) {movingtoPosition = coordinates(this->position.x-1, this->position.y);}
    if (move == directionRight) {movingtoPosition = coordinates(this->position.x+1, this->position.y);}
    if (move == directionUp) {movingtoPosition = coordinates(this->position.x, this->position.y-1);}
    if (move == directionDown) {movingtoPosition = coordinates(this->position.x, this->position.y+1);}
    return movingtoPosition;

}
