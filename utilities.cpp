#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <vector>

#include "utilities.h"
#include <string>    

using namespace std;

bool utilities::colorSwitch = false;
int utilities::BARRELS_SPAWN_TIMER = 0;


void utilities::setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

void utilities::gotoxy(int x, int y) {
    HANDLE hConsole;
    COORD pos ={ 0, 0 };
    cout.flush();
    pos.X = x;
    pos.Y = y;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(hConsole, pos);
}

// Function to draw a single character at a specific location
void utilities::drawCharInLocation(int x, int y, char c) {
    gotoxy(x, y); // Move the cursor to the specified position
    std::cout << c; // Print the character
}

void utilities::ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = showFlag;
    SetConsoleCursorInfo(out, &cursorInfo);
}

bool utilities::isFloor(char board) {
    return (board == '<' || board == '>' || board == '=');
}
bool utilities::isLadder(char board) {
	return (board == 'H');
}

void utilities::showMenu() {
    system("cls");
    if (colorSwitch) setColor(11); // Light cyan
    const char* menuText[] = {
        "********************************************************************************",
        "*                                                                              *",
        "*     _____                 _                  _  __                           *",
        "*    |  __ \\               | |                | |/ /                           *",
        "*    | |  | |  ___   _ __  | | __ ___  _   _  | ' /  ___   _ __    __ _        *",
        "*    | |  | | / _ \\ | '_ \\ | |/ // _ \\| | | | |  <  / _ \\ | '_ \\  / _` |       *",
        "*    | |__| || (_) || | | ||   <|  __/| |_| | | . \\| (_) || | | || (_| |       *",
        "*    |_____/  \\___/ |_| |_||_|\\_\\\\___| \\__, | |_| \_\\\\___/ |_| |_| \\__, |       *",
        "*                                       __/ |                      __/ |       *",
        "*                                      |___/                      |___/        *",
        "*                                                                              *",
        "********************************************************************************",
        "*                                                                              *",
        "*   START A NEW GAME                - (press 1)                                *",
		"*   SELECT STAGE                    - (press 3)                                *",
        "*   SWITCH COLOR                    - (press 7)                                *",
        "*   PRESENT INSTRUCTIONS AND KEYS   - (press 8)                                *",
        "*   EXIT                            - (press 9)                                *",
        "*                                                                              *",
        "********************************************************************************",
        "*                                                                              *",
        "*                    @            Amit Oved           @                        *",
        "*                    &          Ziv Balassiano        &                        *",
        "*                                                                              *",
        "********************************************************************************"
    };

    for (int i = 0; i < 25; ++i) { // Adjusted to fill the screen height
        gotoxy(0, i);
        cout << menuText[i];
        Sleep(80); // Pause for 80 milliseconds
    }
    setColor(7); // Reset to default color
}

void utilities::showStageSelectionMenu(const std::vector<std::string>& screenFiles) {
    system("cls");
    if (colorSwitch) setColor(13); // Light magenta

    // Header
    const char* headerText[] = {
        "********************************************************************************",
        "*                            SELECT A STAGE                                    *",
        "********************************************************************************"
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "*                                                                              *",
        "********************************************************************************",
        "*   Select a stage number or press '8' to return to the main menu              *",
        "********************************************************************************"

    };

    for (int i = 0; i < 24; ++i) {
        gotoxy(0, i);
        cout << headerText[i];
        Sleep(80);
    }

    // Display the list of stages dynamically
    int line = 3; // Start listing stages from line 6
    for (size_t i = 0; i < screenFiles.size(); ++i) {
        gotoxy(1, line++);
        cout << "   " << (i + 1) << ". " << screenFiles[i];
        Sleep(80);
    }


    setColor(7); // Reset to default color
}

void utilities::showErrorScreen(const std::string& message) {
    system("cls"); // Clear screen

    if (colorSwitch)setColor(12); // Red color for errors
    std::cout << std::string(80, '*') << "\n"; // Top border

    for (int i = 0; i < 10; ++i) {
        std::cout << "*" << std::string(78, utilities::SPACE) << "*\n";
    }

    int padding = (78 - static_cast<int>(message.length())) / 2;
    std::cout << "*" << std::string(padding, utilities::SPACE)
        << message
        << std::string(78 - padding - message.length(), utilities::SPACE)
        << "*\n";

    for (int i = 0; i < 10; ++i) {
        std::cout << "*" << std::string(78, utilities::SPACE) << "*\n";
    }

    std::cout << std::string(80, '*') << "\n"; // Bottom border

    Sleep(3000); // Pause for user to read the error message
}

void utilities::presentInstructions() {
    system("cls");
    if (colorSwitch) setColor(14); // Yellow
    const char* instructionsText[] = {

        "********************************************************************************",
        "*                               *Instructions*                                 *",
        "********************************************************************************",
        "* !Welcome Hero!                                                               *",
        "*                                                                              *",
        "* You're playing as Mario(@), Use the keyboard keys to move:                   *",
        "*                                                                              *",
        "* 'A|a' - LEFT, 'D|d' - RIGHT, 'W|w' - UP, 'X|x' - DOWN, 'S|s' - STAY.         *",
        "*                                                                              *",
        "* Avoid Ghosts(x) and barrels(O) thrown from the Donkey Kong(&)                *",
        "*                                                                              *",
        "* The barrels move in the directions of the floor (<,>,=)                      *",
        "*                                                                              *",
        "* Be careful, they might explode if they fall 5 feet or higher                 *",
        "*                                                                              *",
        "* Use the Hammer (p) to strike the enemy by pressing 'P/p'                     *",
        "*                                                                              *",
        "* Climb the Ladders(H), and rescue Princess Pauline($)                         *",
        "*                                                                              *",
        "* You have 3 lives each game, collect (~) for bonus live                        *",       
        "*                                                                              *",
        "* Press 'ESC' to pause during the game.('8' - MainMenu,'ESC' - Resmue playing) *",
        "*                                                                              *",
        "* Press '8' to return to the main menu                                         *",
        "********************************************************************************"
    };

    for (int i = 0; i < 25; ++i) { // Adjusted to fill the screen height
        gotoxy(0, i);
        cout << instructionsText[i];
        Sleep(80); // Pause for 80 milliseconds
    }
    setColor(7); // Reset to default color
}

void utilities::gameOver() {
    system("cls");
    if (colorSwitch) setColor(12); // Light red
    const char* gameOverText[] = {
        "********************************************************************************",
        "*                                                                              *",
        "*        _____                         ____                                    *",
        "*       / ____|                       / __ \\                                   *",
        "*      | |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __                    *",
        "*      | | |_ |/ _` | '_ ` _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__|                   *",
        "*      | |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |                      *",
        "*       \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|                      *",
        "*                                                                              *",
        "*                                                                              *",
        "********************************************************************************",
        "*                                                                              *",
        "*                                                                              *",
        "*   Thank you for playing!                                                     *",
        "*                                                                              *",
        "*   Unfortunately, Mario could not save Princess Pauline this time.            *",
        "*                                                                              *",
        "*   Better luck next time, Hero!                                               *",
        "*                                                                              *",
        "*                                                                              *",
        "********************************************************************************",
        "*                                                                              *",
        "*   Press '8' to return to the main menu                                       *",
        "*                                                                              *",
        "********************************************************************************"
    };

    for (int i = 0; i < 25; ++i) {
        gotoxy(0, i);
        cout << gameOverText[i];
        Sleep(80); // Pause for 80 milliseconds
    }
    setColor(7); // Reset to default color
}

void utilities::displayRoundWinScreen(int stageNumber) {
    system("cls"); // Clear the screen
    if(colorSwitch) setColor(10); // Light green
    // Display a border around the screen
    std::cout << std::string(80, '*') << "\n"; // Top border
    for (int i = 0; i < 10; ++i) {
        std::cout << "*" << std::string(78, utilities::SPACE) << "*\n";
    }

    // Center-align the message
    std::string message = "Congratulations! You completed Stage " + std::to_string(stageNumber) + "!";
    int padding = (78 - static_cast<int>(message.length())) / 2;
    std::cout << "*" << std::string(padding, utilities::SPACE)
        << message
        << std::string(78 - padding - message.length(), utilities::SPACE)
        << "*\n";

    for (int i = 0; i < 10; ++i) {
        std::cout << "*" << std::string(78, utilities::SPACE) << "*\n";
    }
    std::cout << std::string(80, '*') << "\n"; // Bottom border

    Sleep(3000); // Pause for 3 seconds before transitioning
    setColor(7); // Reset to default color
}

void utilities::displayWinnerScreen(int score) {
    system("cls");
    if (colorSwitch) setColor(10); //light green
    const char* winnerText[] = {

        "********************************************************************************",
        "*                                                                              *",
        "*                            YOU ARE THE WINNER!                               *",
        "*                                                                              *",
        "********************************************************************************",
        "*    |@@@@|     |####|                                                         *",
        "*    |@@@@|     |####|                                                         *",
        "*    |@@@@|     |####|        Thank you for playing!                           *",
        "*    \\@@@@|     |####/                                                        *",
        "*     \\@@@|     |###/        Mario has successfully saves Princess Pauline    *",
        "*      `@@|_____|##'                                                           *",
        "*           (O)               You have proven yourself to be a true hero!      *",
        "*        .-'''''-.                                                             *",
        "*      .'  * * *  `.                                                           *",
        "*     :  *       *  :         Score :                                          *",
        "*    : ~  CONGRATS ~ :                                                         *",
        "*    : ~ YOU WON! ~ :                                                          *",
        "*     :  *       *  :                                                          *",
        "*      `.  * * *  .'                                                           *",
        "*        `-.....-'                                                             *",
        "*                                                                              *",
        "*                                                                              *",
        "*                     Press '8' to return to the main menu                     *",
        "*                                                                              *",
        "********************************************************************************"
    };

    for (int i = 0; i < 25; ++i) {
        gotoxy(0, i);
        cout << winnerText[i];
        Sleep(80); // Pause for 80 milliseconds
    }

    gotoxy(40, 14); // Move the cursor to the score position
    cout << score; // Display the score

    setColor(7); // Reset to default color
}