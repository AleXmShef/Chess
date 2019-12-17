#include "pch.h"
#include "Game.h"

using namespace App1;

Game^ Game::mInstance = nullptr;

Game^ Game::getInstance() {
	if (mInstance == nullptr)
		mInstance = ref new Game();
	return mInstance;
}

Game::Game() {
	Init();
}

void Game::Init() {
	//_localhost = getMyIP();
}

std::string Game::getMyIP() {
	auto icp = NetworkInformation::GetInternetConnectionProfile();
	auto hostnames = NetworkInformation::GetHostNames();
	HostName^ myHostname;
	for each (HostName^ hostname in hostnames)
	{
		if (hostname->Type == HostNameType::Ipv4
			&& hostname->IPInformation->NetworkAdapter != nullptr
			&& hostname->IPInformation->NetworkAdapter->NetworkAdapterId == icp->NetworkAdapter->NetworkAdapterId)
			myHostname = hostname;
	}
	std::wstring abc(myHostname->CanonicalName->Begin());
	std::string res(abc.begin(), abc.end());
	return res;
}

String^ Game::getLocalhost() {
	std::wstring abc(_localhost.begin(), _localhost.end());
	auto res = ref new String(abc.c_str(), abc.length());
	return res;
}

void Game::sendRequest() {
	auto icp = NetworkInformation::GetInternetConnectionProfile();
	auto hostnames = NetworkInformation::GetHostNames();
	HostName^ myHostname = nullptr;
	for each (HostName ^ hostname in hostnames)
	{
		if (hostname->Type == HostNameType::Ipv4
			&& hostname->IPInformation->NetworkAdapter != nullptr
			&& hostname->IPInformation->NetworkAdapter->NetworkAdapterId == icp->NetworkAdapter->NetworkAdapterId)
			myHostname = hostname;
	}
	auto socket = ref new Sockets::StreamSocket();
	socket->Control->KeepAlive = false;
	CoreApplication::Properties->Insert("clientSocket", socket);
	create_task(socket->ConnectAsync(
		myHostname,
		"22112",
		Sockets::SocketProtectionLevel::PlainSocket)).then([this](task<void> previousTask) {
		try {
			previousTask.get();
		}
		catch (Exception^ e) {
			OutputDebugString(L"Failed to connect\n");
		}
		}
	);
};

void Game::helloListenerThread() {
	auto icp = NetworkInformation::GetInternetConnectionProfile();
	auto hostnames = NetworkInformation::GetHostNames();
	HostName^ myHostname = nullptr;
	for each (HostName ^ hostname in hostnames)
	{
		if (hostname->Type == HostNameType::Ipv4
			&& hostname->IPInformation->NetworkAdapter != nullptr
			&& hostname->IPInformation->NetworkAdapter->NetworkAdapterId == icp->NetworkAdapter->NetworkAdapterId)
			myHostname = hostname;
	}
	auto listener = ref new Sockets::StreamSocketListener();
	listener->ConnectionReceived += ref new TypedEventHandler<Sockets::StreamSocketListener^, Sockets::StreamSocketListenerConnectionReceivedEventArgs^>(this, &Game::respond);
	listener->Control->KeepAlive = false;
	CoreApplication::Properties->Insert("listener", listener);
	create_task(listener->BindEndpointAsync(myHostname, "22112")).then([this](task<void> previousTask) {
		try {
			previousTask.get();
			OutputDebugString(L"Listening on port 22112\n");
		}
		catch (Exception ^ e) {
			OutputDebugString(L"Unable to listen\n");
		}
	
	});
}

void Game::respond(Sockets::StreamSocketListener^ socket, Sockets::StreamSocketListenerConnectionReceivedEventArgs^ args) {
	auto reader = ref new DataReader(args->Socket->InputStream);
	OutputDebugString(L"Test\n");
}
