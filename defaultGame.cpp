#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>
#include <random>
#include "mario.h"
#include "defaultGame.h"
#include "baseGame.h"
#include "barrels.h"
#include "ghost.h"
#include <cstdlib>   // For srand() and rand()
#include <ctime>     // For time()



// Constructor
defaultGame::defaultGame(bool saveMode)
    : baseGame(), saveMode(saveMode),seed(0) {
	
}

// Destructor
defaultGame::~defaultGame() {
	closeFiles(); // Close all open files
}

// Run the game loop
void defaultGame::run() {
    while (!exitGame) {
        switch (currentState) {
        case GameState::MainMenu:
            handleMainMenu();
            break;
        case GameState::Playing:
            handlePlaying();
            break;
        case GameState::Paused:
            handlePaused();
            break;
        case GameState::GameOver:
            handleGameOver();
            break;
        case GameState::Winner:
            handleWinner();
            break;
        default:
            break;
        }
    }
}

// State handlers   
void defaultGame::handleMainMenu() {
    utilities::showMenu();
    loadScreenFiles(); // Load valid screen files (starting with "dkong_")

    while (!screenFiles.empty()) {
        char choice = _getch(); // Get user input
        if (choice == '1') { // Play all boards starting from the first
            currentStageIndex = 0; // Start from the first board
            for (int i = 0; i < screenFiles.size(); ++i) {
                gameboard.loadBoardFromFile(screenFiles[i]); // Try to load the board

                // If invalid board, show error and skip it
                if (!gameboard.validateBoard()) {
                    utilities::showErrorScreen("Invalid board detected in file '" + screenFiles[i] + "'. Skipping...");
                    Sleep(1000);
                    continue; // Skip invalid board
                }
                // If valid, start the game
                currentState = GameState::Playing;
				currentStageIndex = i; // Set the current stage index 
                setGame(); // Initialize game state
                return;    // Exit loop and start game
            }

            // If no valid boards are found, show error and return to menu
            utilities::showErrorScreen("No valid boards found. Returning to Main Menu.");
            break;
        }
        else if (choice == '3') { // Select a specific stage
            checkScreenFiles();
            utilities::showStageSelectionMenu(screenFiles);

            while (true) {
                char stageChoice = _getch(); // Get user input
                if (stageChoice == '8') { // Return to main menu
                    return; // Exit stage selection
                }

                int index = stageChoice - '1'; // Convert input ('1', '2', ...) to 0-based index
                if (index >= 0 && index < static_cast<int>(screenFiles.size())) {
                    currentStageIndex = index; // Start from the selected stage
                    for (int i = index; i < screenFiles.size(); ++i) {
                        gameboard.loadBoardFromFile(screenFiles[i]); // Try to load the selected board

                        // If invalid board, show error and skip it
                        if (!gameboard.validateBoard()) {
                            utilities::showErrorScreen("Invalid board detected in file '" + screenFiles[i] + "'. Skipping...");
                            continue; // Skip invalid board
                        }

                        // If valid, start the game
                        currentState = GameState::Playing;
                        currentStageIndex = i;
                        setGame();
                        return; // Exit loop and start playing
                    }

                    // If no valid boards are found after the selected index
                    utilities::showErrorScreen("No valid boards found after stage selection. Returning to Main Menu.");
                    return;
                }
                else {
                    drawInLocation(2, 22, "Invalid selection! Please select a valid stage or press '8' to return.");
                }
            }
        }
        else if (choice == '7') { // Toggle color mode
            utilities::colorSwitch = !utilities::colorSwitch;
            handleMainMenu();
            break; // Exit the loop
        }
        else if (choice == '8') { // Show instructions
            utilities::presentInstructions();
            while (true) {
                choice = _getch();
                if (choice == '8') {
                    handleMainMenu();
                    break;
                }
            }
            break; // Exit the loop
        }
        else if (choice == '9') { // Exit game
            exitGame = true;
            system("cls");  
            break; // Exit the loop
        }
        // If the input is invalid, do nothing and continue the loop
    }
}

void defaultGame::handlePlaying() {
    if (_kbhit()) {
        char key = _getch();

        if (key == utilities::ESC) {
            currentState = GameState::Paused;
        }
        else {
            marioCharacter->keyPressed(key);
            if (saveMode) saveStepsToFile(iteration, key); // Log key press
        }
    }
    updateGame();

}

void defaultGame::handlePaused() {
    int legendX, legendY;
    gameboard.getLegend(legendX, legendY);
    //Draw Mario and Barrels
    marioCharacter->draw();
    for (auto& barrel : barrelsList)
        barrel->draw();
    if (utilities::colorSwitch) utilities::setColor(12); //Red
    drawInLocation(legendX, legendY + 3, "Game Paused");
    utilities::setColor(7);
    while (true) {
        if (_kbhit()) {
            char key = _getch();
            if (key == utilities::ESC) {
                drawInLocation(legendX, legendY + 3, "             ");
                currentState = GameState::Playing;
                break;
            }
            else if (key == '8') {
                currentStageIndex = 0; // Reset the stage index for a new game
                gameboard.clearGhostPosition();
                gameboard.clearGhost_UPPosition();
                resetToNewGame();
                saveResultsToFile(); // Save results to file 
                closeFiles(); // Close all open files
                currentState = GameState::MainMenu;
                break;
            }
        }
    }

}

void defaultGame::handleGameOver() {
	if (saveMode) saveResultsToFile(); // Save results to file
    barrelsList.clear(); // Clear all barrels
    ghosts.clear();      // Clear all ghosts
    marioCharacter->marioWithoutHammer(); // Reset Mario's hammer state
    if (marioCharacter->getMarioUseHeart())// reset to false
        marioCharacter->setMarioUseHeart();
    gameboard.clearGhostPosition(); // Clear ghost positions from the board
    gameboard.clearGhost_UPPosition(); // Clear ghost_UP positions from the board
    setmarioHasHammer(false); // Reset Mario's hammer state 
    // Display Game Over screen
    utilities::gameOver();

    // Wait for the user to press '8' to return to the main menu
    while (true) {
        if (_kbhit()) {
            char key = _getch();
            if (key == '8') {
                resetToNewGame(); // Reset to a new game when returning to the main menu
                break;
            }
        }
    }
}

void defaultGame::handleWinner() {
    if (saveMode) saveResultsToFile(); // Save results to file
    system("cls"); // Clear the screen
    setmarioHasHammer(false); // Reset Mario's hammer state 
    // Display the winner screen
    utilities::displayWinnerScreen(score);

    // Wait for the user to press '8' to return to the main menu
    char key;
    while (true) {
        key = _getch();
        if (key == '8') {
            resetToNewGame(); // Reset to a new game when returning to the main menu
            break;
        }
    }
}

// Initialize game state
void defaultGame::setGame(){
	
    if (saveMode) {
        if (stageWinner) {
            stageWinner = false;
            saveResultsToFile(); // Save results to file 
			closeFiles(); // Close all open files
        }
        createSaveFilesForCurrentScreen(); // Create save files for the current screen
		results.clear(); // Clear previous stage's results before starting a new game
    }
    
    baseGame::setGame(); // Initialize game state using base class logic
}



//For Save Methods

// Generate a random seed for reproducibility
void defaultGame::generateSeed() {
	seed = static_cast<long>(std::chrono::system_clock::now().time_since_epoch().count());
	std::srand(seed);
}

// save the current iteration and key press
void defaultGame::saveStepsToFile(int iteration, char key) {
    if (stepFile.is_open()) {
        if (validateKey(key))
        {
            stepFile << iteration << " " << key << std::endl;
            stepFile.flush(); // Ensure data is written immediately
        }
    }
        
    
}

// Save the seed for reproducibility
void defaultGame::saveSeedToFile() {
    generateSeed(); // Generate a random seed for reproducibility
    if (stepFile.is_open()) {
        stepFile << "SEED " << seed << std::endl;
        stepFile << "SCORE " << score << " LIVES " << marioCharacter->getLife() << " COLOR " << (int)utilities::colorSwitch << std::endl;  // Save game state
        
    }
}

// Save all results from resultStats to file
void defaultGame::saveResultsToFile() {

    if (resultFile.is_open()) {
        for (const auto& [iteration, eventStatus] : results) {
            resultFile << iteration << " " << statusToString(eventStatus);
            if (eventStatus == status::Win || eventStatus == status::GameOver) {
                resultFile << " SCORE " << score << std::endl;
            }
            else {
                resultFile << std::endl;
            }
        }
    }
}
   
    

// Create save files for the current screen
void defaultGame::createSaveFilesForCurrentScreen() {
    closeFiles(); // Close any open files before creating new ones
    if (saveMode) {
        // Get the current screen file name
        const std::string& currentScreen = screenFiles[currentStageIndex];

        // Generate .steps and .results file names based on the current screen file
        stepsFileName = currentScreen.substr(0, currentScreen.find_last_of('.')) + ".steps";
        resultsFileName = currentScreen.substr(0, currentScreen.find_last_of('.')) + ".result";

        // Open the .steps file for writing (overwrite if it already exists)
        stepFile.open(stepsFileName, std::ios::out | std::ios::trunc);
        if (!stepFile.is_open()) {
            throw std::runtime_error("Could not open steps file: " + stepsFileName);
        }
		saveSeedToFile(); // Save the seed for reproducibility 

        // Open the .results file for writing (overwrite if it already exists)
        resultFile.open(resultsFileName, std::ios::out | std::ios::trunc);
        if (!resultFile.is_open()) {
            throw std::runtime_error("Could not open results file: " + resultsFileName);
        }

    }
}
 
// Validate the key press
bool defaultGame::validateKey(char key) const {
    // Get Mario's valid keys from the static array in the mario class
    static constexpr char validKeys[] = { 'a', 'w', 'd', 'x', 's', 'p' };
    static constexpr size_t numKeys = sizeof(validKeys) / sizeof(validKeys[0]);

    // Check if the key matches any of the valid keys (case-insensitive)
    for (size_t i = 0; i < numKeys; ++i) {
        if (std::tolower(key) == validKeys[i]) {
            return true;
        }
    }
    return false; // Key is not valid
}

//Close all open files
void defaultGame::closeFiles() {
    if (saveMode) {
        if (stepFile.is_open()) stepFile.close();
        if (resultFile.is_open()) resultFile.close();
    }
}

// Helper function to convert status enum to string
std::string defaultGame::statusToString(baseGame::status eventStatus) const {
    switch (eventStatus) {
    case baseGame::status::BarrelHit:
        return "BarrelHit";
    case baseGame::status::GhostHit:
        return "GhostHit";
    case baseGame::status::ExplosionHit:
        return "ExplosionHit";
    case baseGame::status::FallDamage:
        return "FallDamage";
    case baseGame::status::GameOver:
        return "GameOver";
    case baseGame::status::Win:
        return "Win";
	case baseGame::status::HammerHit:
		return "HammerHit";
	case baseGame::status::HeartCollected:
		return "HeartCollected";
    default:
        return "Unknown"; // Fallback for unexpected values
    }
}




