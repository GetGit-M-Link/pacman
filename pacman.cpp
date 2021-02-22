#include "pacman.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

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


std::vector<std::vector<char>> PacmanWindow::Parsemap()
{
    std::vector<std::vector<char>> Levelmap;
		
    std::fstream file;
	file.open("/home/snow/Uni/EiS/prüfung/ConsoleWindow/copyme/simple-level.txt", std::ios::in);
	if(!file.good())
		std::cout << "FILE OPENING ERROR!!" << std::endl;

    std::string s;
    while (std::getline(file, s)) {
        std::vector<char> line(s.begin(),s.end());
        Levelmap.push_back(line);
    }
	
    return Levelmap;
}

PacmanWindow::PacmanWindow()
    : ConsoleWindow()
{ 
    gameState = theGameIsOn;
    levelMap = Parsemap();
    Initialize();

}
void PacmanWindow::Initialize()
{
    int lineNumber = 0;
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
}


void PacmanWindow::onRefresh()
{
       
}

void PacmanWindow::onKeyPress()
{
    coordinates movingtoPosition = player.position;
    
    if (getPressedKey() == CURSOR_UP)
        movingtoPosition = coordinates(player.position.x, (player.position.y - 1));
    if (getPressedKey() == CURSOR_DOWN)
        movingtoPosition = coordinates(player.position.x, (player.position.y + 1));
    if (getPressedKey() == CURSOR_LEFT)
        movingtoPosition = coordinates((player.position.x - 1), player.position.y);
    if (getPressedKey() == CURSOR_RIGHT)
        movingtoPosition = coordinates((player.position.x + 1), player.position.y);
    bool emptySpace = getCharacter(movingtoPosition.x, movingtoPosition.y) == ' ';
    bool pointFood = getCharacter(movingtoPosition.x, movingtoPosition.y) == '.';
    if ( emptySpace || pointFood ){
        setCharacter(player.position.x, player.position.y, ' ');
        setCharacter(movingtoPosition.x, movingtoPosition.y, '*');
        player.position = movingtoPosition;
        if (pointFood){
            player.EatPoint();
    }
    }
    onRefresh();

         
}
