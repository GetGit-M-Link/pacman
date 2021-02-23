#include "pacman.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <QElapsedTimer>


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
coordinates Pacman::Move(const char& pressedKey)
{
    coordinates movingtoPosition = this->position;
    if      (pressedKey == 3){
                movingtoPosition = coordinates(this->position.x, (this->position.y - 1));}
            if (pressedKey == 4){
                movingtoPosition = coordinates(this->position.x, (this->position.y + 1));}
            if (pressedKey == 1){
                movingtoPosition = coordinates((this->position.x - 1), this->position.y);}
            if (pressedKey == 2){
                movingtoPosition = coordinates((this->position.x + 1), this->position.y);}
    return movingtoPosition;
}
StupidGhost::StupidGhost(int x, int y){
    position = coordinates(x,y);
    isParkedOnDot = true;
    
    
}
coordinates StupidGhost::Move(const std::vector<move_direction> &possibleDirections)
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




std::vector<std::vector<char>> PacmanWindow::Parsemap(int &pointCount)
{
    std::vector<std::vector<char>> Levelmap;
		
    std::fstream file;
	file.open("/home/snow/Uni/EiS/prüfung/ConsoleWindow/copyme/level1.txt", std::ios::in);
	if(!file.good())
		std::cout << "FILE OPENING ERROR!!" << std::endl;

    std::string s;
    int lineNumber = 0;
    while (std::getline(file, s)) {
        std::vector<char> line;
        int column = 0;
        for (char c : s){
            
            if (c == 'G') { stupid_ghosts.push_back(StupidGhost(column, lineNumber +1));}
            if (c == 'g') { c = 'G'; stupid_ghosts.push_back(StupidGhost(column, lineNumber));}
            line.push_back(c);
            if (c == '.') { pointCount++;}
            column++;
        }
        Levelmap.push_back(line);
        lineNumber++;
    }
	
    return Levelmap;
}

PacmanWindow::PacmanWindow()
    : ConsoleWindow()
{ 
    gameState = theGameIsOn;
    levelMaxPoints = 0;
    levelMap = Parsemap(levelMaxPoints);
    Initialize();

}
void PacmanWindow::Initialize()
{
    int lineNumber = 1;
    gameState = theGameIsOn;
    timeNeeded = "";
    player.points = 0;
    writeString(0,  0, "Punkte: " + std::to_string(player.points) + "/" + std::to_string(levelMaxPoints) );
    for (std::vector<char> line : levelMap){
        // convert a vector of chars to std::string https://www.techiedelight.com/convert-vector-chars-std-string/ start
         std::string s(line.begin(), line.end());
         // convert a vector of chars to std::string https://www.techiedelight.com/convert-vector-chars-std-string/ ende
         // char in string finden https://www.cplusplus.com/reference/string/string/find/ start
         // Findet Pacmans Position und speichert sie
         std::size_t found = s.find("*");
         if (found!=std::string::npos){
            player.SetPosition(static_cast<int>(found), lineNumber);
         }
         // Erstellt Spielfeld
         writeString(0,  lineNumber, s );
         lineNumber++;
    }
    timer.start();
}

void PacmanWindow::writeHeader()
{
    writeString(0,  0, "Punkte: " + std::to_string(player.points) + "/" + std::to_string(levelMaxPoints) + "        Eis-Pacman!         Zeit:" + std::to_string(timer.elapsed()/1000) + "sec");
}
void PacmanWindow::checkIfWin()
{
    if (player.points == levelMaxPoints){ 
       gameState = gameWon;
       timeNeeded = std::to_string(timer.elapsed()/1000);
            }
}
std::vector<move_direction> PacmanWindow::GetPossibleDirections(const StupidGhost& ghost)
{
    std::vector<move_direction> possibleDirections;
    if ((getCharacter(ghost.position.x, ghost.position.y - 1) == ' ')||getCharacter(ghost.position.x, ghost.position.y - 1) == '.'){
        possibleDirections.push_back(directionUp); }
    if ((getCharacter(ghost.position.x, ghost.position.y + 1) == ' ')||getCharacter(ghost.position.x, ghost.position.y + 1) == '.'){
        possibleDirections.push_back(directionDown); }
    if ((getCharacter(ghost.position.x-1, ghost.position.y) == ' ')||getCharacter(ghost.position.x-1, ghost.position.y) == '.'){
        possibleDirections.push_back(directionLeft); }
    if ((getCharacter(ghost.position.x+1, ghost.position.y) == ' ')||getCharacter(ghost.position.x+1, ghost.position.y) == '.'){
        possibleDirections.push_back(directionRight); }
   return possibleDirections;
    
}

void PacmanWindow::MoveGhosts()
{
    for (StupidGhost &ghost  : stupid_ghosts){
        std::vector<move_direction> possibleDirections = GetPossibleDirections(ghost);
        coordinates newPos = ghost.Move(possibleDirections);
        bool emptySpace = getCharacter(newPos.x, newPos.y) == ' ';
        bool point = getCharacter(newPos.x, newPos.y) == '.';
        if (emptySpace || point){
            // Verhindere dass Geist Punkte frisst
            if (!ghost.isParkedOnDot){
                setCharacter(ghost.position.x, ghost.position.y, ' ');
            }
            else {
                setCharacter(ghost.position.x, ghost.position.y, '.');
            }
            point ? ghost.isParkedOnDot = true : ghost.isParkedOnDot = false;
            setCharacter(newPos.x, newPos.y, 'G');
            ghost.position = newPos;
        }
        else {
         //std::cout << s  << std::endl;
         
     }
    }
}


void PacmanWindow::onRefresh()
{
     coordinates movingtoPosition = player.position;
     bool emptySpace;
     bool pointFood;
     bool ghostCollision;
     char key = getPressedKey();
     
    
    switch(gameState){
        case theGameIsOn:
            writeHeader();
            checkIfWin();
            if (currentCycle == 0){
                currentCycle++;
            movingtoPosition = player.Move(key);
            MoveGhosts();
            
            //Untersucht das Feld auf dass sich bewegt werden soll
            emptySpace = getCharacter(movingtoPosition.x, movingtoPosition.y) == ' ';
            pointFood = getCharacter(movingtoPosition.x, movingtoPosition.y) == '.';
            ghostCollision = ((getCharacter(movingtoPosition.x, movingtoPosition.y) == 'g')||(getCharacter(movingtoPosition.x, movingtoPosition.y) == 'G'));
            
            if ( emptySpace || pointFood ){
                setCharacter(player.position.x, player.position.y, ' ');
                setCharacter(movingtoPosition.x, movingtoPosition.y, '*');
                player.position = movingtoPosition;
                if (pointFood){
                    player.EatPoint();
                }
            }
            if (ghostCollision){
                gameState = gameOver;
            }
                
            }
            else {
                currentCycle++;
                if (currentCycle == cycle){ currentCycle = 0; } 
            }
            break;
            
        case gameWon:
            clear(' ');
             writeString(5,5,"Klasse! Level geschafft in " + timeNeeded + " Sekunden" );
            if (getPressedKey() == 'y'){ Initialize();}
            break;
            
        case gameOver:
            clear(' ');
            writeString(5,5,"Game Over");
            break;
        
}
}


void PacmanWindow::onKeyPress()
{ 
         
}
