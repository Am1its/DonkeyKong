
#include "loadGame.h"
#include <iostream>
#include "mario.h"
#include <stdexcept>
#include <sstream>
#include <chrono>
#include <random>
#include "ghost.h"
#include "barrels.h"
#include <cstdlib>   // For srand() and rand()
#include <ctime>     // For time()

// Constructor
loadGame::loadGame(bool _silentMode)
    : baseGame(), silentMode(_silentMode), seed(0) {
    baseGame::silentMode = _silentMode;
}

// Destructor
loadGame::~loadGame() {
    closeFiles();
}

// Run the load game mode
void loadGame::run() {
    loadScreenFiles();  // Load all valid screen files
    checkScreenFiles(); // Check for valid screen files

    for (size_t i = 0; i < screenFiles.size(); ++i) {
        std::cout << "Loading screen: " << screenFiles[i] << std::endl;
        Sleep(1000); // Sleep for 1 second
        if (!initializeScreen(screenFiles[i])) {
            if (!silentMode) std::cerr << "Skipping invalid board: " << screenFiles[i] << std::endl;
            continue;
        }

        // Handle playing in silent or normal mode
        handleLoadPlaying();

    }

    if (silentMode) {
        std::cout << "Test completed. Silent mode validation finished.\n";
    }
    else {
        std::cout << "All boards completed.\n";
    }
}

// Update the game state for load mode
void loadGame::updateGame()
{
    if (heartCollected)
    {
        heartCollected = false;
        recordResult(iteration, status::HeartCollected);
    }
    
    if(!isSilentMode()) marioCharacter->draw();
    addScore(10); // Increment score for surviving
    iteration++;

    updateBarrels();
    updateGhosts();

    if(!isSilentMode()) gameboard.printLegendInfo(marioCharacter->getLife(), score, getmarioHasHammer()); 
    if (gameboard.getChar(marioCharacter->getX(), marioCharacter->getY()) == utilities::PAULINA) { 
        recordResult(iteration, status::Win);  // Record the stage completion status 
        return; 
    }
    
    if (!silentMode) {
        Sleep(70);
        marioCharacter->erase();
    } 
    
    marioCharacter->move();  
    marioCharacter->resetPressP(); 

}

// Initialize the screen and open the necessary files
bool loadGame::initializeScreen(const std::string& screen) {
    closeFiles(); // Close any open files
    gameboard.loadBoardFromFile(screen); // Load the board from the file

    if (!gameboard.validateBoard()) {
        return false;
    }

    stepsFileName = screen.substr(0, screen.find_last_of('.')) + ".steps";
    resultsFileName = screen.substr(0, screen.find_last_of('.')) + ".result";

    stepFile.open(stepsFileName);
    resultFile.open(resultsFileName);

    if (!stepFile.is_open() || !resultFile.is_open()) {
        throw std::runtime_error("Could not open required files."); // Throw an exception if files are not open
    }

    loadSeedFromFile();
    loadResultsFromFile();
    setGame();

    return true;
}

// Load the seed from the .steps file
void loadGame::loadSeedFromFile() {
    std::string line;

    // Read the first two lines
    for (int i = 0; i < 2; ++i) {
        if (!std::getline(stepFile, line)) {
            throw std::runtime_error("Error: Missing necessary information in the .steps file.");
        }

        if (line.rfind("SEED", 0) == 0) {
            seed = std::stol(line.substr(5));
            std::srand(seed);  // Initialize random generator with this seed
        }
        else if (line.rfind("SCORE", 0) == 0) {
            std::istringstream stateStream(line);
            std::string dummy;
            int loadedScore, loadedLives, colorMode;

            stateStream >> dummy >> loadedScore >> dummy >> loadedLives >> dummy >> colorMode;

            score = loadedScore;                    // Set the score
            marioCharacter->setLife(loadedLives);  // Set Mario's lives
            if (colorMode) utilities::colorSwitch = true; // Set the color mode
        }
    }
}

// Load expected results from .results file
void loadGame::loadResultsFromFile() {
    expectedResults.clear();

    int iteration;
    std::string statusStr;

    while (resultFile >> iteration >> statusStr) {
        status parsedStatus = parseStatus(statusStr);
        addResult(iteration, parsedStatus); 
    }

    if (expectedResults.empty()) {
        std::cerr << "Warning: No valid results loaded from file.\n";
    }
}

// Validate results during silent mode
void loadGame::validateResults() {
    bool testPassed = true;

    if (results.size() != expectedResults.size()) {
        std::cerr << "Error: Mismatch in result sizes. Expected: " << expectedResults.size()
            << ", Got: " << results.size() << "\n";
        testPassed = false;
    }

    if(expectedResults.size() > results.size()) {
        std::cout << "Warning: Fewer results than expected. Expected: " << expectedResults.size()
			<< ", Got: " << results.size() << "\n";
        std::cout << "Skipping test" << std::endl;
        return;
	}

    // Iterate and compare both vectors
    for (size_t i = 0; i < expectedResults.size(); ++i) {
        // Check for size mismatch first
       
        const auto& [expectedIteration, expectedStatus] = expectedResults[i];
        const auto& [actualIteration, actualStatus] = results[i];
        // Ensure there is a corresponding result at the same position
       
        // Check iteration match
        if (expectedIteration != actualIteration) {
            std::cerr << "Error: Iteration mismatch. Expected: " << expectedIteration
                << ", Got: " << actualIteration << "\n";
            testPassed = false;
        }

        // Check status match
        if (expectedStatus != actualStatus) {
            std::cerr << "Error: Status mismatch at iteration " << actualIteration
                << ". Expected: " << statusToString(expectedStatus)
                << ", Got: " << statusToString(actualStatus) << "\n";
            testPassed = false;
        }
    }

    if (testPassed) {
        std::cout << "All results match expected outcomes.\n";
    }
}

// Convert status enum to string
std::string loadGame::statusToString(baseGame::status s) {
    switch (s) {
    case baseGame::status::GameOver: return "GameOver";
    case baseGame::status::Win: return "Win";
    case baseGame::status::HeartCollected: return "HeartCollected";
    case baseGame::status::BarrelHit: return "BarrelHit";
    case baseGame::status::GhostHit: return "GhostHit";
    case baseGame::status::ExplosionHit: return "ExplosionHit";
    case baseGame::status::FallDamage: return "FallDamage";
    case baseGame::status::HammerHit: return "HammerHit";
    default: return "Unknown";
    }
}

// Close all open files
void loadGame::closeFiles()
{
    if (stepFile.is_open()) stepFile.close();
    if (resultFile.is_open()) resultFile.close();
}

// Set the game state for load mode
void loadGame::setGame() {
    if (!silentMode) baseGame::setGame();
    else {
        system("cls"); // Clear the screen
        results.clear(); // Clear results from previous games
        ghosts.clear();      // Clear all ghosts
        initializeGhosts();  // Initialize ghosts for the current stage
        barrelsList.clear(); // Clear all barrels
        iteration = 0;       //Reset the game loop
        gameboard.resetBoard(); // Reset the board 
        int marioStartX, marioStartY;
        gameboard.getStartPos(marioStartX, marioStartY); // Get Mario's starting position

        marioCharacter->setBoard(gameboard); // Set the board for Mario
        marioCharacter->resetPosition(marioStartX, marioStartY); // Reset Mario's position
        setHammerPos(); // Set the hammer position
        if (marioHasHammer) {
            marioCharacter->marioWithHammer();
            gameboard.setChar(hammerX, hammerY, utilities::SPACE);
            useHammer = false;
            hammerX = hammerY = 0;
        }

        for (auto it = ghosts.begin(); it != ghosts.end(); ++it) {
            (*it)->resetPosition((*it)->getX(), (*it)->getY()); // Reset each ghost's position
        }
    }

}

// Handle playing for the current board
void loadGame::handleLoadPlaying() {
    int stepIteration;
    char key;

    int lastIteration = expectedResults.back().first; // Get the last iteration from the expected results


    while (!stepFile.eof()) {
        // Read the next step from the file
        stepFile >> stepIteration >> key;

        // Check if the iteration matches the expected iteration
        while (iteration < stepIteration) {
            updateGame();
        }

        if (iteration == stepIteration) {
            marioCharacter->keyPressed(key);
            updateGame();
        }

    }
    // Check if the game completed all steps
    while (iteration < lastIteration) {
        updateGame();
    }
        

    system("cls");  
    std::cout << "Game completed all steps and iterations.\n";
    if(silentMode) validateResults(); // Compare results in silent mode

    Sleep(3000);
}

// Parse the status string to status enum
baseGame::status loadGame::parseStatus(const std::string& statusStr) {  
    if (statusStr == "GameOver") {
        return baseGame::status::GameOver;
    }
    else if (statusStr == "Win") {
        return baseGame::status::Win;
    }
    else if (statusStr == "HeartCollected") {
        return baseGame::status::HeartCollected;
    }
    else if (statusStr == "BarrelHit") {
        return baseGame::status::BarrelHit;
    }
    else if (statusStr == "GhostHit") {
        return baseGame::status::GhostHit;
    }
    else if (statusStr == "ExplosionHit") {
        return baseGame::status::ExplosionHit;
    }
    else if (statusStr == "FallDamage") {
        return baseGame::status::FallDamage;
    }
    else if (statusStr == "HammerHit") {
        return baseGame::status::HammerHit;
    }
    else {
        return baseGame::status::UNKONWN; // Unknown status 
    }
}

// Add result to the expected results map
void loadGame::addResult(int iteration, status eventStatus) {
    expectedResults.emplace_back(iteration, eventStatus);
}
