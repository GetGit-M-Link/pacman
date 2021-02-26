#ifndef PACMAN_H
#define PACMAN_H

#include "characters.h"
#include <ConsoleWindow.h>
#include <QElapsedTimer>
#include <memory>


/**
 * @todo write docs
 */

enum move_direction;
enum state_of_game {
    gameOver, theGameIsOn, gameWon,
};
enum on_screen { menu, game, smallMenu};
 
class PacmanWindow : public ConsoleWindow
{
    int cycle; 
    int currentCycle; 
    bool pillIsActive;
    bool pillIsOnPoint;
    bool pillHasSpawned;
    QElapsedTimer pillTimer;
    QElapsedTimer pillSpawnTimer;
    Pacman player;
    std::vector<coordinates> ghosts_original_pos;
    std::vector<coordinates> possiblePillPositions;
    std::vector<std::unique_ptr<Ghost>> ghosts;
    state_of_game gameState;
    on_screen currentScreen;
    int levelMaxPoints;
    std::vector<std::vector<char>> levelMap;
    std::string levelFile;
    int timeNeeded;
    int bestTime;
    
    std::vector<std::vector<char>> Parsemap(int &pointCount); 
    void Initialize();
    void writeHeader();
    void writeFooter();
    void MainMenu();
    void WinMenu();
    void GameOverMenu();
    void SmallMenu();
    class Time{
        QElapsedTimer timer;
        int savedTime;
        bool paused;
    public:
        int Elapsed();
        void Pause();
        void Start();
        void UnPause();
    };
    Time clock;
    void checkIfWin();
    void ManagePills();
    std::vector<move_direction> GetPossibleDirections(const Meeple* meeple);
    void MoveGhosts();
    void MovePacman();
    void GhostCollisionEvent();
    void Cleanup();
  
    
 protected:
    virtual void onRefresh() override;
    virtual void onKeyPress() override;
 public:
    PacmanWindow();
    
};

#endif // PACMAN_H
