#pragma once
#include <iostream>
#include "board.h"
#include "utilities.h"
#include "gameObject.h" 
#include "baseGame.h"

// Forward declaration of the game class
class game;

class mario : public gameObject {

private:
    static constexpr char keys[] = { 'a', 'w', 'd', 'x', 's','p'};
    static constexpr size_t numKeys = sizeof(keys) / sizeof(keys[0]);

    int fallCounter = 0;
    int lives = 3;
    int jumpStep = 0, jumpHeight = 2;
    bool isJumping = false;
    bool onLadder = false;
    bool marioHasHammer = false;
    bool marioUseHammer = false;
    bool hammerIsActive = false;
    bool pressP = false;
    bool useHeart = false;

    baseGame* pGame = nullptr;  // Pointer to the game object  
    board* pBoard = nullptr; // Pointer to the board object

    // Draw Mario at the current position
    void draw(char c) const;

      
      
public:
  
     
    // Constructor accepting a game pointer
    mario(baseGame* g)  
        : gameObject(0, 0, utilities::MARIO), // Default position (0, 0) and character '@'
        pGame(g),                           // Initialize the game pointer
        pBoard(nullptr),                    // Default board pointer to nullptr
        isJumping(false),                   // Initialize other attributes
        marioHasHammer(false),
        pressP(false),
        lives(3),
        jumpStep(0),
        jumpHeight(2) {
    }


    // destractor for allocad mamory
    ~mario() = default;

    //copy constractor
    mario(const mario& other);

    mario& operator=(const mario& other); // Assignment operator


    // Set the board reference
    void setBoard(board& board) {
        pBoard = &board;
    }

    // Draw Mario
    void draw() const override {
        draw(objectChar);
    }

    // Erase Mario from the current position
    void erase() override ;

    // Move Mario based on the current direction
    void move() override ; 
     
	void handleMovement();

	void handleJumping();

	void handleLadderTransitions();

	void handleItemPickup(); 

	void handleFalling();

    // Handle key press events
    void keyPressed(char key);

    // Change the direction of Mario based on the key pressed
    void changeDirection(char key) ; 

    // Get the number of lives remaining
    int getLife() const { return lives; }

    // Decrease the number of lives by one
    void loseLife();

    // Reset the number of lives to 3
    void resetLife() { lives = 3; }

    // Reset Mario's position
	void resetPosition(int startX, int startY);

    void isMariopickHammer() { marioHasHammer = true; }
    bool isMarioHasHammer() { return marioHasHammer; }
    bool getPressP() { return pressP; }
    void setPressP() { pressP = true; }
    void resetPressP() { pressP = false; }
    void marioWithHammer() { objectChar = utilities::HAMMER; }
    void marioWithoutHammer() { objectChar = utilities::MARIO; }
    void setMarioUseHeart() { useHeart= !useHeart; }
    bool getMarioUseHeart() { return useHeart; }
    void setLife(int newLives) { lives = newLives; }
 
};
