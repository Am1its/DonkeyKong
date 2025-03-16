#pragma once
#include <iostream>
#include "utilities.h"
#include "gameObject.h"
#include "ghost.h"


class ghostUpgrade: public ghost {
private:

    bool onLadder = false;
    
public:
    
    // Constructor to initialize ghostUpgrade with game and board references
    ghostUpgrade(baseGame* g, board& gameBoard, int startX, int startY, int initialDirX, int initialDirY, char sign)
        : ghost(g, gameBoard, startX, startY, initialDirX, initialDirY, sign), onLadder(false) {}

    // Copy constructor
    ghostUpgrade(const ghostUpgrade& other)
        : ghost(other), onLadder(other.onLadder) {}

    // Assignment operator
    ghostUpgrade& operator=(const ghostUpgrade& other) {
        if (this == &other) return *this;
        ghost::operator=(other);
        onLadder = other.onLadder;
        return *this;
    }

    ~ghostUpgrade() = default;
    void move() override;
    void setOnLedder(bool _onLedder) {onLadder = _onLedder; }
	bool getOnLedder() const { return onLadder; } 	
    void handelLedder();
};


