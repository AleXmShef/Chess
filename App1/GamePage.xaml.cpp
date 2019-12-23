//
// GamePage.xaml.cpp
// Implementation of the GamePage class
//

#include "pch.h"
#include "GamePage.xaml.h"
#include "MainPage.xaml.h"

using namespace App1;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

GamePage::GamePage()
{
	InitializeComponent();
	try {
		mGame = Game::getInstance();
	}
	catch (Exception ^ e) {

	}
	mGame->registerGamePage(this);
}

void GamePage::populateBoard() {
	mBoard = Board::getInstance();
	mBoardVector = new std::vector<std::vector<Image^>*>;
	for (int i = 0; i < 8; i++) {
		auto tvec = new std::vector<Image^>;
		for (int j = 0; j < 8; j++) {
			auto timg = ref new Image;
			timg->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Blank.png"));
			timg->Stretch = Stretch::UniformToFill;
			timg->PointerEntered += ref new PointerEventHandler(this, &GamePage::ChipOnFocus);
			timg->PointerExited += ref new PointerEventHandler(this, &GamePage::ChipOffFocus);
			timg->PointerPressed += ref new PointerEventHandler(this, &GamePage::ChipButtonClick);
			this->BoardGrid->Children->Append(timg);
			this->BoardGrid->SetColumn(timg, j);
			this->BoardGrid->SetRow(timg, i);
		}
	}
	updateBoard();
}

void GamePage::ChipOnFocus(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e) {
	if (mGame->getGameStatus() == gameStatus::MyMove) {
		int x = BoardGrid->GetColumn((Image^)sender);
		int y = 7 - BoardGrid->GetRow((Image^)sender);
		if (!isAnyChipSelected) {
			if ((*(*mCellBoard)[y])[x]->chip != nullptr && mMoveMap->count(std::pair<int, int>(x, y)) > 0) {
				if ((*(*mCellBoard)[y])[x]->chip->type == ChipType::Regular) {
					if ((*(*mCellBoard)[y])[x]->chip->colour == GameSide::Brown)
						((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Brown-selected.png"));
					else
						((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/White-selected.png"));
				}
				else {
					if ((*(*mCellBoard)[y])[x]->chip->colour == GameSide::Brown)
						((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Brown-crown-selected.png"));
					else
						((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/White-crown-selected.png"));
				}
			}
		}
		else {
			int xC = selectedChip.first;
			int yC = selectedChip.second;
			auto mChip = (*(*mCellBoard)[yC])[xC]->chip;
			auto mMoveVec = mMoveMap->at(std::pair<int, int>(xC, yC));
			for (int i = 0; i < mMoveVec->size(); i++) {
				if ((*mMoveVec)[i]->toXY.first == x && (*mMoveVec)[i]->toXY.second == y)
					((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Blank-selected.png"));
			}
		}
	}
};

void GamePage::ChipOffFocus(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e) {
	if (mGame->getGameStatus() == gameStatus::MyMove) {
		int x = BoardGrid->GetColumn((Image^)sender);
		int y = 7 - BoardGrid->GetRow((Image^)sender);
		if ((isAnyChipSelected == true && x != selectedChip.first && y != selectedChip.second) || isAnyChipSelected == false) {
			if ((*(*mCellBoard)[y])[x]->chip == nullptr)
				((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Blank.png"));
			else if ((*(*mCellBoard)[y])[x]->chip->type == ChipType::Regular) {
				if ((*(*mCellBoard)[y])[x]->chip->colour == GameSide::Brown)
					((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Brown.png"));
				else 
					((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/White.png"));
			}
			else {
				if ((*(*mCellBoard)[y])[x]->chip->colour == GameSide::Brown)
					((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Brown-crown.png"));
				else
					((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/White-crown.png"));
			}
		}
	}
}

void GamePage::ChipButtonClick(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e) {
	if (mGame->getGameStatus() == gameStatus::MyMove) {
		int x = BoardGrid->GetColumn((Image^)sender);
		int y = 7 - BoardGrid->GetRow((Image^)sender);
		if (!isAnyChipSelected) {
			if ((*(*mCellBoard)[y])[x]->chip != nullptr && mMoveMap->count(std::pair<int, int>(x, y)) > 0) {
				if ((*(*mCellBoard)[y])[x]->chip->colour == GameSide::Brown)
					((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Brown-selected.png"));
				else
					((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/White-selected.png"));
				isAnyChipSelected = true;
				selectedChip.first = x;
				selectedChip.second = y;
			}
		}
		else {
			int xC = selectedChip.first;
			int yC = selectedChip.second;
			auto mChip = (*(*mCellBoard)[yC])[xC]->chip;
			auto mMoveVec = mMoveMap->at(std::pair<int, int>(xC, yC));
			for (int i = 0; i < mMoveVec->size(); i++) {
				if ((*mMoveVec)[i]->toXY.first == x && (*mMoveVec)[i]->toXY.second == y)
					mBoard->move((*mMoveVec)[i]);
			}
		}
	}
}

void GamePage::updateBoard() {
	isAnyChipSelected = false;
	mCellBoard = mBoard->getCellBoard();
	mBoard->moveAvailibilityPass();
	mMoveMap = mBoard->getMoveAvailibilityPassMap();
	for (unsigned int i = 0; i < BoardGrid->Children->Size; i++) {
		auto timg = (Image^)(BoardGrid->Children->GetAt(i));
		int x = BoardGrid->GetColumn(timg);
		int y = 7 -  BoardGrid->GetRow(timg);
		if ((*(*mCellBoard)[y])[x]->chip != nullptr) {
			if ((*(*mCellBoard)[y])[x]->chip->type == ChipType::Regular) {
				if ((*(*mCellBoard)[y])[x]->chip->colour == GameSide::Brown)
					timg->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Brown.png"));
				else
					timg->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/White.png"));
			}
			else {
				if ((*(*mCellBoard)[y])[x]->chip->colour == GameSide::Brown)
					timg->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Brown-crown.png"));
				else
					timg->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/White-crown.png"));
			}
		}
		else
			timg->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Blank.png"));
	}
}

void GamePage::showLoseDialog() {
	create_task(LoseDialog->ShowAsync()).then([this]()
		{
			delete mGame;
			delete mBoard;
			auto rootFrame = dynamic_cast<Windows::UI::Xaml::Controls::Frame^>(Window::Current->Content);
			if (!rootFrame->Navigate(TypeName(MainPage::typeid))) {
				OutputDebugString(L"Failed to navigate to settings screen\n");
			}
		});
}
void GamePage::showWinDialog() {
	create_task(WinDialog->ShowAsync()).then([this]()
		{
			delete mGame;
			delete mBoard;
			auto rootFrame = dynamic_cast<Windows::UI::Xaml::Controls::Frame^>(Window::Current->Content);
			if (!rootFrame->Navigate(TypeName(MainPage::typeid))) {
				OutputDebugString(L"Failed to navigate to settings screen\n");
			}
		});
}
