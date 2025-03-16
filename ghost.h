#pragma once
#include <iostream>
#include "utilities.h"
#include "gameObject.h"


class baseGame;
class board;

class ghost : public gameObject
{
protected:

    baseGame* pGame = nullptr; 
    board* pBoard = nullptr;

    virtual void draw(char c) const { 

        if (utilities::colorSwitch)
        {
            if (c == utilities::LADDER)
                utilities::setColor(6);
            else if (c == utilities::DONKY)
                utilities::setColor(8);
            else if (c == utilities::HAMMER)
                utilities::setColor(11);
			else if (c == utilities::PAULINA)
                utilities::setColor(14);
            else if (c == utilities::HEART)
                utilities::setColor(11);
			else if (utilities::isFloor(c))
                utilities::setColor(7);
            else
                utilities::setColor(4);
        }
        utilities::gotoxy(x, y);
        std::cout << c;
        utilities::setColor(7);
    }
 
public:
    // Constructor to initialize ghost with game and board references
    ghost(baseGame* g, board& gameBoard, int startX, int startY, int initialDirX, int initialDirY, char sign)
        : gameObject(startX, startY, initialDirX, initialDirY, sign), pGame(g), pBoard(&gameBoard) {}

    // Copy constructor
    ghost(const ghost& other)
        : gameObject(other), pGame(other.pGame), pBoard(other.pBoard) {}

    // Assignment operator
    ghost& operator=(const ghost& other) {
        if (this == &other) return *this;
        gameObject::operator=(other);
        pGame = other.pGame;
        pBoard = other.pBoard;
        return *this;
    }

     ~ghost() = default;

    // Draw the barrel
    virtual void draw() const override { 
        draw(objectChar); 
    }

    // Erase the Ghost from the current position
    virtual void erase() override; 
     
    //chang the ghost dirction
    virtual void changeDirXGhost() {
        dir_x = -dir_x; 
    } 

    // Move the ghost
    virtual void move() override; 
    virtual void handelLedder()  {};
   
    virtual int changeDirY(int = 0) { return 0; };
    virtual void setOnLedder(bool _onLedder) {};
    // Set the board reference
    virtual void setBoard(board& gameBoard) {
        pBoard = &gameBoard;
    }
    virtual void setDirY(int _dirY) { dir_y = _dirY; }
    virtual void resetPosition(int startX, int startY); 
    virtual int getX() const { return x; }
    virtual int getY() const { return y; }
};