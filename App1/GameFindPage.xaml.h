//
// GameFindPage.xaml.h
// Declaration of the GameFindPage class
//

#pragma once

#include "GameFindPage.g.h"
#include "MainPage.xaml.h"
#include "Game.h"

namespace App1
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class GameFindPage sealed
	{
	public:
		GameFindPage();
	internal:
		task<bool> showInvitationDialog();
	private:
		Game^ _mGame;
		void buttonPressed(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void FindMenuButtonOnFocus(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void FindMenuButtonOffFocus(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void FindMenuButtonOnClick(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
	};
}
