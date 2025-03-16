#include <iostream>
#include <Windows.h>
#include "ghostUpgrade.h"
#include "utilities.h"
#include "ghost.h"
#include "board.h"
#include "gameObject.h"
#include "baseGame.h"

void ghostUpgrade::move() {
    if (pBoard == nullptr) {
        return;
    }

   

    // Check if ghost starts in the air
    char downFloor = pBoard->getChar(x, y + 1);
    if (!utilities::isFloor(downFloor) && !onLadder) {
        dir_y = 1;  // Start falling
    }
    else if(utilities::isFloor(downFloor) && !onLadder) {
        dir_y = 0;  // Stop falling
    }

    handelLedder();

    // If not on a ladder, handle random direction change
    if (!onLadder && dir_x != 0) {
        if (rand() % 100 < 5) {  // 5% chance to change direction
            changeDirXGhost();
        }
    }

    // Calculate new position
    int newX = x + dir_x;
    int newY = y + dir_y;

    // Handle collisions with boundaries or obstacles
    char nextFloor = pBoard->getChar(newX, y + 1);
    char nextStep = pBoard->getChar(newX, newY);

    if (newX < 0 || newX >= utilities::GAME_WIDTH || nextFloor == utilities::SPACE || nextStep == utilities::BORDER) {
        changeDirXGhost();  // Reverse direction if hitting boundaries
    }
    else {
        x = x+dir_x;  // Move horizontally
        y = y+dir_y;  // Move vertically (if applicable)
    } 
}

void ghostUpgrade::handelLedder() {
    if (pBoard == nullptr || pGame == nullptr) {
        return; // Safety check
    }

    int marioX, marioY;
    pGame->getMarioPos(marioX, marioY);  // Get Mario's current position  

    // Check the characters around the ghost
    char currentChar = pBoard->getChar(x, y);
    char aboveChar = pBoard->getChar(x, y - 1);  // Character above
    char belowChar = pBoard->getChar(x, y + 1);  // Character below
    char twoBelowChar = pBoard->getChar(x, y + 2); // Two spaces below

    bool currentlyOnFloor = utilities::isFloor(belowChar);

    // Handle ascending a ladder
    if (!onLadder && (currentChar == utilities::LADDER) && currentlyOnFloor) {
        // Decide whether to climb the ladder based on Mario Position
        if (marioY < y) { // Mario is above the ghost
            if (rand() % 100 < 100) { // 80% chance to climb
                onLadder = true;
                dir_x = 0; // Stop horizontal movement
                dir_y = -1; // Start climbing upward
            }
        }
		
        else if (rand() % 100 < 40) { // 40% chance to climb
            onLadder = true;
            dir_x = 0; // Stop horizontal movement
            dir_y = -1; // Start climbing upward
        }
    }

    // Handle descending from a floor onto a ladder
    if (!onLadder && currentlyOnFloor && twoBelowChar == utilities::LADDER) {
        // Decide whether to descend the ladder
        if(marioY > y) { // Mario is below the ghost
			if (rand() % 100 < 80) { // 80% chance to descend
				onLadder = true;
				dir_x = 0;      // Stop horizontal movement
				dir_y = 1;      // Start moving downward
			}
		}
		else if (rand() % 100 < 40) { // 40% chance to descend
			onLadder = true;
			dir_x = 0;      // Stop horizontal movement
			dir_y = 1;      // Start moving downward
		}
    }

    // If the ghost is already on the ladder
    if (onLadder) { 
        // Climbing up: If there's a floor above, move onto it
        if (dir_y == -1 && utilities::isFloor(aboveChar)) {
            y -= 2;          // Move to the floor above
            dir_y = 0;       // Stop vertical movement
            dir_x = (rand() % 2 == 0) ? -1 : 1;  // Resume horizontal movement
            onLadder = false;  // Exit ladder state
            return;
        }

        // Descending: If there's a floor below, move onto it
        if (dir_y == 1 && utilities::isFloor(belowChar)&& twoBelowChar == utilities::LADDER ) { 
            y += 1;          // Move to the floor below
            dir_y = 1;       // Stop vertical movement
            dir_x = 0;  // Resume horizontal movement
            return;
        }
        if (dir_y == 1 && currentlyOnFloor && twoBelowChar != utilities::LADDER)
        {
            onLadder = false;
			dir_y = 0;
			dir_x = (rand() % 2 == 0) ? -1 : 1;
        }

        
    }  
}




