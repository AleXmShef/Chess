// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "GameFindPage.xaml.h"
#include "GamePage.xaml.h"
#include "SettingsPage.xaml.h"

using namespace App1;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace Windows;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
}

void App1::MainPage::WelcomeMenuButtonOnFocus(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	Color color;
	color.R = 255;
	color.G = 114;
	color.B = 54;
	color.A = 255;
	auto brush = ref new SolidColorBrush(color);
	((TextBlock^)sender)->Foreground = brush;
}

void App1::MainPage::WelcomeMenuButtonOffFocus(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	Color color;
	color.R = 212;
	color.G = 212;
	color.B = 212;
	color.A = 255;
	auto brush = ref new SolidColorBrush(color);
	((TextBlock^)sender)->Foreground = brush;
}

void App1::MainPage::WelcomeMenuButtonOnClick(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (((TextBlock^)sender)->Name == "exit") {
		OutputDebugString(L"Exiting application\n");
		Application::Current->Exit();
	}
	else if (((TextBlock^)sender)->Name == "findGame") {
		auto rootFrame = dynamic_cast<Windows::UI::Xaml::Controls::Frame^>(Window::Current->Content);
		if (!rootFrame->Navigate(TypeName(GameFindPage::typeid))) {
			OutputDebugString(L"Failed to navigate to findGame screen\n");
		}
	}
	else if (((TextBlock^)sender)->Name == "settings") {
		auto rootFrame = dynamic_cast<Windows::UI::Xaml::Controls::Frame^>(Window::Current->Content);
		if (!rootFrame->Navigate(TypeName(SettingsPage::typeid))) {
			OutputDebugString(L"Failed to navigate to settings screen\n");
		}
	}
}

