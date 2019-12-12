//
// SettingsPage.xaml.cpp
// Implementation of the SettingsPage class
//

#include "pch.h"
#include "SettingsPage.xaml.h"
#include "MainPage.xaml.h"

using namespace App1;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

SettingsPage::SettingsPage()
{
	InitializeComponent();
}

void SettingsPage::SettingsMenuButtonOnFocus(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	Color color;
	color.R = 255;
	color.G = 114;
	color.B = 54;
	color.A = 255;
	auto brush = ref new SolidColorBrush(color);
	((TextBlock^)sender)->Foreground = brush;
}

void SettingsPage::SettingsMenuButtonOffFocus(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	Color color;
	color.R = 212;
	color.G = 212;
	color.B = 212;
	color.A = 255;
	auto brush = ref new SolidColorBrush(color);
	((TextBlock^)sender)->Foreground = brush;
}

void SettingsPage::SettingsMenuButtonOnClick(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (((TextBlock^)sender)->Name == "back") {
		auto rootFrame = dynamic_cast<Windows::UI::Xaml::Controls::Frame^>(Window::Current->Content);
		if (!rootFrame->Navigate(TypeName(MainPage::typeid))) {
			OutputDebugString(L"Failed to navigate back to MainPage screen\n");
		}
	}
	else if (((TextBlock^)sender)->Name == "isFullScrn") {
		auto appView = ApplicationView::GetForCurrentView();
		if (!appView->IsFullScreen) {
			if (appView->TryEnterFullScreenMode()) {
				appView->PreferredLaunchWindowingMode = ApplicationViewWindowingMode::FullScreen;
				this->isFullScrn->Text = L"On";
			}
		}
		else {
			appView->ExitFullScreenMode();
			appView->PreferredLaunchWindowingMode = ApplicationViewWindowingMode::Auto;
			this->isFullScrn->Text = L"Off";
		}
	}
}


void App1::SettingsPage::Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	auto appView = ApplicationView::GetForCurrentView();
	if (appView->IsFullScreen) {
		this->isFullScrn->Text = L"On";
	}
}
