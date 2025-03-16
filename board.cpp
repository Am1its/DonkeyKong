#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <locale>
#include <codecvt>
#include <fstream>
#include <cstring>
#include <string>
#include <sstream>

#include "mario.h"
#include "board.h"
#include "utilities.h"
#include "ghost.h"



using namespace std;

void board::resetBoard() {
    for (int i = 0; i < MAX_Y; ++i) {
        for (int j = 0; j < MAX_X; ++j) {
            if (originalBoard[i][j] == utilities::MARIO) {
                currentBoard[i][j] = utilities::SPACE; // Replace Mario's initial position with a space
            }
            else {
                currentBoard[i][j] = originalBoard[i][j]; // Copy other characters as-is
            }
        }
    }
}

void board::printBoard() const {
    system("cls"); // Clear the screen before printing

    for (int y = 0; y < MAX_Y; ++y) {
        for (int x = 0; x < MAX_X; ++x) {
            char c = currentBoard[y][x];
            if (utilities::colorSwitch) {
                if (c == utilities::PAULINA) utilities::setColor(14);         // Pauline
                else if (c == utilities::DONKY) utilities::setColor(8);      // Donkey Kong
                else if (c == utilities::LADDER) utilities::setColor(6);    // Ladder
                else if (c == utilities::HAMMER) utilities::setColor(11);  //Hammer
                else if (utilities::isFloor(c)) utilities::setColor(7);   // Floor
                else utilities::setColor(7);                  // Default
            }
            utilities::gotoxy(x, y);
            std::cout << c;
        }
    }

    utilities::setColor(7); // Reset to default color after printing
}

bool board::loadBoardFromFile(const std::string& filename) {
    std::ifstream screen_file(filename);

    if (!screen_file.is_open()) {
        std::cerr << "Error: Unable to open file '" << filename << "'." << std::endl;
        return false;
    }

    ghostPositions.clear();     // Clear previous ghost positions
    ghost_UPPositions.clear(); // Clear previous ghost positions
    resetBoard();             // Clear the board before loading

    int curr_row = 0, curr_col = 0;
    char c;

    while (screen_file.get(c)) {
        if (curr_row >= MAX_Y) 
        {// If more than 25 rows are encountered, mark as invalid
            std::cerr << "Error: Board height exceeds 25 rows. Invalid board." << std::endl;
            screen_file.close();
            resetBoard(); // Reset to an empty state
            return false;
        }
        std::stringstream expandedLine; 
        if (c == '\n') { 
            // Fill remaining spaces in the current row if it's shorter than 80 columns
            while (curr_col <= MAX_X) { 
                originalBoard[curr_row][curr_col++] = utilities::SPACE; 
            } 
            curr_row++; 
            curr_col = 0; // Move to the next row 
		}   
        else if (curr_col <= MAX_X) {
            // Process and store each character in the board
            originalBoard[curr_row][curr_col] = c;

            // Handle specific character roles
            switch (c) {
            case utilities::MARIO:
                startX = curr_col;
                startY = curr_row;
                break;
            case utilities::PAULINA:
                endX = curr_col;
                endY = curr_row;
                break;
            case utilities::DONKY:
                donkeyX = curr_col;
                donkeyY = curr_row;
                break;
            case utilities::GHOST:
                ghostPositions.emplace_back(curr_col, curr_row);
                break;
            case utilities::GHOST_UP: 
                ghost_UPPositions.emplace_back(curr_col, curr_row);
                break;
            case utilities::HAMMER:
                hammerX = curr_col;
                hammerY = curr_row;
                break;
            case utilities::LEGEND:
                legendX = curr_col;
                legendY = curr_row;
                break;
            default:
                break; // Handle other characters normally
            }
            curr_col++;
        }
    }
    // Handle cases where fewer than 25 rows are present
    if (curr_row + 1 < MAX_Y) {
        std::cerr << "Error: Board height is less than 25 rows. Invalid board." << std::endl;
        screen_file.close();
        return false;
    }

    screen_file.close();

    // Validate the loaded board
    if (!validateBoard()) {
        std::cerr << "Error: Board validation failed. Invalid board." << std::endl;
        return false;
    }

    return true; // Successfully loaded and validated the board
}
 
bool board::validateBoard() const {
    int marioCount = 0;
    int paulineCount = 0;
    int donkeyCount = 0;
    int legendCount = 0;

    bool hasFloor = false;
    bool marioOnValidPosition = false;

    for (int y = 0; y < MAX_Y; ++y) {
        for (int x = 0; x < MAX_X; ++x) {
            char c = originalBoard[y][x];

            if (c == utilities::MARIO) {
                marioCount++;
                if (checkMarioValidPos(x, y)) { // Check if Mario is on a valid position
                    marioOnValidPosition = true;
                }
            }
            else if (c == utilities::PAULINA) paulineCount++;
            else if (c == utilities::DONKY) donkeyCount++;
            else if (c == utilities::LEGEND) legendCount++;
            else if (utilities::isFloor(c)) hasFloor = true;
        }
    }
    // Validate required characters
    if (marioCount != 1 || paulineCount != 1 || donkeyCount != 1 || legendCount != 1) {
        return false;
    }

    // Validate floors and Mario's position
    if (!hasFloor) {
        return false;
    }
    if (!marioOnValidPosition) {
        return false;
    }

    return true; // Board is valid
}

bool board::checkMarioValidPos(int x, int y) const {
    // Iterate over the next 4 rows below Mario's position
    for (int i = 1; i <= 4; ++i) {
        int newY = y + i;
        if (newY >= MAX_Y) break; // Prevent accessing out of bounds

        // Check if there's a floor or ladder within the next 4 rows below Mario
        if (utilities::isFloor(originalBoard[newY][x]) || originalBoard[newY][x] == utilities::LADDER) {
            return true; // Valid position found
        }
    }

    return false; // No valid position found within the next 4 rows
}

void board::printLegendInfo(int lives, int score, bool hasHammer) const {
    // Calculate positions for printing below 'L'
    int line1Y = legendY;
    int line2Y = legendY + 1;
    int line3Y = legendY + 2;
    
    // Ensure we don't go out of bounds vertically
    if (line3Y >= MAX_Y) return;

    // Print Mario lives
    if (utilities::colorSwitch) utilities::setColor(12);
    for (int i = 0; i < lives; ++i) {
        int posX = legendX + (3 * i);
        if (posX + 2 > utilities::GAME_WIDTH) break; // Ensure we don't go out of bounds horizontally
        utilities::gotoxy(posX, line1Y);
        std::cout << "<3";
    }
    if (utilities::colorSwitch) utilities::setColor(14); // Reset to default color

    // Print score on the second line
    utilities::gotoxy(legendX, line2Y); 
    std::cout << "Score: " << score; 

    // Print hammer status on the third line
    if (utilities::colorSwitch) utilities::setColor(11); // Reset to default color
    utilities::gotoxy(legendX, line3Y);
    std::cout << "Hammer:" << (hasHammer ? "Active   " : "Inactive");

    utilities::setColor(7); // Reset to default color
} 

void board::clearGhostPosition() {
	ghostPositions.clear();
}

void board::clearGhost_UPPosition()
{
    ghost_UPPositions.clear(); 
}


