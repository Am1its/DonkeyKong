#include <iostream>
#include <Windows.h>

#include "utilities.h"
#include "ghost.h"
#include "board.h"
#include "gameObject.h"
 



void ghost::erase()
{
  
    char currentChar = pBoard->getChar(x, y);
    draw(currentChar);
}

void ghost::move() {

    if (pBoard == nullptr) {
        // Handle the error, log it, or initialize pBoard
        return;
    }
   


    // Check if the ghost starts in the air
    char currentChar = pBoard->getChar(x, y);
    char downFloor = pBoard->getChar(x, y + 1);
    char down2Floor = pBoard->getChar(x, y + 2);
    if (!utilities::isFloor(downFloor)) { // If there's no floor below
        dir_y = 1; // Start falling
    }
    else {
        dir_y = 0; // Stop falling
    }
   

    // Randomly decide whether to change direction
    if (rand() % 100 < 5) { // 5% chance
        changeDirXGhost(); // Change direction
    }

    // Calculate new position
    int newX = x + dir_x;
    int newY = y + dir_y;
    // Handle edge cases: boundaries and falling off the floor  
    char nextFloor = pBoard->getChar(newX, y + 1); // Floor at the new position 
    char nextStep = pBoard->getChar(newX, newY); // Character at the new position
    if (newX < 0 || newX >= utilities::GAME_WIDTH || nextFloor == utilities::SPACE || nextStep == utilities::BORDER) { // Out of bounds or no floor
        changeDirXGhost(); // Reverse direction
        newX = x + dir_x; // Update new position after reversing direction
    }

    

    // Update position
    x = newX;
    y = newY;
}
// Reset Mario's position
void ghost::resetPosition(int startX, int startY) {
    // Erase the current Ghost position
    if (pBoard) {
        pBoard->setChar(x, y, utilities::SPACE); // Clear the old position on the board
    }

    // Set new position
    x = startX;
    y = startY;
}
