//
// GameFindPage.xaml.cpp
// Implementation of the GameFindPage class
//

#include "pch.h"
#include "GameFindPage.xaml.h"

using namespace App1;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

GameFindPage::GameFindPage()
{
	InitializeComponent();
	try {
		_mGame = Game::getInstance();
	}
	catch (Exception^ e) {
		///TODO
	}
	//auto str = _mGame->getLocalhost();
	_mGame->helloListenerThread();
}


void App1::GameFindPage::buttonPressed(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	_mGame->sendRequest();
}
