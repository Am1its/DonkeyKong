#include "baseGame.h"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include "utilities.h"
#include "mario.h"
#include "barrels.h"
#include "ghost.h"
#include "ghostUpgrade.h"

namespace fs = std::filesystem;


// Constructor
baseGame::baseGame() 
    : currentState(GameState::MainMenu),
    marioCharacter(new mario(this)),
    gameboard(),
    iteration(0),
    score(0),
    currentStageIndex(0),
    completedStages(0),
    barrelSpawnCounter(0),
    exitGame(false),
    stageWinner(false),
    marioHasHammer(false),
    useHammer(false),
    heartCollected(false),
    marioFallDammage(false),
    silentMode(false),
    barrelX(0),
    barrelY(0),
    ghostX(0),
    ghostY(0),
    hammerX(0),
    hammerY(0),
    marioX(0),
    marioY(0) {} // Initialize currentStatus

// Destructor
baseGame::~baseGame() {
    delete marioCharacter;
}

// Load screen files from the directory
void baseGame::loadScreenFiles(const std::string& directory) {

    screenFiles.clear();

    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            const std::string filename = entry.path().filename().string();
            if (filename.find("dkong_") == 0 && entry.path().extension() == ".screen") {
                screenFiles.push_back(filename);
            }
        }
    }

    std::sort(screenFiles.begin(), screenFiles.end());
}

//Validate screen files
void baseGame::checkScreenFiles() {
    if (screenFiles.empty()) {
        std::cerr << "No valid 'dkong_' files found in directory." << std::endl;
        exitGame = true;
    }
}

// Initialize game state
void baseGame::setGame() {
    system("cls"); // Clear the screen
	results.clear(); // Clear results from previous games
    ghosts.clear();      // Clear all ghosts
    initializeGhosts();  // Initialize ghosts for the current stage
    barrelsList.clear(); // Clear all barrels
    iteration = 0;       //Reset the game loop

    gameboard.resetBoard(); // Reset the board 
    gameboard.printBoard(); // Print the board
    

    int marioStartX, marioStartY;
	gameboard.getStartPos(marioStartX, marioStartY); // Get Mario's starting position

	marioCharacter->setBoard(gameboard); // Set the board for Mario
	marioCharacter->resetPosition(marioStartX, marioStartY); // Reset Mario's position
    setHammerPos(); // Set the hammer position
    if (marioHasHammer) {   
        marioCharacter->marioWithHammer();
        gameboard.setChar(hammerX, hammerY, utilities::SPACE);
        drawCharInLocation(hammerX, hammerY, utilities::SPACE);
        useHammer = false;
        hammerX = hammerY = 0;
    }

    for (auto it = ghosts.begin(); it != ghosts.end(); ++it) {
        (*it)->resetPosition((*it)->getX(), (*it)->getY()); // Reset each ghost's position
    }
    gameboard.printLegendInfo(marioCharacter->getLife(), score, getmarioHasHammer()); 
}

// Run the game loop
void baseGame::updateGame() {
	if (heartCollected)
	{
		heartCollected = false;
		recordResult(iteration, status::HeartCollected); 
	}
    marioCharacter->draw();
    addScore(10); // Increment score for surviving
    iteration++;

    updateBarrels();
    updateGhosts();

    gameboard.printLegendInfo(marioCharacter->getLife(), score,getmarioHasHammer() );//marioCharacter->isMarioHasHammer()

    // Check if Mario reached Pauline (end of stage)
    if (gameboard.getChar(marioCharacter->getX(), marioCharacter->getY()) == utilities::PAULINA) {
		recordResult(iteration, status::Win);  // Record the stage completion status
        stageWinner = true; 
        utilities::displayRoundWinScreen(currentStageIndex + 1); // Show win screen for the current stage
        completedStages++; // Increment completed stages

        while (++currentStageIndex < static_cast<int>(screenFiles.size())) {
            gameboard.clearGhostPosition(); // Clear ghost positions from the previous board
            gameboard.clearGhost_UPPosition(); // Clear ghost_UP positions from the previous board
            gameboard.loadBoardFromFile(screenFiles[currentStageIndex]); // Load the next board

            if (gameboard.validateBoard()) {

                setGame(); // Reset game state for the new board
                Sleep(1000);
                return;
            }
            else {
                utilities::showErrorScreen("Invalid board detected. Skipping to the next stage...");
            }
        }
        currentState = GameState::Winner; // Handle winner state when no more stages remain
        return;
    }

    Sleep(70);
    marioCharacter->erase();
    marioCharacter->move();
    marioCharacter->resetPressP();
}

// Reset the current stage
void baseGame::resetGame() {
    if (marioFallDammage) {
        recordResult(iteration, status::FallDamage);
        marioFallDammage = false;
    }

    if (marioCharacter->getLife() == 0) {
        currentState = GameState::GameOver;
		recordResult(iteration, status::GameOver);
        if (marioCharacter->getMarioUseHeart())// reset to false
            marioCharacter->setMarioUseHeart();
        marioCharacter->marioWithoutHammer(); // Reset Mario's hammer state
        setmarioHasHammer(false);
        gameboard.clearGhostPosition(); // Clear ghost positions from the previous board
        gameboard.clearGhost_UPPosition(); // Clear ghost_UP positions from the previous board
        return;
    }
    

    barrelsList.clear(); // Clear all barrels
    ghosts.clear(); // Clear all ghosts
    initializeGhosts(); // Reinitialize ghosts for the current stage

    int startX, startY;
    gameboard.getStartPos(startX, startY);
    gameboard.resetBoard(); // Reset the board without affecting hammer state
    if (!isSilentMode()) {
        gameboard.printBoard();
    }

    marioCharacter->setBoard(gameboard);
    marioCharacter->resetPosition(startX, startY); // Reset Mario's position
    for (auto it = ghosts.begin(); it != ghosts.end(); ++it) {
        (*it)->resetPosition((*it)->getX(), (*it)->getY()); // Reset each ghost's position
    }
    //HEART
    if (!marioCharacter->getMarioUseHeart())
    {
        // Spawn a heart if conditions are met
        int heartX = rand() % utilities::GAME_WIDTH; // Random X coordinate
        int heartY = rand() % utilities::GAME_HEIGHT; // Random Y coordinate 
        // Ensure the heart spawns on a valid position (e.g., not on a wall or ladder)
        while (!utilities::isFloor(gameboard.getChar(heartX, heartY + 1))) {
            heartX = rand() % utilities::GAME_WIDTH;
            heartY = rand() % utilities::GAME_HEIGHT;
        }
        gameboard.setChar(heartX, heartY, utilities::HEART); // Use HEART as a character 
        utilities::gotoxy(heartX, heartY);
        if (!isSilentMode()) std::cout << utilities::HEART; // Draw the heart on the screen 
    }
    setmarioHasHammer(false);
    if(!isSilentMode()) gameboard.printLegendInfo(marioCharacter->getLife(), score,getmarioHasHammer() );//marioCharacter->isMarioHasHammer()

}

// Reset the entire game to start a new one
void baseGame::resetToNewGame() {
    currentStageIndex = 0; // Reset to the first stage
    completedStages = 0;   // Reset completed stages
    score = 0;             // Reset the score
    iteration = 0;         //Reset the game loop

    marioCharacter->resetLife(); // Reset Mario's lives to 3
    marioCharacter->marioWithoutHammer(); // Reset Mario's hammer state
    setmarioHasHammer(false); 
    barrelsList.clear();   // Clear all barrels
    ghosts.clear();        // Clear all ghosts
    currentState = GameState::MainMenu; // Return to the main menu
}

// Update all barrels' positions and check collisions
void baseGame::updateBarrels() {
    int legendX, legendY;
    gameboard.getLegend(legendX, legendY);

    // Randomly spawn new barrels if conditions are met
    if (barrelsList.size() < utilities::MAX_BARRELS && rand() % 20 == 0) {
        spawnBarrel();
    }

    // Get Mario's position
    marioX = marioCharacter->getX();
    marioY = marioCharacter->getY();

    // Iterate over the barrels
    for (auto it = barrelsList.begin(); it != barrelsList.end();) {
        auto& barrel = **it;  // Dereference the unique_ptr to get the barrel object

        barrelX = barrel.getX();
        barrelY = barrel.getY();

        // Check for collision with Mario before moving the barrel
        if (marioX == barrelX && marioY == barrelY) {
            if (utilities::colorSwitch) utilities::setColor(12);
            if (!isSilentMode()) {
                drawInLocation(legendX, legendY + 3, "Barrel Hit");
                drawInLocation(barrelX, barrelY, "X");
                Sleep(500);
            }

            recordResult(iteration, status::BarrelHit);
            marioCharacter->loseLife();
            utilities::setColor(7);
            return;
        }

        // Erase the barrel from its current position before moving
        if (!isSilentMode()) {
            barrel.erase();
        }

        // Move the barrel
        barrel.move();

        // Draw the barrel after moving (if not in silent mode)
        if (!isSilentMode()) {
            barrel.draw();
        }

        // Update barrel position after movement
        barrelX = barrel.getX();
        barrelY = barrel.getY();

        // Check for hammer collision (Mario with hammer hitting the barrel)
        if (abs(marioX - barrelX) <= utilities::EXPLOSION_RADIUS && barrelY == marioY && marioCharacter->isMarioHasHammer()) {
            if (marioCharacter->getDirX() != barrel.getDirX() && marioCharacter->getPressP()) {
                if (utilities::colorSwitch) utilities::setColor(12);
                if (!isSilentMode()) {
                    drawInLocation(legendX, legendY + 3, "Hammer Hit");
                    Sleep(200);
                    drawInLocation(legendX, legendY + 3, "           ");  // Clear message
                }

                barrel.erase();
                it = barrelsList.erase(it);  // Remove and delete the barrel
                utilities::setColor(7);
                recordResult(iteration, status::HammerHit);
                score += 20;  // Increment score for hitting a barrel with a hammer
                continue;
            }
        }

        // Check for explosion after movement
        if (barrel.getExplode()) {
            if (abs(marioX - barrelX) <= utilities::EXPLOSION_RADIUS && abs(marioY - barrelY) <= utilities::EXPLOSION_RADIUS) {
                if (utilities::colorSwitch) utilities::setColor(12);
                if (!isSilentMode()) {
                    drawInLocation(legendX, legendY + 3, "Explosion");
                    drawInLocation(marioX, marioY, "X");
                    Sleep(500);
                }

                recordResult(iteration, status::ExplosionHit);
                marioCharacter->loseLife();
                utilities::setColor(7);
                return;
            }

            barrel.erase();
            it = barrelsList.erase(it);  // Remove and delete the barrel
            continue;
        }

        // Check if the barrel is out of bounds
        if (barrel.getOutOfBoard()) {
            barrel.erase();
            it = barrelsList.erase(it);  // Remove and delete the barrel
        }
        else {
            ++it;  // Move to the next barrel
        }
    }
}

// Update all ghosts' positions and check collisions
void baseGame::updateGhosts() {
    int legendX, legendY;
    gameboard.getLegend(legendX, legendY);
    marioX = marioCharacter->getX();
    marioY = marioCharacter->getY();

    for (auto it = ghosts.begin(); it != ghosts.end(); ) {
        ghostX = (*it)->getX();
        ghostY = (*it)->getY();

        int marioDirX = marioCharacter->getDirX();
        int marioDirY = marioCharacter->getDirY();
        int nextStepMarioX = marioX + marioDirX;
        int nextStepMarioY = marioY + marioDirY;

        // Check for collision before moving the ghost
        if (marioX == ghostX && marioY == ghostY) {
            if (utilities::colorSwitch) utilities::setColor(12);
            if (!isSilentMode()) {
                drawInLocation(legendX, legendY + 3, "Ghost Hit");
                drawInLocation(ghostX, ghostY, "X");
                Sleep(500);
            }
            recordResult(iteration, status::GhostHit);

            marioCharacter->loseLife();
            utilities::setColor(7);
            return;
        }

        // Handle collisions with other ghosts
        bool directionChanged = false;
        for (const auto& other : ghosts) {
            if (*it == other) continue; // Skip checking against itself

            int nextStepGhostX = (*it)->getX() + (*it)->getDirX();
            int nextStepGhostY = (*it)->getY() + (*it)->getDirY();
            int nextStepOtherX = other->getX() + other->getDirX();
            int nextStepOtherY = other->getY() + other->getDirY();

            if (nextStepGhostX == nextStepOtherX && nextStepGhostY == nextStepOtherY) {
                (*it)->changeDirXGhost();
                other->changeDirXGhost();
                directionChanged = true;
                break; // Avoid multiple direction changes for the same ghost
            }
        }

        // Move the ghost if direction was not just changed
        if (!directionChanged) {
            
            if (!isSilentMode()) {
                (*it)->erase(); // Erase the ghost from the current position
    
            }
            (*it)->move(); 

            if (!isSilentMode()) (*it)->draw(); 
        }

        // Check for hammer collision with ghost
        ghostX = (*it)->getX();
        ghostY = (*it)->getY();
        if (abs(marioX - ghostX) <= utilities::EXPLOSION_RADIUS && ghostY == marioY && marioCharacter->isMarioHasHammer()) {
            if (marioCharacter->getDirX() != (*it)->getDirX() && marioCharacter->getPressP()) {
                if (utilities::colorSwitch) utilities::setColor(12);
                if (!isSilentMode()) {
                    drawInLocation(legendX, legendY + 3, "Hammer Hit");
                    Sleep(200);
                    drawInLocation(legendX, legendY + 3, "           "); // Clear the message by overwriting with spaces
                }
                (*it)->erase();
                it = ghosts.erase(it); // Remove ghost and adjust iterator
                utilities::setColor(7);
                score += 20; // Increment score for hitting a ghost with a hammer
                recordResult(iteration, status::HammerHit);
                continue; // Skip incrementing the iterator
            }
        }

        // Check for collision after moving the ghost
        if (marioX == ghostX && marioY == ghostY) {
            if (utilities::colorSwitch) utilities::setColor(12);
            if (!isSilentMode()) {
                drawInLocation(legendX, legendY + 3, "Ghost Hit");
                drawInLocation(ghostX, ghostY, "X");
                Sleep(500);
            }
            recordResult(iteration, status::GhostHit);

            marioCharacter->loseLife();
            utilities::setColor(7);
            return;
        }

        ++it; // Increment the iterator
    }
}

// Initialize ghosts for the current stage
void baseGame::initializeGhosts() {
    ghosts.clear(); // Clear any existing ghosts

    const auto& ghostPositions = gameboard.getGhostPositions();
    for (const auto& pos : ghostPositions) {
        int initialDirection = (rand() % 2 == 0) ? 1 : -1; // Randomize direction (-1 or 1)
        ghosts.emplace_back(std::make_unique<ghost>(this,gameboard, pos.first, pos.second, initialDirection, 0, utilities::GHOST));
    }

    const auto& ghost_UPPositions = gameboard.getGhost_UPPositions();
    for (const auto& pos : ghost_UPPositions) {
        int initialDirectionX = (rand() % 2 == 0) ? 1 : -1;
        int initialDirectionY = (rand() % 2 == 0) ? 1 : -1;
        ghosts.emplace_back(std::make_unique<ghostUpgrade>(this,gameboard, pos.first, pos.second, initialDirectionX, initialDirectionY, utilities::GHOST_UP));
    }
}
 
// Spawn a new barrel
void baseGame::spawnBarrel() {
    int startX, startY;
    gameboard.getDonkeyPos(startX, startY);

    int randomValue = rand() % 2;
    int initialDirection = (randomValue == 0) ? 1 : -1;

    if (initialDirection == 1) startX += 2;
    else startX -= 2;

    barrelsList.emplace_back(std::make_unique<barrels>(this, gameboard, startX, startY, initialDirection, 0, utilities::BARREL)); 
}



// Draw a message at a specific location on the screen
void baseGame::drawInLocation(int x, int y, const char* message)
{
    utilities::gotoxy(x, y);
    std::cout << message;
}

// Draw a single character at a specific location on the screen
void baseGame::drawCharInLocation(int x, int y, char c)
{
    utilities::gotoxy(x, y);
    std::cout << c;
}

// Add points to the score
void baseGame::addScore(int points) {
    score += points;
}

// Record a result in the results vector
void baseGame::recordResult(int iteration, status eventStatus) {
    results.emplace_back(iteration, eventStatus);
}

// Get Mario's position
void baseGame::getMarioPos(int& x, int& y) const {
    if (marioCharacter != nullptr) {
        x = marioCharacter->getX();
        y = marioCharacter->getY();
    }
    else {
        x = -1; // Invalid position if Mario doesn't exist
        y = -1;
    }
}

