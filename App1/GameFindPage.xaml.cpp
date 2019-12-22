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
using namespace Windows::UI;
using namespace Windows;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Interop;
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
	_mGame->registerFindPage(this);
	ipTextBlock->Text = _mGame->getStringIP();
}

void App1::GameFindPage::FindMenuButtonOnFocus(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	Color color;
	color.R = 255;
	color.G = 114;
	color.B = 54;
	color.A = 255;
	auto brush = ref new SolidColorBrush(color);
	((TextBlock^)sender)->Foreground = brush;
}


void App1::GameFindPage::FindMenuButtonOffFocus(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	Color color;
	color.R = 212;
	color.G = 212;
	color.B = 212;
	color.A = 255;
	auto brush = ref new SolidColorBrush(color);
	((TextBlock^)sender)->Foreground = brush;
}


void App1::GameFindPage::FindMenuButtonOnClick(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (((TextBlock^)sender)->Name == "sendInvite") {
		OutputDebugString(L"Sending Invite\n");
		_mGame->sendInvitation(ipUserInput->Text);
	}
	else if (((TextBlock^)sender)->Name == "back") {
		auto rootFrame = dynamic_cast<Windows::UI::Xaml::Controls::Frame^>(Window::Current->Content);
		if (!rootFrame->Navigate(TypeName(MainPage::typeid))) {
			OutputDebugString(L"Failed to navigate to main screen\n");
		}
	}
}
