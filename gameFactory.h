#pragma once
#include <memory>
#include <string>
#include "baseGame.h" 
#include "defaultGame.h"
#include "loadGame.h"



class gameFactory {
    public:
	static std::unique_ptr<baseGame> createGame(int argc, char* argv[]);
};

