#pragma once
#include <iostream>
#include <Windows.h>
#include "utilities.h" 
#include "baseGame.h"

class gameObject {
protected:
    int x, y; // Position of the game object
    int dir_x = 0;
    int dir_y = 0;
    char objectChar;

public:
    // Constructor 1: Initialize with position and symbol
    gameObject(int startX, int startY, char symbol) : x(startX), y(startY), objectChar(symbol) {}

    // Constructor 2: Initialize with position, directions, and symbol
    gameObject(int startX, int startY, int dirX, int dirY, char symbol)
        : x(startX), y(startY), dir_x(dirX), dir_y(dirY), objectChar(symbol) {}

    // Copy constructor
    gameObject(const gameObject& other)
        : x(other.x), y(other.y), dir_x(other.dir_x), dir_y(other.dir_y), objectChar(other.objectChar) {}

    
    // Virtual destructor
    virtual ~gameObject() = default;

    // Draw the object at the specified position
    virtual void draw(char c) const {
        utilities::gotoxy(x, y);  
        std::cout << objectChar;  
    }
    // Pure virtual method for movement
    virtual void move() = 0;

    virtual void erase() {
        draw(utilities::SPACE); // Clear the object from the board
    }

    // Draw the object
    virtual void draw() const {
        draw(objectChar);
    }
    void setCharoObject(char c) { objectChar = c; } 
    int getX() const { return x; }
    int getY() const { return y; }
    int getDirX() const { return dir_x; }
    int getDirY() const { return dir_y; };
    void setPosition(int newX, int newY) { x = newX; y = newY; }
    void setDirection(int newDirX, int newDirY) { dir_x = newDirX; dir_y = newDirY; }
};
