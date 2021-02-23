#include "characters.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <QElapsedTimer>
#include <cstdlib>

coordinates::coordinates(int x, int y){
        this->x = x;
        this->y = y;
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
}
NotAsStupidGhost::NotAsStupidGhost(int x, int y)
{
    
    position = coordinates(x,y);
    isParkedOnDot = true;
    everySecondMove = 0;
}
coordinates StupidGhost::Move(const std::vector<move_direction> &possibleDirections, Pacman &pacman)
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

coordinates NotAsStupidGhost::Move(const std::vector<move_direction> &possibleDirections, Pacman &pacman){
    coordinates movingtoPosition = this->position;
    int max = 0;
    bool up = false;
    bool down= false;
    bool left= false;
    bool right= false;
     if (everySecondMove == 0){
                everySecondMove++;
    for (move_direction direction : possibleDirections){
        if (direction == directionUp){up = true;}
        if (direction == directionDown){down = true;}
        if (direction == directionLeft){left = true;}
        if (direction == directionRight){right = true;}
    }   
    if (up) { 
            movingtoPosition = coordinates(this->position.x, this->position.y-1); 
            max = abs((pacman.position.y - movingtoPosition.y-1));
    }
   
    if (down){
            if (max < (abs(pacman.position.y - movingtoPosition.y+1))){ 
                max = abs(pacman.position.y - movingtoPosition.y+1);
                movingtoPosition = coordinates(this->position.x, this->position.y+1);
            }
    }
    
    if (left) {
            if (max < (abs(pacman.position.x - movingtoPosition.x-1))){
                max = abs(pacman.position.x - movingtoPosition.x-1);
                movingtoPosition = coordinates(this->position.x-1, this->position.y);
            }
    }
    
    if (right){
            if (max < (abs(pacman.position.x - movingtoPosition.x+1))){
                movingtoPosition = coordinates(this->position.x+1, this->position.y);
            }
    }
     }
     else {
         everySecondMove++;
          if (everySecondMove == 2){ everySecondMove = 0; } 
    }
    return movingtoPosition;
}
