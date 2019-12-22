#include "pch.h"
#include "GameFindPage.xaml.h"
#include "GamePage.xaml.h"
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
	startServer();
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
			mSocket = socket;
			auto requestJson = ref new JsonObject();
			requestJson->Insert("requestType", JsonValue::CreateStringValue("GameInvite"));
			sendJson(requestJson, mSocket);
			recieveJson(mSocket).then([this](JsonObject^ responseJson)
				{
					responseHandler(responseJson);
				});
		}
		catch (Exception^ e) {
			OutputDebugString(L"Failed to connect\n");
			delete socket;
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
	mSocket = args->Socket;
	recieveJson(mSocket).then([this](JsonObject^ requestJson)
		{
			requestHandler(requestJson);
			//serverRequestHandler();
		});
	OutputDebugString(L"Recieved connection\n");
}

void Game::serverRequestHandler() {
	recieveJson(mSocket).then([this](JsonObject^ requestJson)
		{
			sendJson(requestHandler(requestJson), mSocket);
			serverRequestHandler();
		});
}

void Game::clientResponseHandler(DataReader^ reader, DataWriter^ writer, Sockets::StreamSocket^ socket) {
	
}

JsonObject^ Game::requestHandler(JsonObject^ jsonRequest) {
	auto requestType = jsonRequest->GetNamedString("requestType");
	if (requestType == "GameInvite") {
		if (mGameFindPage) {
			create_task(mGameFindPage->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this]()
				{
					this->mGameFindPage->showInvitationDialog().then([this](bool result)
						{
							if (result) {
								//confirm invitation
								mGameStatus = gameStatus::InviteAccepted;
								auto responseJson = ref new JsonObject();
								responseJson->Insert("responseType", JsonValue::CreateStringValue("GameInviteAccept"));
								sendJson(responseJson, mSocket);
								mGameFindPage->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this, result]() {
									auto rootFrame = dynamic_cast<Windows::UI::Xaml::Controls::Frame^>(Window::Current->Content);
									if (!rootFrame->Navigate(TypeName(GamePage::typeid))) {
										OutputDebugString(L"Failed to navigate to findGame screen\n");
									}
									}));
							}
							else {
								auto responseJson = ref new JsonObject();
								responseJson->Insert("responseType", JsonValue::CreateStringValue("GameInviteDecline"));
								sendJson(responseJson, mSocket);
							}
						});;
				})));
		}
	}
	else if (requestType == "WhichSide") {
		auto responseJson = ref new JsonObject();
		responseJson->Insert("responseType", JsonValue::CreateStringValue("WhichSide"));
		responseJson->Insert("responseContent", JsonValue::CreateStringValue("Brown"));
		return responseJson;
	}
	else if (requestType == "MyMove") {

	}
	else if (requestType == "YourMove") {

	}
	else if (requestType == "Consiede") {

	}
	else {

	}
}

void Game::responseHandler(JsonObject^ jsonResponse) {
	auto responseType = jsonResponse->GetNamedString("responseType");
	if (responseType == "GameInviteAccept") {
		mGameStatus = gameStatus::InviteAccepted;

		mGameFindPage->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this]() {
			auto rootFrame = dynamic_cast<Windows::UI::Xaml::Controls::Frame^>(Window::Current->Content);
			if (!rootFrame->Navigate(TypeName(GamePage::typeid))) {
				OutputDebugString(L"Failed to navigate to findGame screen\n");
			}
		}));
	}
	if (responseType == "GameInviteDecline") {

	}
}

void Game::registerFindPage(GameFindPage^ page) {
	mGameFindPage = page;
}

void Game::registerGamePage(GamePage^ page) {
	mGamePage = page;
}

gameStatus Game::getGameStatus() {
	return mGameStatus;
}



void Game::sendJson(JsonObject^ jsonParam, Sockets::StreamSocket^ socket) {
	auto writer = ref new DataWriter(socket->OutputStream);
	auto jsonString = jsonParam->Stringify();
	writer->WriteUInt32(writer->MeasureString(jsonString));
	writer->WriteString(jsonString);
	create_task(writer->StoreAsync()).then([this, socket](task<unsigned int> writeTask)
		{
			try {
				writeTask.get();
				OutputDebugString(L"Json sent successfully");
			}
			catch (Exception ^ e) {
				OutputDebugString(L"Failed to send json");
				delete socket;
			}
		});
}

task<JsonObject^> Game::recieveJson(Sockets::StreamSocket^ socket) {
	auto reader = ref new DataReader(socket->InputStream);
	return create_task(reader->LoadAsync(sizeof(UINT32))).then([this, reader, socket](unsigned int size)
		{
			if (size < sizeof(UINT32)) {
				OutputDebugString(L"Socket was closed before reading was complete\n");
				cancel_current_task();
			}
			unsigned int stringLength = reader->ReadUInt32();
			return create_task(reader->LoadAsync(stringLength)).then([this, reader, socket, stringLength](unsigned int actualStringLength)
				{
					if (actualStringLength != stringLength) {
						OutputDebugString(L"Socket was closed before reading was complete\n");
						cancel_current_task();
					}
					//auto myJson = ref new JsonObject();
					auto jsonString = reader->ReadString(actualStringLength);
					auto mJson = ref new JsonObject();
					mJson->TryParse(jsonString, &mJson);
					auto str = mJson->Stringify();
					return mJson;
				}).then([this, socket](JsonObject^ mJson)
					{
						try {
							//previousTask.get();
							return mJson;
						}
						catch (Exception ^ e) {
							OutputDebugString(L"Unknown error\n");
							delete socket;
						}
						catch (task_canceled&) {
							OutputDebugString(L"Unknown error\n");
							delete socket;
						}
					});
		});
}
