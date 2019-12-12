#pragma once

using namespace Platform;

namespace App1 {
	class Game
	{
	public:
		Game();
		static Game* getInstance();
	private:
		static Game* mInstance;
	};
}

