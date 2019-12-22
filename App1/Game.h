#pragma once
#include "pch.h"
#include "GameFindPage.g.h"
#include "GamePage.g.h"
#include <string>
#include <vector>
#include "Board.h"

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Data::Json;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::Networking;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Networking::Connectivity;
using namespace Windows::Security::Cryptography;
using namespace Windows::Storage::Streams;
using namespace concurrency;

namespace App1 {
	public enum class gameStatus { Idle, InviteAccepted, PopulatingBoard, DecidingSide, GameStart, MyMove, NotMyMove };
	public enum class gameSide {White, Brown};
	ref class Game sealed
	{
	public:
		static Game^ getInstance();
		String^ getStringIP();
		void sendInvitation(String^ serverHost);
		//void acceptInvitation();
		//void declineInvitation();
		void registerFindPage(GameFindPage^ page);
		void registerGamePage(GamePage^ page);
		gameStatus getGameStatus();
	private:
		Game();
		void Init();
		void sendJson(JsonObject^ jsonParam, Sockets::StreamSocket^ socket);
		task<JsonObject^> recieveJson(Sockets::StreamSocket^ socket);
		void getLocalhost();
		void startServer();
		void serverOnConnectHandler(Sockets::StreamSocketListener^ socket, Sockets::StreamSocketListenerConnectionReceivedEventArgs^ args);
		void serverRequestHandler();
		void clientResponseHandler(DataReader^ reader, DataWriter^ writer, Sockets::StreamSocket^ socket);
		JsonObject^ requestHandler(JsonObject^ jsonRequest);
		void responseHandler(JsonObject^ jsonResponse);
	private:
		DataWriter^ mWriter;
		DataReader^ mReader;
		gameStatus mGameStatus = gameStatus::Idle;
		gameSide mGameSide;
		GameFindPage^ mGameFindPage;
		GamePage^ mGamePage;
		HostName^ mLocalhost;
		Board^ mBoard;
		Sockets::StreamSocketListener^ mServerSocket;
		Sockets::StreamSocket^ mSocket;
		static Game^ mInstance;
	};
}

