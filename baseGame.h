#pragma once
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include "board.h"

class ghost; // Forward declaration of ghost class
class ghostUpgrade; // Forward declaration of ghostUpgrade class
class mario; // Forward declaration of mario class
class barrels; // Forward declaration of barrels class

class baseGame {
protected:
    enum class GameState { MainMenu, Playing, Paused, GameOver, Winner};
    enum status {Default, BarrelHit, GhostHit, ExplosionHit, FallDamage,HammerHit, HeartCollected, GameOver ,Win, UNKONWN};

    bool silentMode;  // Indicates whether the game is in silent mode
    GameState currentState;
    board gameboard;      
    mario* marioCharacter; // Pointer to Mario

    std::vector<std::unique_ptr<barrels>> barrelsList; // Store barrels as unique pointers
    std::vector<std::unique_ptr<ghost>> ghosts; //Ghosts
    std::vector<std::string> screenFiles;   //Screen files

    std::vector<std::pair<int, status>> results; // Stores iteration and event status


    int currentStageIndex = 0; // Tracks the current stage being played
    int completedStages = 0;   // Tracks how many stages Mario has completed
    int score = 0;           //Mario score
    int iteration = 0;      //Game loop counter

    //Objects variables
    int hammerX, hammerY;
	int marioX, marioY;
	int barrelX, barrelY;
	int ghostX, ghostY;
	int barrelSpawnCounter = 0;

    bool exitGame = false;
	bool heartCollected = false;
    bool stageWinner = false;
    bool useHammer = false;
    bool marioHasHammer = false;
    bool marioFallDammage = false;
    
	void updateBarrels();   // Update the state of all barrels
    void spawnBarrel();    // Spawn a new barrel
	void updateGhosts();    // Update the state of all ghosts
	void initializeGhosts();  // Initialize ghosts for the current stage
    void recordResult(int iteration ,status eventStatus);       // Helper function to save results in the resultStats map

public:
    baseGame();                                             // Constructor
    ~baseGame();                                           // Destructor
    baseGame(const baseGame& other) = delete;             // Disable copy constructor
    baseGame& operator=(const baseGame& other) = delete; // Disable assignment operator

    virtual void run() = 0; // Pure virtual function for running the game loop

	void loadScreenFiles(const std::string& directory = ".");   // Load screen files from the directory
	void checkScreenFiles();                                   // Check for valid screen files

    virtual void setGame();         // Initialize game state
    void resetGame();      // Reset the current stage
	virtual void updateGame();    // Update the game state
    void resetToNewGame();// Reset the entire game

	void drawInLocation(int x, int y, const char* message);  // Draw a message at a specific location on the screen
	void drawCharInLocation(int x, int y, char c);          // Draw a single character at a specific location on the screen
	void addScore(int points);                             // Add points to the score

    //Helper Funcions
    void changeUseHammer() { useHammer = !useHammer; }   
    bool getUseHammer() const { return useHammer; }
    void setHammerPos() { gameboard.getHammer(hammerX, hammerY); } 
    void setmarioHasHammer(bool x) { marioHasHammer = x; }
    bool getmarioHasHammer() const { return marioHasHammer; }
    void setMarioFallDammage() { marioFallDammage = true; }
    void getMarioPos(int& x, int& y) const; 
	void setHeartCollected() { heartCollected = true; }
    void setScore(int x) { score = x; }
    bool isSilentMode() const { return silentMode; }  // Getter for silent mode


    // Game state handling methods to be overridden by derived classes
    virtual void handleMainMenu() {};
    virtual void handlePlaying() {};
    virtual void handlePaused() {};
    virtual void handleGameOver() {};
    virtual void handleWinner() {};
};
