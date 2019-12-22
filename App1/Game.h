#pragma once
#include <string>
#include <vector>
#include "GameFindPage.g.h"
#include "Board.h"

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Data::Json;
using namespace Windows::Networking;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Networking::Connectivity;
using namespace Windows::Security::Cryptography;
using namespace Windows::Storage::Streams;
using namespace concurrency;

namespace App1 {
	enum gameStatus { Idle, GameStart, MyMove, NotMyMove };
	ref class Game sealed
	{
	public:
		static Game^ getInstance();
		String^ getStringIP();
		void sendInvitation(String^ serverHost);
		void registerFindPage(GameFindPage^ page);
	public:
		//property gameStatus mGameStatus;
	private:
		Game();
		void Init();
		void getLocalhost();
		void startServer();
		void serverOnConnectHandler(Sockets::StreamSocketListener^ socket, Sockets::StreamSocketListenerConnectionReceivedEventArgs^ args);
		void serverRequestHandler(DataReader^ reader, Sockets::StreamSocket^ socket);
		void clientResponseHandler(DataReader^ reader, DataWriter^ writer, Sockets::StreamSocket^ socket);
		JsonObject^ requestHandler(JsonObject^ jsonReauest);
	private:
		GameFindPage^ mGameFindPage;
		HostName^ mLocalhost;
		Board^ mBoard;
		Sockets::StreamSocketListener^ mServerSocket;
		Sockets::StreamSocket^ mClientSocket;
		static Game^ mInstance;
	};
}

