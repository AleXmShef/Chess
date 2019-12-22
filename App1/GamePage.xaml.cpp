//
// GamePage.xaml.cpp
// Implementation of the GamePage class
//

#include "pch.h"
#include "GamePage.xaml.h"

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
	populateBoard();
}

void GamePage::populateBoard() {
	mBoardVector = new std::vector<std::vector<Image^>*>;
	for (int i = 0; i < 8; i++) {
		auto tvec = new std::vector<Image^>;
		for (int j = 0; j < 8; j++) {
			auto timg = ref new Image;
			timg->Source = ref new BitmapImage(ref new Uri("ms-appx:///Assets/cross-out-clipart-5.png"));
			timg->Stretch = Stretch::UniformToFill;
			this->BoardGrid->Children->Append(timg);
			this->BoardGrid->SetColumn(timg, j);
			this->BoardGrid->SetRow(timg, i);
			tvec->push_back(timg);
		}
		mBoardVector->push_back(tvec);
	}
}
