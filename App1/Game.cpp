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
			isServer = false;
			mReader = ref new DataReader(mSocket->InputStream);
			mWriter = ref new DataWriter(mSocket->OutputStream);
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
	mReader = ref new DataReader(mSocket->InputStream);
	mWriter = ref new DataWriter(mSocket->OutputStream);
	recieveJson(mSocket).then([this](JsonObject^ requestJson)
		{
			requestHandler(requestJson);
			serverRequestHandler();
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
		mGameSide = GameSide::White;
		mGameStatus = gameStatus::MyMove;
		mBoard = Board::getInstance();
		mBoard->populateBoard(mGameSide);
		while (mGamePage == nullptr) {

		};
		mGamePage->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this]() {
			mGamePage->populateBoard();
		}));
		auto responseJson = ref new JsonObject();
		responseJson->Insert("responseType", JsonValue::CreateStringValue("WhichSide"));
		responseJson->Insert("responseContent", JsonValue::CreateStringValue("Brown"));
		return responseJson;
	}
	else if (requestType == "MyMove") {
		auto requestContent = jsonRequest->GetNamedString("requestContent");
		if (requestContent == "MoveReady") {
			mBoard->moveFromJson(jsonRequest);
			mGameStatus = gameStatus::MyMove;
			mGamePage->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this]() {
				mGamePage->updateBoard();
			}));
			auto responseJson = ref new JsonObject();
			responseJson->Insert("responseType", JsonValue::CreateStringValue("OK boomer"));
			return responseJson;
		}
	}
	else if (requestType == "YourMove") {
		if (mGameStatus == gameStatus::MyMove) {
			auto respJson = ref new JsonObject();
			respJson->Insert("responseType", JsonValue::CreateStringValue("MyMove"));
			respJson->Insert("responseContent", JsonValue::CreateStringValue("Await"));
			return respJson;
		}
		else if (mGameStatus == gameStatus::NotMyMove) {
			return mMyLastMove;
		}
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
		auto requestJson = ref new JsonObject();
		requestJson->Insert("requestType", JsonValue::CreateStringValue("WhichSide"));
		sendJson(requestJson, mSocket);
		recieveJson(mSocket).then([this](JsonObject^ responseJson)
			{
				responseHandler(responseJson);
			});

		mGameFindPage->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this]() {
			auto rootFrame = dynamic_cast<Windows::UI::Xaml::Controls::Frame^>(Window::Current->Content);
			if (!rootFrame->Navigate(TypeName(GamePage::typeid))) {
				OutputDebugString(L"Failed to navigate to findGame screen\n");
			}
		}));
	}
	if (responseType == "GameInviteDecline") {

	}
	if (responseType == "WhichSide") {
		auto side = jsonResponse->GetNamedString("responseContent");
		if (side == "White") {
			mGameSide = GameSide::White;
			mGameStatus = gameStatus::MyMove;
		}
		else {
			mGameSide = GameSide::Brown;
			mGameStatus = gameStatus::NotMyMove;
			auto workItem = ref new Windows::System::Threading::WorkItemHandler([this](IAsyncAction^ workItem)
				{
					awaitMoveClient();
				});
			auto asyncAction = Windows::System::Threading::ThreadPool::RunAsync(workItem);
		}
		mBoard = Board::getInstance();
		mBoard->populateBoard(mGameSide);
		while (mGamePage == nullptr) {

		};
		mGamePage->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this]() {
			this->mGamePage->populateBoard();
		}));
	}
	if (responseType == "MyMove") {
		auto content = jsonResponse->GetNamedString("responseContent");
		if (content == "Await") {

		}
		else if (content == "MoveReady") {
			mGameStatus = gameStatus::MyMove;
			mBoard->moveFromJson(jsonResponse);
			mGamePage->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this]() {
				this->mGamePage->updateBoard();
			}));
		}
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
	auto jsonString = jsonParam->Stringify();
	mWriter->WriteUInt32(mWriter->MeasureString(jsonString));
	mWriter->WriteString(jsonString);
	create_task(mWriter->StoreAsync()).then([this, socket](task<unsigned int> writeTask)
		{
			try {
				writeTask.get();
				OutputDebugString(L"Json sent successfully\n");
			}
			catch (Exception ^ e) {
				OutputDebugString(L"Failed to send json\n");
				delete socket;
			}
		});
}

task<JsonObject^> Game::recieveJson(Sockets::StreamSocket^ socket) {
	return create_task(mReader->LoadAsync(sizeof(UINT32))).then([this](unsigned int size)
		{
			if (size < sizeof(UINT32)) {
				OutputDebugString(L"Socket was closed before reading was complete\n");
				cancel_current_task();
			}
			unsigned int stringLength = mReader->ReadUInt32();
			return create_task(mReader->LoadAsync(stringLength)).then([this, stringLength](unsigned int actualStringLength)
				{
					if (actualStringLength != stringLength) {
						OutputDebugString(L"Socket was closed before reading was complete\n");
						cancel_current_task();
					}
					//auto myJson = ref new JsonObject();
					auto jsonString = mReader->ReadString(actualStringLength);
					auto mJson = ref new JsonObject();
					mJson->TryParse(jsonString, &mJson);
					auto str = mJson->Stringify();
					return mJson;
				}).then([this](JsonObject^ mJson)
					{
						try {
							//previousTask.get();
							return mJson;
						}
						catch (Exception ^ e) {
							OutputDebugString(L"Unknown error\n");
							delete mSocket;
						}
						catch (task_canceled&) {
							OutputDebugString(L"Unknown error\n");
							delete mSocket;
						}
					});
		});
}

GameSide Game::getGameSide() {
	return mGameSide;
}

void Game::awaitMoveClient() {
	recieveJson(mSocket).then([this](JsonObject^ resp) 
		{
			responseHandler(resp);
			awaitMoveClient();
		});
}

void Game::sendMove(JsonObject^ jsonMove) {
	if (!isServer) {
		jsonMove->Insert("requestType", JsonValue::CreateStringValue("MyMove"));
		jsonMove->Insert("requestContent", JsonValue::CreateStringValue("MoveReady"));
	}
	else {
		jsonMove->Insert("responseType", JsonValue::CreateStringValue("MyMove"));
		jsonMove->Insert("responseContent", JsonValue::CreateStringValue("MoveReady"));
	}
	sendJson(jsonMove, mSocket);
	mGamePage->Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([this]() {
		this->mGamePage->updateBoard();
	}));
	mGameStatus = gameStatus::NotMyMove;

};

void Game::winMessage() {

}

void Game::loseMessage() {

}