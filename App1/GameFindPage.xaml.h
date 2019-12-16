//
// GameFindPage.xaml.h
// Declaration of the GameFindPage class
//

#pragma once

#include "GameFindPage.g.h"
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
	private:
		Game^ _mGame;
		void sendRequest(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
	};
}
