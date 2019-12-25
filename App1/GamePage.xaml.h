//
// GamePage.xaml.h
// Declaration of the GamePage class
//

#pragma once

#include "GamePage.g.h"
#include "Game.h"
#include "Board.h"
#include <vector>
#include <map>
#include <utility>

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
		void populateBoard();
		void updateBoard();
		void showWinDialog();
		void showLostDialog();
	private:
		bool isAnyChipSelected = false;
		std::pair<int, int> selectedChip;
		std::vector<std::pair<int, int>>* selectedMoves = new std::vector<std::pair<int, int>>;
		std::vector<Move^>* movesStack = new std::vector<Move^>;
		std::vector<Move^>* movesForSelectedChip = new std::vector<Move^>;
		Game^ mGame;
		Board^ mBoard;
		std::vector<std::vector<Image^>*>* mBoardVector;
		std::map<std::pair<int,int>, std::vector<Move^>*>* mMoveMap;
		std::vector<std::vector<Cell^>*>* mCellBoard;
		void ChipOnFocus(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void ChipOffFocus(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void ChipButtonClick(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
	};
}
