#include <iostream>
#include <Windows.h>

#include "utilities.h"
#include "barrels.h"
#include "gameObject.h"
 


void barrels::erase()
{
    char currentChar = pBoard->getChar(x, y);
    draw(currentChar);
}

void barrels::move() {  
    // Update floor type based on current position
    if (y + 1 <utilities::GAME_HEIGHT) {
        floorType = pBoard->getChar(x, y + 1);
    }
    // Check if the barrel should fall or explode
    if (floorType == ' ') {
        dir_y = 1;
        countBarrelFall++;
    }
    if (countBarrelFall > 7) { // Trigger explosion after long fall
        // Check for explosion state
        if (utilities::isFloor(pBoard->getChar(x, y + 1))) {
            explode = true;
        }
    }
    if (utilities::isFloor(pBoard->getChar(x, y + 1))) { // if fall and small than 7 and get floor 
        dir_y = 0; // Reset vertical movement if on a floor
        countBarrelFall = 0; // Reset fall counter when landing
    }

    // Determine direction based on floor type
    switch (floorType) {
    case '<':
        dir_x = -1; // Move left
        break;
    case '>':
        dir_x = 1; // Move right
        break;
    case '=':
        break; // Continue moving in the same direction
    }

    // Calculate new position
    int newX = x + dir_x;
    int newY = y + dir_y;

    // Check bounds for new position
    if (newX < 0 || newX >= utilities::GAME_WIDTH || newY < 0 || newY >= utilities::GAME_HEIGHT || pBoard->getChar(newX, newY) == utilities::BORDER) {
        outOfBoard = true;
        return;
    }

    // Check if the new position is a border ('Q')
    if (pBoard->getChar(newX, newY) == utilities::BORDER) {
        // Do not move if the new position is a border
        return;
    }

    // Update position and draw the barrel
    erase();
    x = newX;
    y = newY;
}
