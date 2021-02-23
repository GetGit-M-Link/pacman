#include "pacman.h"
#include "characters.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <QElapsedTimer>
#include <unordered_map>


std::unordered_map<std::string, std::string> levels = {
    {"1", "level1.txt"},
    {"2", "level2.txt"},
    {"3", "level3.txt"},
};


std::vector<std::vector<char>> PacmanWindow::Parsemap(int &pointCount)
{
    std::vector<std::vector<char>> Levelmap;
    Cleanup();
    ghosts_original_pos.clear();
    std::fstream file;
	file.open(levelFile, std::ios::in);
	if(!file.good())
		std::cout << "FILE OPENING ERROR!!" << std::endl;

    std::string s;
    int lineNumber = 0;
    while (std::getline(file, s)) {
        std::vector<char> line;
        int column = 0;
        for (char c : s){
            
            if ((c == 'G')||(c == 'g')) { ghosts_original_pos.push_back(coordinates(column, lineNumber +1));}
            if (c == 'g') { ghosts.push_back(new StupidGhost(column, lineNumber +1));pointCount++;}
            if (c == 'G') { ghosts.push_back(new NotAsStupidGhost(column, lineNumber +1));pointCount++;}
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
    currentScreen = menu;
    bestTime = 0;
    gameState = theGameIsOn;
    levelMaxPoints = 0;
    levelFile = "level1.txt";
    
    

}
void PacmanWindow::Initialize()
{
    int lineNumber = 1;
    gameState = theGameIsOn;
    timeNeeded = 0;
    clear(' ');
    //reset pacman
    player.points = 0;
    //reset ghosts
    for (int32_t i = 0; i < ghosts.size(); i++){
        ghosts[i]->position = ghosts_original_pos[i];
        ghosts[i]->isParkedOnDot = true;
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
    writeString(0,  0, "Score: " + std::to_string(player.points) + "/" + std::to_string(levelMaxPoints) + "        Eis-Pacman!         time:" + std::to_string(timer.elapsed()/1000) + "sec");
}
void PacmanWindow::checkIfWin()
{
    if (player.points == levelMaxPoints){ 
       gameState = gameWon;
       timeNeeded = timer.elapsed()/1000;
            }
}
std::vector<move_direction> PacmanWindow::GetPossibleDirections(const Meeple* meeple)
{
    std::vector<move_direction> possibleDirections;
    if ((getCharacter(meeple->position.x, meeple->position.y - 1) == ' ')||(getCharacter(meeple->position.x, meeple->position.y - 1) == '.')||getCharacter(meeple->position.x, meeple->position.y - 1) =='*'){
        possibleDirections.push_back(directionUp); }
    if ((getCharacter(meeple->position.x, meeple->position.y + 1) == ' ')||(getCharacter(meeple->position.x, meeple->position.y + 1) == '.')||(getCharacter(meeple->position.x, meeple->position.y + 1) == '*')){
        possibleDirections.push_back(directionDown); }
    if ((getCharacter(meeple->position.x-1, meeple->position.y) == ' ')||(getCharacter(meeple->position.x-1, meeple->position.y) == '.')||(getCharacter(meeple->position.x-1, meeple->position.y) == '*')){
        possibleDirections.push_back(directionLeft); }
    if ((getCharacter(meeple->position.x+1, meeple->position.y) == ' ')||(getCharacter(meeple->position.x+1, meeple->position.y) == '.')||(getCharacter(meeple->position.x+1, meeple->position.y) == '*')){
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
        bool eatPacman = getCharacter(newPos.x, newPos.y) == '*';
        if (emptySpace || point){
            // Verhindere dass Geist Punkte frisst
            if (!ghost->isParkedOnDot){
                setCharacter(ghost->position.x, ghost->position.y, ' ');
            }
            else {
                setCharacter(ghost->position.x, ghost->position.y, '.');
            }
            point ? ghost->isParkedOnDot = true : ghost->isParkedOnDot = false;
            setCharacter(newPos.x, newPos.y, ghost->symbol);
            ghost->position = newPos;
            
        }
        else {
            if (eatPacman){gameState = gameOver;}
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
     
     // Spiel beenden mit q / Zurück ins Menü mit m
     if (key == 'q'){ Cleanup();close();}
     if (key == 'm'){ Cleanup();currentScreen = menu;}
     
     //switch zwischen Menü und Spiel
     switch(currentScreen){
         case game:
    
             //switch zwischen laufendem Spiel und Anzeige nachdem das Spiel gewonnen oder verloren wurde
            switch(gameState){
                case theGameIsOn:
                writeHeader();
                checkIfWin();
                // Reguliert die Geschwindigkeit des Spiels indem es nur in gewissen Refresh Cyclen Bewegung erlaubt
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
                writeString(5,5,"Well done Level completed in:  " + std::to_string(timeNeeded) + " seconds" );
                writeString(5,7,"Want to play again? (y/n) ");
                writeString(5,9,"Back to the menu? (m) ");
                if (getPressedKey() == 'y'){ Initialize();}
                if (getPressedKey() == 'n'){ Cleanup();close();}
                
                break;
                
            case gameOver:
                clear(' ');
                writeString(5,5,"Game Over");
                writeString(5,7,"Want to play again? (y/n) ");
                if (getPressedKey() == 'y'){ Initialize();}
                if (getPressedKey() == 'n'){ Cleanup(); close();}
                break;
            
}
                break;
            case menu:
                clear(' ');
                writeString(28,4,"EIS-Pacman!");
                std::string xline(64,'X');
                writeString(0, 6, xline);
                writeString(28,8,"Main Menu");
                writeString(0, 10, xline);
                
                writeString(10,13,"1. Choose Level: " + levelFile + " Press 1/2/3");
                
                //Schwierigkeitsgrad
                std::string difficulty;
                if (cycle == 1){difficulty = "hard";}
                else if (cycle == 3){difficulty = "moderate";}
                else if (cycle == 10){difficulty = "easy";}
                writeString(10,14,"2. difficulty: " + difficulty + " (Change with (s))");
                // in onKeypress()
                
                //Steuerung
                writeString(10,15,"3. Controls:");
                
                //Spiel starten
                writeString(10,16,"4. Start Game       (g)");
                if (key == 'g'){ 
                    currentScreen = game; 
                    levelMap = Parsemap(levelMaxPoints);
                    Initialize();
                    break;}
                //Spiel beenden (q funktioniert immer daher ist das Tasteneevent weiter oben)
                writeString(10,17,"5. Quit Game       (q)");
                
                break;
                
}
}


void PacmanWindow::onKeyPress()
{  switch(currentScreen){
         case game:
             break;
         case menu:
             if (getPressedKey() == 's'){
                    if (cycle == 1){cycle = 10;}
                    else if (cycle == 3){cycle = 1;}
                    else if (cycle == 10){cycle = 3;}
                }
                if (getPressedKey() == '1'){levelFile = levels["1"];}
                if (getPressedKey() == '2'){levelFile = levels["2"];}
                if (getPressedKey() == '3'){levelFile = levels["3"];}
                break;
    }       
}
void PacmanWindow::Cleanup()
{
    for (Ghost* ghost : ghosts){
        delete ghost;
        ghosts.pop_back();
    }

}
