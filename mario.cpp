#include <iostream>
#include <Windows.h>
#include <conio.h>

#include "mario.h"
#include "utilities.h"
#include "baseGame.h"

// Copy constructor
mario::mario(const mario& other)
    : gameObject(other),
    fallCounter(other.fallCounter),
    isJumping(other.isJumping),
    jumpStep(other.jumpStep),
    jumpHeight(other.jumpHeight),
    onLadder(other.onLadder),
    lives(other.lives),
    marioHasHammer(other.marioHasHammer),
    pGame(other.pGame),   // Copy pointer value (no reallocation)
    pBoard(other.pBoard) {
} // Copy pointer value (no reallocation)


// Assignment operator
mario& mario::operator=(const mario& other) {
    if (this == &other) return *this; // Handle self-assignment

    x = other.x;
    y = other.y;
    dir_x = other.dir_x;
    dir_y = other.dir_y;
    objectChar = other.objectChar;

    fallCounter = other.fallCounter;
    isJumping = other.isJumping;
    jumpStep = other.jumpStep;
    jumpHeight = other.jumpHeight;

    onLadder = other.onLadder;
    lives = other.lives;

    marioHasHammer = other.marioHasHammer;

    pGame = other.pGame;   // Copy pointer value (no reallocation)
    pBoard = other.pBoard; // Copy pointer value (no reallocation)

    return *this;
}


void mario::draw(char c) const {
    if (utilities::colorSwitch) { // Check if colorSwitch is enabled
        if (c == utilities::LADDER) {
            utilities::setColor(6); // Yellow for ladders
        }
        else if (c == utilities::DONKY) {
            utilities::setColor(8); // Gray for Donkey Kong
        }
        else if (utilities::isFloor(c)) {
            utilities::setColor(7); // Default color for floors
        }
        else if (c == utilities::PAULINA) {
            utilities::setColor(14); // Pauline's color
        }
        else if (c == utilities::HEART) {
            utilities::setColor(11); // Heart's color
        }
        else if (c==utilities::HAMMER) {
            utilities::setColor(11); // Cyan for Mario with a hammer
        }
        else {
            utilities::setColor(9); // Default blue for Mario without a hammer
        }
    }

    utilities::gotoxy(x, y);
    std::cout << c;

    utilities::setColor(7); // Reset to default color after drawing
}

void mario::erase() {
    char currentChar = pBoard->getChar(x, y);

    // If Mario just picked up a hammer, clear it
    if (currentChar == utilities::HAMMER) {
        draw(utilities::SPACE); // Clear the hammer from the board state    
    }
    else  if (currentChar == utilities::HEART) {
        draw(utilities::SPACE); // Clear the hammer from the board state    
    }
    else {
        draw(currentChar); // Restore the underlying character (e.g., floor, ladder)
    }
}

// Move Mario based on the current direction
void mario::move() {
    // Handle jumping
    if (isJumping) {
        handleJumping();
        return; // Skip normal movement during jump
    }

    // Handle picking up the hammer
    handleItemPickup();

    // Handle normal movement (left/right)
    handleMovement();

    // Handle falling logic
    handleFalling();

    // Handle ladder transitions
    handleLadderTransitions();
}

void mario::handleFalling() {
    int legendX, legendY;
    pBoard->getLegend(legendX, legendY);
    bool currentlyOnFloor = utilities::isFloor(pBoard->getChar(x, y + 1)); // Check if Mario is on a floor

    if (!currentlyOnFloor && !onLadder && !isJumping) {
        y++; // Mario falls
        fallCounter++; // Count fall duration
    }
    else if (currentlyOnFloor) {
        // Check if the fall was too long
        if (fallCounter > utilities::LONGFALL) {
            if (!pGame->isSilentMode()) {
                // Print "Fall Damage" and display Mario as 'X'
                if (utilities::colorSwitch) utilities::setColor(12); // Red color for damage
                utilities::gotoxy(x, y);
                std::cout << 'X'; // Display Mario as 'X'
                utilities::gotoxy(legendX, legendY + 3);
                std::cout << "Fall Damage";
                utilities::setColor(7); // Reset color
                Sleep(500); // Pause briefly to show the message
            }
            pGame->setMarioFallDammage();
            loseLife(); // Lose a life and reset Mario
        }

        fallCounter = 0; // Reset fall counter when Mario lands on a floor or platform
    }
}

void mario::handleLadderTransitions() {
    bool currentlyOnFloor = utilities::isFloor(pBoard->getChar(x, y + 1));

    if (onLadder && dir_y == -1 && !currentlyOnFloor) { // Descend from a ladder onto a floor
        if (utilities::isFloor(pBoard->getChar(x, y - 1))) {
            y--; // Move Mario to the floor
            return;
        }
    }

    if ((dir_y == 1) && currentlyOnFloor && (pBoard->getChar(x, y + 2) == utilities::LADDER)) { // Descend from floor onto ladder
        y += dir_y + 1;
        onLadder = true;
        return;
    }

    if (onLadder && (pBoard->getChar(x, y + dir_y) != utilities::LADDER)) { // Stop climbing when no ladder exists
        dir_y = 0;
        onLadder = false;
    }
}

void mario::handleMovement() {
    int newX = x + dir_x;
    int newY = y + dir_y;
    char nextChar = pBoard->getChar(newX, newY);

    if (newX >= 0 && newX < utilities::GAME_WIDTH && newY >= 0 && newY < utilities::GAME_HEIGHT && nextChar != utilities::BORDER) {
        x = newX;
		y = newY;
    }
}

void mario::handleItemPickup() {
	int legendX, legendY;
	pBoard->getLegend(legendX, legendY);
    char currentChar = pBoard->getChar(x, y);

    if (currentChar == utilities::HAMMER) {
        marioHasHammer = true; // Set hammer state to active
        marioWithHammer();     // Change Mario's appearance to indicate he has a hammer
        pBoard->setChar(x, y, utilities::SPACE); // Remove the hammer from the board
        pGame->setmarioHasHammer(true);// initiliaz that mario has hammer in the game control 
    }
    else if (currentChar == utilities::HEART) { // Check for heart
        if (lives < 3) {
            pBoard->setChar(x, y, utilities::SPACE); // Remove the hammer from the board
            lives++; // Increase Mario's lives by 1 (up to a maximum of 3)
            setMarioUseHeart();
            utilities::gotoxy(legendX, legendY+3);
            if(!pGame->isSilentMode()) std::cout << "Heart Collected! ";
            Sleep(500); // Display message briefly
            utilities::gotoxy(legendX, legendY+3);
            std::cout << "                     "; // Clear message
			pGame->setHeartCollected(); // Set the heart collected flag
        }
    }
}

void mario::handleJumping() {
    if (jumpStep < jumpHeight && !utilities::isFloor(pBoard->getChar(x, y - 1))) {
        y--; // Move up
        if ((x + dir_x) > 0 && (x + dir_x) < utilities::GAME_WIDTH)
            x += dir_x; // Keep horizontal movement
        jumpStep++; // Increment jump step
    }
    else {
        isJumping = false; // End of jump only when reaching max height or hitting something above
        jumpStep = 0;
        dir_y = 0; // Reset vertical direction to allow falling
    }
}

// Handle key press events
void mario::keyPressed(char key) {
    for (size_t i = 0; i < numKeys; i++) {
        if (std::tolower(key) == keys[i]) {
            changeDirection(keys[i]);
            return;
        }
    }
}

void mario::changeDirection(char key)  {
    switch (key) {
    case 'w': // UP
        if (pBoard->getChar(x, y) == utilities::LADDER) {
            dir_y = -1; // Move up the ladder
            dir_x = 0;
            onLadder = true;
        }
        else if (!isJumping && !onLadder && utilities::isFloor(pBoard->getChar(x, y + 1))) {
            isJumping = true; // Start jump
            fallCounter = 0;
            dir_y = -1;
        }
        break;

    case 'x': // DOWN
        if (onLadder || (utilities::isFloor(pBoard->getChar(x, y + 1)) && pBoard->getChar(x, y + 2) == utilities::LADDER)) {
            dir_y = 1; // Move down the ladder
            dir_x = 0;
            onLadder = true;
        }
        else {
            dir_y = 0; // Prevent downward move through floors
        }
        break;

    case 'a': // LEFT
        dir_x = -1;
        dir_y = 0;
        onLadder = false; // Stop climbing when moving left
        break;

    case 'd': // RIGHT
        dir_x = 1;
        dir_y = 0;
        onLadder = false; // Stop climbing when moving right
        break;

    case 's': // STAY
        dir_x = 0;
        dir_y = 0;
        break;

    case 'p':
        setPressP();
        break;
    }
}

void mario::resetPosition(int startX, int startY) {
    // Erase Mario's current position from the board
    if (pBoard) {
        pBoard->setChar(x, y, pBoard->getChar(x, y)); // Restore the underlying character

    }

    // Set new position
    x = startX;
    y = startY;

    // Reset other movement-related states
    dir_x = 0;
    dir_y = 0;
    isJumping = false;
    jumpStep = 0;
    onLadder = false;
    fallCounter = 0;
    marioHasHammer = false;
    objectChar = utilities::MARIO; // Reset Mario's appearance
}

// Decrease the number of lives by one and reset Mario's position
void mario::loseLife() {
    lives--; // Decrease the number of lives
    pGame->addScore(-50); // Deduct 50 points from the score
    pGame->resetGame(); // Reset the game state
}


