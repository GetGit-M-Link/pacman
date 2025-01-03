#include "pacman.h"
#include "characters.h"
#include <memory>
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
    pointCount = 0;
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
            if (c == 'g') { ghosts.push_back(std::make_unique<StupidGhost>(column, lineNumber +1));pointCount++;}
            if (c == 'G') { ghosts.push_back(std::make_unique<NotAsStupidGhost>(column, lineNumber +1));pointCount++;}
            line.push_back(c);
            if (c == '.') {
                pointCount++;
                possiblePillPositions.push_back(coordinates(column, lineNumber +1));
            }
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
    cycle = 2;
    currentCycle = 0;
    
    
    
    

}
void PacmanWindow::Initialize()
{
    int lineNumber = 1;
    gameState = theGameIsOn;
    timeNeeded = 0;
    pillIsActive = false;
    pillSpawnTimer.start();
    pillHasSpawned = false;
    clear(' ');
    //reset pacman
    player.points = 0;
    //reset ghosts
    for (int32_t i = 0; i < ghosts.size(); i++){
        ghosts[i]->position = ghosts_original_pos[i];
        ghosts[i]->isActive = true;
        ghosts[i]->isParkedOnDot = true;
    }
    
    for (std::vector<char> line : levelMap){
        // convert a vector of chars to std::string https://www.techiedelight.com/convert-vector-chars-std-string/ start
         std::string s(line.begin(), line.end());
         // convert a vector of chars to std::string https://www.techiedelight.com/convert-vector-chars-std-string/ ende
         // char in string finden https://www.cplusplus.com/reference/string/string/find/ start
         // Findet Pacmans Position und speichert sie
         std::size_t found = s.find("*");
         if (found!=std::string::npos){
             // char in string finden https://www.cplusplus.com/reference/string/string/find/ ende
            player.SetPosition(static_cast<int>(found), lineNumber);
         }
         // Erstellt Spielfeld
         writeIcons(0,  lineNumber, s );
         lineNumber++;
    }
    clock.Start();
}

void PacmanWindow::writeHeader()
{
    for (int x = 0; x < 64; x++) {
            setCharacter(x, 0, ' ');
        }
    writeString(0,  0, "Score: " + std::to_string(player.points) + "/" + std::to_string(levelMaxPoints));
    writeString(25, 0, "Eis-Pacman!");
    writeString(50, 0,("time:" + std::to_string(clock.Elapsed()) + "sec  "));
}
void PacmanWindow::writeFooter()
{
    writeString(2,  42, "Return to Menu");
    writeString(17,  42, "(m)");
    writeString(2,  43, "Quit");
    writeString(17,  43, "(q) ");
}
void PacmanWindow::MainMenu()
{
    clear(' ');
                clearIcons();
                writeString(28,4,"EIS-Pacman!");
                std::string xline(64,'X');
                writeIcons(0, 6, xline);
                writeString(28,8,"Main Menu");
                writeIcons(0, 10, xline);
                
                writeIcons(20,13,"*");
                writeIcons(10,16,"G");
                writeIcons(30,16,"G");
                writeIcons(8,18,"T-----------------------------t");  
                writeIcons(8,28,"L-----------------------------J");   
     
                writeString(48,18,"Press Key:");
                
                writeString(10,20,"1. Choose Level: " + levelFile);
                writeString(50,20,"1/2/3");
                
                //Schwierigkeitsgrad
                std::string difficulty;
                if (cycle == 0){difficulty = "3/3";}
                else if (cycle == 2){difficulty = "2/3";}
                else if (cycle == 9){difficulty = "1/3";}
                writeString(10,22,"2. Difficulty: " + difficulty);
                writeString(52,22,"s");
                // in onKeypress()
                
                //Spiel starten
                writeString(10,24,"3. Start Game");
                writeString(52,24,"g");
                if (getPressedKey() == 'g'){ 
                    currentScreen = game; 
                    levelMap = Parsemap(levelMaxPoints);
                    Initialize();
                    return;}
                //Spiel beenden (q funktioniert immer daher ist das Tasteneevent weiter oben)
                writeString(10,26,"4. Quit Game");
                writeString(52,26,"q");
}
void PacmanWindow::WinMenu()
{
    clear(' ');
    clearIcons();
    writeIcons(8,3,"T-------------------------------t");
    writeIcons(3, 9, "G");
    writeIcons(45, 9, "G");
    writeIcons(6, 25, "       G              G              G       ");
    writeIcons(6, 35, "G              G              G              G");
    writeString(10,5,"Well done! Level completed in:  ");
    writeString(21,7, (std::to_string(timeNeeded) + " seconds"));
    if ((bestTime > 0)&& (timeNeeded >= bestTime)){
        writeString(10,9,"Best Time: " + std::to_string(bestTime)+ " seconds");
    }
    else if ((bestTime > 0)&& (timeNeeded < bestTime)){
        writeString(10,11,"New Best Time: " + std::to_string(timeNeeded)+ " seconds");
        bestTime = timeNeeded;
    }
    
    
    writeString(10,13,"Want to play again? (y/n) ");
    writeString(10,15,"Back to the menu? (m) ");
    writeIcons(8,17,"L-------------------------------J");  
    if (getPressedKey() == 'y'){
        if (bestTime == 0)
        {
            bestTime = timeNeeded;
        } 
        clearIcons();
        Initialize();}
        
    if (getPressedKey() == 'n'){
        Cleanup();
        close();
        
    }
}
void PacmanWindow::GameOverMenu()
{
     clear(' ');
     clearIcons();
     writeIcons(8,3,"T-------------------------------t");
     writeIcons(3, 9, "g");
     writeIcons(45, 9, "g");
     writeIcons(6, 25, "       g              g              g       ");
     writeIcons(6, 35, "g              g              g              g");
     writeIcons(8,17,"L-------------------------------J");  
     writeString(10,9,"Game Over");
     writeString(10,12,"Want to play again? (y/n) ");
     if (getPressedKey() == 'y'){clearIcons(); Initialize();}
     if (getPressedKey() == 'n'){ Cleanup(); close();}
}
void PacmanWindow::SmallMenu()
{
    clock.Pause();
    for (int x = 0; x < 64; x++) {
            setCharacter(x, 0, ' ');
        }
    writeString(10,0,"Do you really want to quit? y/n");
     
    if (getPressedKey() == 'n'){clock.UnPause();currentScreen=game;}
    if (getPressedKey() == 'y'){ Cleanup();close();}
}
int PacmanWindow::Time::Elapsed()
{
    return ((savedTime + timer.elapsed())/1000);
}
void PacmanWindow::Time::Start()
{
    timer.start();
    savedTime = 0;
    paused = false;
    
}
void PacmanWindow::Time::Pause()
{
    if (!paused){
    paused = true;
    
    savedTime = savedTime + timer.elapsed();
    
    }
    
}
void PacmanWindow::Time::UnPause()
{
    if (paused){
         paused = false;
         timer.start();}
    
}



void PacmanWindow::checkIfWin()
{
    if (player.points == levelMaxPoints){ 
       gameState = gameWon;
       timeNeeded = clock.Elapsed();
       
            }
}
void PacmanWindow::ManagePills()
{ 
    if (pillIsActive){
        if (pillTimer.hasExpired(5000)){
            pillIsActive = false;
        }
    }
    else if (!pillIsActive && !pillHasSpawned){
        if (pillSpawnTimer.hasExpired(5000)){
            coordinates pillCoordinates = possiblePillPositions[rrnd(0,possiblePillPositions.size())];
            if (getIcon(pillCoordinates.x,pillCoordinates.y) == '.'){
                pillIsOnPoint = true;
            }
            else {
                pillIsOnPoint = false;
                
            }
            setIcon(pillCoordinates.x,pillCoordinates.y,'p');
            pillHasSpawned = true;
            pillIsActive = true;
        }
    }
}

std::vector<move_direction> PacmanWindow::GetPossibleDirections(const Meeple* meeple)
{
    std::vector<move_direction> possibleDirections;
    std::vector<char> possibleFields = {' ','.','*','g','G','p'};
    if (possibleFields.end() != std::find(possibleFields.begin(), possibleFields.end(),(getIcon(meeple->position.x, meeple->position.y - 1)))){
            possibleDirections.push_back(directionUp);}
    if (possibleFields.end() != std::find(possibleFields.begin(), possibleFields.end(),(getIcon(meeple->position.x, meeple->position.y + 1)))){
            possibleDirections.push_back(directionDown);}
    if (possibleFields.end() != std::find(possibleFields.begin(), possibleFields.end(),(getIcon(meeple->position.x-1, meeple->position.y)))){
            possibleDirections.push_back(directionLeft);}
    if (possibleFields.end() != std::find(possibleFields.begin(), possibleFields.end(),(getIcon(meeple->position.x+1, meeple->position.y)))){
            possibleDirections.push_back(directionRight);}
   return possibleDirections;
    
}

void PacmanWindow::MoveGhosts()
{
    for (int i = 0; i< ghosts.size(); i++){
        Ghost* ghost = ghosts[i].get();
        if (ghost->isActive){
            std::vector<move_direction> possibleDirections = GetPossibleDirections(ghost);
            coordinates newPos = ghost->Move(possibleDirections, player, pillIsActive);
            bool emptySpace = getIcon(newPos.x, newPos.y) == ' ';
            bool point = getIcon(newPos.x, newPos.y) == '.';
            bool eatPacman = getIcon(newPos.x, newPos.y) == '*';
            bool destroyPill = getIcon(newPos.x, newPos.y) == 'p';

            if (emptySpace || point || destroyPill){
                // Verhindere dass Geist Punkte frisst
                if (!ghost->isParkedOnDot){
                    setIcon(ghost->position.x, ghost->position.y, ' ');
                }
                else {
                    setIcon(ghost->position.x, ghost->position.y, '.');
                }
                if (destroyPill){
                pillIsOnPoint?  ghost->isParkedOnDot = true : ghost->isParkedOnDot = false;
                }
                else {
                    point ? ghost->isParkedOnDot = true : ghost->isParkedOnDot = false;
                }
                setIcon(newPos.x, newPos.y, ghost->symbol);
                ghost->position = newPos;
            
            }
            
            else    {
                if (eatPacman){
                    ghost->position = newPos;
                    GhostCollisionEvent();
                }
         
         
        }
        }
        }
}
void PacmanWindow::MovePacman()
{
    std::vector<move_direction> pacmansOptions = GetPossibleDirections(&player);
    coordinates movingtoPosition = player.position;
    char key = getPressedKey();
    bool emptySpace;
    bool pointFood;
    bool pillFood;
    bool ghostCollision;
    movingtoPosition = player.Move(key, pacmansOptions);
    
    //Regelt Konsequenzen der Bewegung
    emptySpace = getIcon(movingtoPosition.x, movingtoPosition.y) == ' ';
    pointFood = getIcon(movingtoPosition.x, movingtoPosition.y) == '.';
    pillFood = getIcon(movingtoPosition.x, movingtoPosition.y) == 'p';
    ghostCollision = ((getIcon(movingtoPosition.x, movingtoPosition.y) == 'g')||(getIcon(movingtoPosition.x, movingtoPosition.y) == 'G'));
    if ( emptySpace || pointFood || pillFood){
                setIcon(player.position.x, player.position.y, ' ');
                setIcon(movingtoPosition.x, movingtoPosition.y, '*');
                player.position = movingtoPosition;
                if (pointFood){
                        player.EatPoint();
                    }
                else if (pillFood){
                        pillIsActive = true;
                        pillTimer.start();
                        if (pillIsOnPoint){player.EatPoint();}
                    }
                }
    if (ghostCollision){
        
        setIcon(player.position.x, player.position.y, ' ');
        player.position = movingtoPosition;
       
        GhostCollisionEvent();    
    }
}

void PacmanWindow::GhostCollisionEvent()
{
    
    if (!pillIsActive) {
    gameState = gameOver;
    }
    else if (pillIsActive) {
        for (int i = 0; i< ghosts.size(); i++){
        Ghost* ghost = ghosts[i].get();
        if (ghost->isActive){
            if (ghost->position == player.position){
                ghost->isActive = false;
                
                if (ghost->isParkedOnDot){
                    setIcon(ghost->position.x, ghost->position.y, ' ');
                    player.EatPoint();
                }
                else {
                    setIcon(ghost->position.x, ghost->position.y, ' ');
                }
            }
        }
        
    }
}
}


void PacmanWindow::onRefresh()
{
     
     char key = getPressedKey();
     
     // Spiel beenden mit q / Zurück ins Menü mit m
     if (key == 'q'){ currentScreen = smallMenu;}
     if (key == 'm'){ Cleanup();currentScreen = menu;}
     
     //switch zwischen Menü und Spiel
     switch(currentScreen){
         case smallMenu:
             SmallMenu();
             break;
         case game:
    
             //switch zwischen laufendem Spiel und Anzeige nachdem das Spiel gewonnen oder verloren wurde
            switch(gameState){
                    case theGameIsOn:
                            writeHeader();
                            writeFooter();
                            checkIfWin();
                            ManagePills();
                            // Reguliert die Geschwindigkeit des Spiels indem es nur in gewissen Refresh Cyclen Bewegung erlaubt
                            if (currentCycle == 0){
                                    currentCycle++;
                                    if (currentCycle > cycle){ currentCycle = 0; } 
                                    MovePacman();
                                    MoveGhosts();
                            }
                            else {
                                    currentCycle++;
                                    if (currentCycle > cycle){ currentCycle = 0; } 
                                    }
                
                            break;
                
                    case gameWon:
                            WinMenu();
                            break;
                
                    case gameOver:
                            GameOverMenu();
                            break;
            
            }
                break;
                case menu:
                    MainMenu();
                    break;
                
            }
}


void PacmanWindow::onKeyPress()
{  switch(currentScreen){
                case smallMenu:
                    break;
                case game:
                    break;
                case menu:
                    if (getPressedKey() == 's'){
                        if (cycle == 0){cycle = 9;}
                        else if (cycle == 2){cycle = 0;}
                        else if (cycle == 9){cycle = 2;}
                    }
                    if (getPressedKey() == '1'){levelFile = levels["1"];}
                    if (getPressedKey() == '2'){levelFile = levels["2"];}
                    if (getPressedKey() == '3'){levelFile = levels["3"];}
                    break;
        }       
}
void PacmanWindow::Cleanup()
{
    ghosts.clear();
    clearIcons();
    }
   
 
