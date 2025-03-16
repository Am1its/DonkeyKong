#pragma once
#include <fstream>
#include <string>
#include <map>
#include "baseGame.h"


class defaultGame : public baseGame {
	bool saveMode; // Indicates whether the game is in save mode
	long seed; // Seed for reproducibility
    std::ofstream stepFile, resultFile; // File streams for saving game steps and results
    std::string stepsFileName, resultsFileName; // File names for saving game steps and results
    std::map<std::string, std::pair<std::string, std::string>> fileMapping; // Maps screen file to .steps and .results files 

public:
    
	defaultGame(bool saveMode); // Constructor 
    ~defaultGame(); // Destructor
	

    void run() override;         // Override the run method to implement the game loop
	void setGame() override; // Override setGame to provide specific behavior for default game

    // Override state handlers to provide specific behavior for default game
    void handleMainMenu() override;
    void handlePlaying() override;
    void handlePaused() override;
    void handleGameOver() override;
    void handleWinner() override;

	//Saving Methods
	void generateSeed(); // Generate a random seed for reproducibility
	void saveStepsToFile(int iteration, char key);  // Save the current iteration and key press
	void saveResultsToFile();   // Save all results from resultStats to file
    void saveSeedToFile();  // Save the seed for reproducibility
	void createSaveFilesForCurrentScreen(); // Create save files for the current screen
	bool validateKey(char key) const; // Validate the key press
	void closeFiles();   // Close all open files
	std::string statusToString(baseGame::status eventStatus) const; // Convert status enum to string

};


