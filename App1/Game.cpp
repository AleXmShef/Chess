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
	getLocalhost();
}

void Game::getLocalhost() {
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
	mLocalhost = myHostname;
}

String^ Game::getStringIP() {
	return mLocalhost->CanonicalName;
}

void Game::sendInvitation(String^ serverHost) {
	auto serverHostname = ref new HostName(serverHost);
	auto socket = ref new Sockets::StreamSocket();
	socket->Control->KeepAlive = false;
	create_task(socket->ConnectAsync(
		serverHostname,
		"22112",
		Sockets::SocketProtectionLevel::PlainSocket)).then([this, socket](task<void> previousTask) {
		try {
			previousTask.get();
			auto writer = ref new DataWriter(socket->OutputStream);
			auto helloJson = ref new JsonObject();
			helloJson->Insert("requestType", JsonValue::CreateStringValue("GameInvite"));
			auto jsonString = helloJson->Stringify();
			writer->WriteUInt32(writer->MeasureString(jsonString));
			writer->WriteString(jsonString);
			create_task(writer->StoreAsync()).then([this, socket, jsonString](task<unsigned int> writeTask)
				{
					try {
						writeTask.get();
						OutputDebugString(L"Invite sent successfully");
					}
					catch (Exception ^ e) {
						OutputDebugString(L"Failed to send invitation");
					}
				});
		}
		catch (Exception^ e) {
			OutputDebugString(L"Failed to connect\n");
		}
		}
	);
};

void Game::startServer() {
	mServerSocket = ref new Sockets::StreamSocketListener();
	mServerSocket->ConnectionReceived += ref new TypedEventHandler<Sockets::StreamSocketListener^, Sockets::StreamSocketListenerConnectionReceivedEventArgs^>(this, &Game::serverOnConnectHandler);
	mServerSocket->Control->KeepAlive = false;
	create_task(mServerSocket->BindEndpointAsync(mLocalhost, "22112")).then([this](task<void> previousTask) {
		try {
			previousTask.get();
			OutputDebugString(L"Listening on port 22112\n");
		}
		catch (Exception ^ e) {
			OutputDebugString(L"Unable to start server\n");
		}
	});
}

void Game::serverOnConnectHandler(Sockets::StreamSocketListener^ socket, Sockets::StreamSocketListenerConnectionReceivedEventArgs^ args) {
	auto reader = ref new DataReader(args->Socket->InputStream);
	OutputDebugString(L"Recieved connection\n");
}

void Game::serverRequestHandler(DataReader^ reader, Sockets::StreamSocket^ socket) {
	create_task(reader->LoadAsync(sizeof(UINT32))).then([this, reader, socket](unsigned int size)
		{
			if (size < sizeof(UINT32)) {
				OutputDebugString(L"Socket was closed before reading was complete");
				cancel_current_task();
			}
			unsigned int stringLength = reader->ReadUInt32();
			return create_task(reader->LoadAsync(stringLength)).then([this, reader, stringLength](unsigned int actualStringLength)
				{
					if (actualStringLength != stringLength) {
						OutputDebugString(L"Socket was closed before reading was complete");
						cancel_current_task();
					}
					auto myJson = ref new JsonObject();
					auto jsonString = reader->ReadString(actualStringLength);
					myJson->Parse(jsonString);
					auto requestType = myJson->GetNamedString("requestType");
					if (requestType == "GameInvite") {
						OutputDebugString(L"Fuck yeah");
					}
				});
		});
}

void Game::registerFindPage(GameFindPage^ page) {
	mGameFindPage = page;
}
