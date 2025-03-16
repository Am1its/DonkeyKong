#include "gameFactory.h"



// Function to create the appropriate game mode based on the command line arguments
std::unique_ptr<baseGame> gameFactory::createGame(int argc, char* argv[]){
    if (argc > 1) {
        std::string mode = argv[1];
        if (mode == "-save") {
            bool save = true;
            return std::make_unique<defaultGame>(save); // Create a default game with save mode
        }
        else if (mode == "-load") {
            bool silentMode = (argc > 2 && std::string(argv[2]) == "-silent");
            return std::make_unique<loadGame>(silentMode);  // Create a load game (with or without silent mode)
        }
    }
    // Default mode if no arguments are provided
    return std::make_unique<defaultGame>(false); // Create a default game without save mode
}
