#pragma once 
#include <iostream>
#include "board.h"
#include "gameObject.h"
#include "baseGame.h"



class barrels : public gameObject {
private:
    int stepCount = 0;
    int countBarrelFall = 0;
    char floorType = utilities::SPACE; // Initialize to a default value
    bool explode = false;
    bool outOfBoard = false;

    baseGame* pGame = nullptr; 
    board* pBoard = nullptr;

    // Draw the barrel at the current position
    void draw(char c) const override {
        
        if (utilities::colorSwitch)
        {
            if (c == utilities::LADDER) utilities::setColor(6);
            else if (c == utilities::HAMMER) utilities::setColor(11);
            else if (c == utilities::PAULINA) utilities::setColor(14);
            else if (c == utilities::HEART) utilities::setColor(11);
            else if (utilities::isFloor(c)) utilities::setColor(7);
            else if (c == ' ');
            else utilities::setColor(8);
        }
        utilities::gotoxy(x, y);
        std::cout << c;
        utilities::setColor(7);
    }

public:
    // Constructor to initialize barrels with game and board references
    barrels(baseGame* g, board& gameBoard, int startX, int startY, int initialDirX, int initialDirY, char sign)
        : gameObject(startX, startY, initialDirX, initialDirY, sign), pGame(g), pBoard(&gameBoard) {}

    // Copy constructor
    barrels(const barrels& other)
        : gameObject(other), pGame(other.pGame), pBoard(other.pBoard) {}

    // Assignment operator
    barrels& operator=(const barrels& other) {
        if (this == &other) return *this;
        gameObject::operator=(other);
        pGame = other.pGame;
        pBoard = other.pBoard;
        return *this;
    }


    // Draw the barrel
    void draw() const {
        draw(objectChar);
    }

    // Erase the barrel from the current position
    void erase();

    // Move the barrel
   void move() override;

    // Get the step count
    int getStepCount() const { return stepCount; }

    // Increment the step count
    void incrementStepCount() { stepCount++; }

    // Check if the barrel has exploded
    bool getExplode() const { return explode; }

    bool getOutOfBoard() const { return outOfBoard; }

    int getcountBarrelFall() const { return countBarrelFall; }
       
};