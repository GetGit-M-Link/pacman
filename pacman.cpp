#include "pacman.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <QElapsedTimer>

Pacman::Pacman(){
   points = 0;
   
     
};
void Pacman::SetPosition(int x, int y)
{
    position = coordinates(x,y);
}
void Pacman::EatPoint()
{
    points++;
}


std::vector<std::vector<char>> PacmanWindow::Parsemap(int &pointCount)
{
    std::vector<std::vector<char>> Levelmap;
		
    std::fstream file;
	file.open("/home/snow/Uni/EiS/prüfung/ConsoleWindow/copyme/level1.txt", std::ios::in);
	if(!file.good())
		std::cout << "FILE OPENING ERROR!!" << std::endl;

    std::string s;
    while (std::getline(file, s)) {
        std::vector<char> line;
        for (char c : s){
            if (c == 'G') { c = ' ';}
            if (c == 'g') { c = ' ';}
            line.push_back(c);
            if (c == '.') { pointCount++;}
        }
        Levelmap.push_back(line);
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
    std::cout << levelMaxPoints << std::endl;
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


void PacmanWindow::onRefresh()
{
    switch(gameState){
        case theGameIsOn:
            writeString(0,  0, "Punkte: " + std::to_string(player.points) + "/" + std::to_string(levelMaxPoints) + "        Eis-Pacman!         Zeit:" + std::to_string(timer.elapsed()/1000) + "sec");
            if (player.points == levelMaxPoints){ 
                gameState = gameWon;
                timeNeeded = std::to_string(timer.elapsed()/1000);
            }
            break;
        case gameWon:
            clear(' ');
            writeString(5,5,"Klasse! Level geschafft in " + timeNeeded + " Sekunden" );
            break;
        case gameOver:
            clear(' ');
            writeString(5,5,"Game Over");
            break;
        
}
}

void PacmanWindow::onKeyPress()
{
     coordinates movingtoPosition = player.position;
     bool emptySpace;
     bool pointFood;
    switch(gameState){
        case theGameIsOn:
           
    
            if (getPressedKey() == CURSOR_UP){
                movingtoPosition = coordinates(player.position.x, (player.position.y - 1));}
            if (getPressedKey() == CURSOR_DOWN){
                movingtoPosition = coordinates(player.position.x, (player.position.y + 1));}
            if (getPressedKey() == CURSOR_LEFT){
                movingtoPosition = coordinates((player.position.x - 1), player.position.y);}
            if (getPressedKey() == CURSOR_RIGHT){
                movingtoPosition = coordinates((player.position.x + 1), player.position.y);}
            emptySpace = getCharacter(movingtoPosition.x, movingtoPosition.y) == ' ';
            pointFood = getCharacter(movingtoPosition.x, movingtoPosition.y) == '.';
            if ( emptySpace || pointFood ){
                setCharacter(player.position.x, player.position.y, ' ');
                setCharacter(movingtoPosition.x, movingtoPosition.y, '*');
                player.position = movingtoPosition;
            if (pointFood){
                player.EatPoint();
            
            }
        }
            onRefresh();
            break;
        case gameWon:
            clear(' ');
            writeString(5,5,"Congratulations you beat the level");
            if (getPressedKey() == 'y'){ Initialize();}
            break;
        case gameOver:
            clear(' ');
            writeString(5,5,"Game Over");
            break;
    }

         
}
