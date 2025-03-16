
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "utilities.h"
#include "gameFactory.h"
#include "baseGame.h"

using namespace std;
   

// Create the appropriate game mode based on the command line arguments
    int main(int argc, char* argv[]) {
        utilities::ShowConsoleCursor(false);
        auto game = gameFactory::createGame(argc, argv); // Dynamically create the appropriate game mode
        game->run(); // Call the polymorphic run() method
        return 0;
    }
