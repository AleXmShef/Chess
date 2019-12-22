//
// GamePage.xaml.h
// Declaration of the GamePage class
//

#pragma once

#include "GamePage.g.h"
#include "Game.h"
#include "Board.h"
#include <vector>

using namespace Platform::Collections;
using namespace Windows::UI::Xaml::Controls;

namespace App1
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class GamePage sealed
	{
	public:
		GamePage();
	private:
		Game^ mGame;
		void populateBoard();
		std::vector<std::vector<Image^>*>* mBoardVector;
	};
}
