#pragma once
#include <vector>
#include <iostream> 

class utilities {
public:
	// Constructor to initialize any default settings
	utilities() = default;

	// Turn on or off color
	static bool colorSwitch;

	// ASCII code for the Escape key
	static constexpr char ESC = 27;

	// Constant representing a long fall distance
	static constexpr int LONGFALL = 4;

	// Maximum number of barrels that can be on the screen at once
	static constexpr int MAX_BARRELS = 10;
	static constexpr int EXPLOSION_RADIUS = 2;
	static int BARRELS_SPAWN_TIMER;

	// Game dimensions
	static constexpr int GAME_WIDTH = 80;
	static constexpr int GAME_HEIGHT = 25;

	// Characters representing game entities
	static constexpr char SPACE = ' ';
	static constexpr char LADDER = 'H';
	static constexpr char BARREL = 'O';
	static constexpr char MARIO = '@';
	static constexpr char PAULINA = '$';
	static constexpr char GHOST = 'x';
	static constexpr char DONKY = '&';
	static constexpr char HAMMER = 'p';
	static constexpr char BORDER = 'Q';
	static constexpr char LEGEND = 'L';
	static constexpr char HEART = '~';
	static constexpr char GHOST_UP = 'X'; 

	// Utility methods
	static void gotoxy(int x, int y);
	void drawCharInLocation(int x, int y, char c); 
	static bool isFloor(char board);
	bool isLadder(char board);
	static void showMenu();
	static void showStageSelectionMenu(const std::vector<std::string>& screenFiles);
	static void showErrorScreen(const std::string& message);
	static void ShowConsoleCursor(bool showFlag);
	static void gameOver();
	static void presentInstructions();
	static void displayWinnerScreen(int score);
	static void displayRoundWinScreen(int stageNumber);
	static void setColor(int color);
};










