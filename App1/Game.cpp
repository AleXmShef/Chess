#include "pch.h"
#include "Game.h"

using namespace App1;

Game* Game::mInstance = nullptr;

Game* Game::getInstance() {
	return mInstance;
}

Game::Game() {
	if (!(mInstance == nullptr)) {
		mInstance = this;
	}
	else {
		auto e = ref new Exception(E_FAIL, "Trying to create more than 1 instance of the Game singleton");
	}
}
