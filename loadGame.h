#pragma once
#include <fstream>
#include <string>
#include <vector>
#include "baseGame.h" 

class loadGame : public baseGame {
private:
    bool silentMode; // Indicates whether silent mode is enabled
    long seed; // Seed for reproducibility

    std::ifstream stepFile; // Input stream for reading steps
    std::ifstream resultFile; // Input stream for reading expected results
    std::string stepsFileName, resultsFileName; // File names for steps and results
    std::vector<std::pair<int, status>> expectedResults; // Map to store expected results from .results file

    // Helper functions
    void loadSeedFromFile(); // Load the seed from the .steps file
    void loadResultsFromFile(); // Load expected results from the .results file 
    void validateResults(); // Validate actual results against expected results
    void closeFiles(); // Close all open files
	bool initializeScreen(const std::string& screen); // Initialize the screen from the file


public:
    loadGame(bool _silentMode); // Constructor
    ~loadGame(); // Destructor
	void setGame() override; // Override the setGame method to implement load game logic
    void run() override; // Override the run method to implement load game logic
	void updateGame() override; // Override the updateGame method to implement load game logic
    void handleLoadPlaying() ; // Override the handlePlaying method to implement load game logic
    baseGame::status parseStatus(const std::string& statusStr); // Parse status from string to enum
    std::string statusToString(baseGame::status s); // Convert status enum to string
    void addResult(int iteration, baseGame::status eventStatus); // Add a result to the expected results map 

};
