#pragma once
#include <iostream>
#include <vector>
#include "utilities.h"


class ghost;    

class baseGame;
class board {
   
    static constexpr int MAX_X = utilities::GAME_WIDTH;  // Maximum width of the board
    static constexpr int MAX_Y = utilities::GAME_HEIGHT; // Maximum height of the board

    char originalBoard[MAX_Y][MAX_X + 1] = {}; // Original board loaded from file
    char currentBoard[MAX_Y][MAX_X + 1] = {};  // Current state of the board


    int startX = 0, startY = 0; // Starting position of Mario
    int endX = 0, endY = 0;     // Position of Pauline or goal
	int donkeyX = 0, donkeyY = 0; // Position of Donkey Kong
	int legendX = 0, legendY = 0; // Position of the legend
	int hammerX = 0, hammerY = 0; // Position of the hammer

    std::vector<std::pair<int, int>> ghostPositions; // Vector to store the ghost positions
    std::vector<std::pair<int, int>> ghost_UPPositions; // Vector to store the ghost_UP positions


public:
    
    // Reset the board to its original state
    void resetBoard(); 

    // Print the current state of the board
    void printBoard() const;

    //Load the board from a file
    bool loadBoardFromFile(const std::string& filename);

	// Validate the board for correctness
    bool validateBoard() const;

	// Check if the specified position is a valid position for Mario
    bool checkMarioValidPos(int x, int y) const;

    // Get Mario's starting position
    void getStartPos(int& x, int& y) const { x = startX; y = startY; }

    // Get Pauline's position (end goal)
    void getEndPos(int& x, int& y) const { x = endX; y = endY; }

	//get the DonkeyKong's position
	void getDonkeyPos(int& x, int& y) const { x = donkeyX; y = donkeyY; }

	// Get the hammer's position
	void getHammer(int& x, int& y) const { x = hammerX; y = hammerY; }

	// Get the legend position
	void getLegend(int& x, int& y) const { x = legendX; y = legendY; }

	// Print the legend information
    void printLegendInfo(int lives, int score, bool hasHammer) const; 
    
    // Get the character at the specified position
    char getChar(int x, int y) const {
        if (y < 0 || y >= MAX_Y || x < 0 || x >= MAX_X) {
            return utilities::SPACE; // Return a default character for out-of-bounds coordinates
        }
        return currentBoard[y][x];
    }

    // Set the character at the specified position
    void setChar(int x, int y, char c) {
        if (x >= 0 && x < utilities::GAME_WIDTH && y >= 0 && y < utilities::GAME_HEIGHT) {
            currentBoard[y][x] = c;
        }
    }


	// Get the ghost positions
    const std::vector<std::pair<int, int>>& getGhostPositions() const { return ghostPositions; } 

    const std::vector<std::pair<int, int>>& getGhost_UPPositions() const { return ghost_UPPositions; }   
	// Add a ghost position to the vector
    void addGhostPosition(int x, int y) { ghostPositions.emplace_back(x, y); }  

    void addGhost_UPposition(int x, int y) { ghost_UPPositions.emplace_back(x, y); }  

    //Reset Ghost postisions vector
    void clearGhostPosition();
    void clearGhost_UPPosition(); 


 };
