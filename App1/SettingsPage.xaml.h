//
// SettingsPage.xaml.h
// Declaration of the SettingsPage class
//

#pragma once

#include "SettingsPage.g.h"

namespace App1
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class SettingsPage sealed
	{
	public:
		SettingsPage();
		void SettingsMenuButtonOnFocus(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void SettingsMenuButtonOffFocus(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void SettingsMenuButtonOnClick(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
	private:
		void Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
