#include "pacman.h"
#include "characters.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <QElapsedTimer>





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
            
            if ((c == 'G')||(c == 'g')) { ghosts_original_pos.push_back(coordinates(column, lineNumber +1));}
            if (c == 'g') { ghosts.push_back(new StupidGhost(column, lineNumber +1));}
            if (c == 'G') { ghosts.push_back(new NotAsStupidGhost(column, lineNumber +1));}
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
    //reset pacman
    player.points = 0;
    //reset ghosts
    for (int32_t i = 0; i < ghosts.size(); i++){
        ghosts[i]->position = ghosts_original_pos[i];
    }
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
std::vector<move_direction> PacmanWindow::GetPossibleDirections(const Meeple* meeple)
{
    std::vector<move_direction> possibleDirections;
    if ((getCharacter(meeple->position.x, meeple->position.y - 1) == ' ')||getCharacter(meeple->position.x, meeple->position.y - 1) == '.'){
        possibleDirections.push_back(directionUp); }
    if ((getCharacter(meeple->position.x, meeple->position.y + 1) == ' ')||getCharacter(meeple->position.x, meeple->position.y + 1) == '.'){
        possibleDirections.push_back(directionDown); }
    if ((getCharacter(meeple->position.x-1, meeple->position.y) == ' ')||getCharacter(meeple->position.x-1, meeple->position.y) == '.'){
        possibleDirections.push_back(directionLeft); }
    if ((getCharacter(meeple->position.x+1, meeple->position.y) == ' ')||getCharacter(meeple->position.x+1, meeple->position.y) == '.'){
        possibleDirections.push_back(directionRight); }
   return possibleDirections;
    
}

void PacmanWindow::MoveGhosts()
{
    for (Ghost* ghost  : ghosts){
        std::vector<move_direction> possibleDirections = GetPossibleDirections(ghost);
        coordinates newPos = ghost->Move(possibleDirections, player);
        bool emptySpace = getCharacter(newPos.x, newPos.y) == ' ';
        bool point = getCharacter(newPos.x, newPos.y) == '.';
        if (emptySpace || point){
            // Verhindere dass Geist Punkte frisst
            if (!ghost->isParkedOnDot){
                setCharacter(ghost->position.x, ghost->position.y, ' ');
            }
            else {
                setCharacter(ghost->position.x, ghost->position.y, '.');
            }
            point ? ghost->isParkedOnDot = true : ghost->isParkedOnDot = false;
            setCharacter(newPos.x, newPos.y, 'G');
            ghost->position = newPos;
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
            std::vector<move_direction> pacmansOptions = GetPossibleDirections(&player);
            movingtoPosition = player.Move(key, pacmansOptions);
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
            if (getPressedKey() == 'n'){ Cleanup();close();}
            break;
            
        case gameOver:
            clear(' ');
            writeString(5,5,"Game Over");
            if (getPressedKey() == 'y'){ Initialize();}
            if (getPressedKey() == 'n'){ Cleanup(); close();}
            break;
        
}
}


void PacmanWindow::onKeyPress()
{ 
         
}
void PacmanWindow::Cleanup()
{
    for (Ghost* ghost : ghosts){
        delete ghost;
        ghosts.pop_back();
    }

}
