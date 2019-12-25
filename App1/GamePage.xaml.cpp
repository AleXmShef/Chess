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
				if ((*(*mCellBoard)[y])[x]->chip->colour == GameSide::Brown) {
					if((*(*mCellBoard)[y])[x]->chip->type == ChipType::Regular)
						((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Brown-selected.png"));
					else
						((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Brown-crown-selected.png"));
				}
				else {
					if ((*(*mCellBoard)[y])[x]->chip->type == ChipType::Regular)
						((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/White-selected.png"));
					else
						((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/White-crown-selected.png"));
				}
			}
		}
		else {
			if (movesStack->empty()) {
				int xC = selectedChip.first;
				int yC = selectedChip.second;
				auto mChip = (*(*mCellBoard)[yC])[xC]->chip;
				auto mMoveVec = mMoveMap->at(std::pair<int, int>(xC, yC));
				for (int i = 0; i < mMoveVec->size(); i++) {
					if ((*mMoveVec)[i]->toXY.first == x && (*mMoveVec)[i]->toXY.second == y)
						((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Blank-selected.png"));
				}
			}
			else {
				for (int i = 0; i < movesForSelectedChip->size(); i++) {
					if ((*movesForSelectedChip)[i]->fromXY.first == x && (*movesForSelectedChip)[i]->fromXY.second == y) {
						((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Blank-selected.png"));
					}
				}
			}
		}
	}
};

void GamePage::ChipOffFocus(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e) {
	if (mGame->getGameStatus() == gameStatus::MyMove) {
		int x = BoardGrid->GetColumn((Image^)sender);
		int y = 7 - BoardGrid->GetRow((Image^)sender);
		bool flag  = 0;
		if (!selectedMoves->empty()) {
			for (int i = 0; i < selectedMoves->size(); i++) {
				if ((*selectedMoves)[i].first == x && (*selectedMoves)[i].second == y)
					flag = 1;
			}
		}
		if (((isAnyChipSelected == true && x != selectedChip.first && y != selectedChip.second) || isAnyChipSelected == false) && !flag) {
			if ((*(*mCellBoard)[y])[x]->chip == nullptr)
				((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Blank.png"));
			else if ((*(*mCellBoard)[y])[x]->chip->colour == GameSide::Brown) {
				if ((*(*mCellBoard)[y])[x]->chip->type == ChipType::Regular)
					((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Brown.png"));
				else
					((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Brown-crown.png"));
			}
			else if ((*(*mCellBoard)[y])[x]->chip->colour == GameSide::White) {
				if ((*(*mCellBoard)[y])[x]->chip->type == ChipType::Regular)
					((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/White.png"));
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
				if ((*(*mCellBoard)[y])[x]->chip->colour == GameSide::Brown) {
					if ((*(*mCellBoard)[y])[x]->chip->type == ChipType::Regular)
						((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Brown-selected.png"));
					else
						((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Brown-crown-selected.png"));
				}
				else {
					if ((*(*mCellBoard)[y])[x]->chip->type == ChipType::Regular)
						((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/White-selected.png"));
					else
						((Image^)sender)->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/White-crown-selected.png"));
				}
				isAnyChipSelected = true;
				selectedChip.first = x;
				selectedChip.second = y;
			}
		}
		else {
			int xC = selectedChip.first;
			int yC = selectedChip.second;
			if (x == xC && y == yC) {
				isAnyChipSelected = false;
			}
			else {
				if (movesStack->empty()) {
					auto mChip = (*(*mCellBoard)[yC])[xC]->chip;
					auto mMoveVec = mMoveMap->at(std::pair<int, int>(xC, yC));
					for (int i = 0; i < mMoveVec->size(); i++) {
						if ((*mMoveVec)[i]->toXY.first == x && (*mMoveVec)[i]->toXY.second == y) {
							movesStack->push_back((*mMoveVec)[i]);
							if ((*mMoveVec)[i]->isCutting) {
								int xCC;
								int yCC;
								for (int h = 0; h < 8; h++) {
									for (int j = 0; j < 8; j++) {
										if ((*(*mCellBoard)[i])[j]->chip == (*mMoveVec)[i]->cuttedChip) {
											xCC = j;
											yCC = h;
										}
									}
								}
								if (mBoard->findMovesForCurrentChip(std::pair<int, int>(xC, yC), std::pair<int, int>(xCC, yCC), mChip->type)->empty())
									mBoard->move(movesStack);
								else {
									movesForSelectedChip = mBoard->findMovesForCurrentChip(std::pair<int, int>(xC, yC), std::pair<int, int>(xCC, yCC), mChip->type);
									selectedMoves->push_back(std::pair<int, int>(xC, yC));
								}
							}
						}
					}
				}
				else {
					for (int i = 0; i < movesForSelectedChip->size(); i++) {
						if ((*movesForSelectedChip)[i]->toXY.first == x && (*movesForSelectedChip)[i]->toXY.second == y) {
							movesStack->push_back((*movesForSelectedChip)[i]);
							int xCC;
							int yCC;
							for (int i = 0; i < 8; i++) {
								for (int j = 0; j < 8; j++) {
									if ((*(*mCellBoard)[i])[j]->chip == movesStack->back()->cuttedChip) {
										xCC = j;
										yCC = i;
									}
								}
							}
							int xC = movesStack->back()->fromXY.first;
							int yC = movesStack->back()->fromXY.second;
							auto t = mBoard->findMovesForCurrentChip(std::pair<int, int>(xC, yC), std::pair<int, int>(xCC, yCC), (*(*mCellBoard)[movesStack->front()->fromXY.second])[movesStack->front()->fromXY.first]->chip->type);
							if (t->empty()) {
								mBoard->move(movesStack);
							}
							else {
								movesForSelectedChip = t;
								selectedMoves->push_back(std::pair<int, int>(xC, yC));
							}
						}
					}
				}
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
			if ((*(*mCellBoard)[y])[x]->chip->colour == GameSide::Brown) {
				if ((*(*mCellBoard)[y])[x]->chip->type == ChipType::Regular)
					timg->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Brown.png"));
				else
					timg->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Brown-crown.png"));
			}
			else {
				if ((*(*mCellBoard)[y])[x]->chip->type == ChipType::Regular)
					timg->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/White.png"));
				else
					timg->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/White-crown.png"));
			}
		}
		else
			timg->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/Blank.png"));
	}
}

void GamePage::showLostDialog() {
	create_task(LoseDialog->ShowAsync()).then([this](ContentDialogResult res)
		{
			mGame->closeGame();
			auto rootFrame = dynamic_cast<Windows::UI::Xaml::Controls::Frame^>(Window::Current->Content);
			if (!rootFrame->Navigate(TypeName(MainPage::typeid))) {
				OutputDebugString(L"Failed to navigate to settings screen\n");
			}
		});
}

void GamePage::showWinDialog() {
	create_task(WinDialog->ShowAsync()).then([this](ContentDialogResult res)
		{
			mGame->closeGame();
			auto rootFrame = dynamic_cast<Windows::UI::Xaml::Controls::Frame^>(Window::Current->Content);
			if (!rootFrame->Navigate(TypeName(MainPage::typeid))) {
				OutputDebugString(L"Failed to navigate to settings screen\n");
			}
		});
}
