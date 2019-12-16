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
	_localhost = getMyIP();
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
	auto mySocket = ref new Sockets::StreamSocket();
	auto serverHost = ref new HostName("192.168.1.218");
	mySocket->ConnectAsync(serverHost, L"8081");
	auto request = ref new String(L"hello");
	auto stream = mySocket->OutputStream;
	auto req = CryptographicBuffer::ConvertStringToBinary(request, BinaryStringEncoding::Utf8);
	stream->WriteAsync(req);
	stream->FlushAsync();
};

void Game::helloListenerThread() {
	auto myListener = ref new Sockets::StreamSocketListener();
	myListener->ConnectionReceived += ref new TypedEventHandler<Sockets::StreamSocketListener^, Sockets::StreamSocketListenerConnectionReceivedEventArgs^>(this, &Game::respond);
	myListener->BindServiceNameAsync(L"8081");
}

void Game::respond(Sockets::StreamSocketListener^ socket, Sockets::StreamSocketListenerConnectionReceivedEventArgs^ args) {
	auto stream = args->Socket->InputStream;
	auto request = ref new String();
	request = stream->ToString();
	std::wstring req(request->Begin());
	OutputDebugString(req.c_str());
}
