#pragma once
#include <string>
#include <vector>

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Networking;
using namespace Windows::Networking::Connectivity;

namespace App1 {
	ref class Game sealed
	{
	public:
		Game();
		static Game^ getInstance();
		String^ getLocalhost();
		void sendRequest();
		void helloListenerThread();
	private:
		void Init();
		std::string getMyIP();
		void networkScanThread();
		void respond(Sockets::StreamSocketListener, Sockets::StreamSocketListenerConnectionReceivedEventArgs args);
	private:
		std::string _localhost;
		std::vector<std::string> _availablePlayers;
		static Game^ mInstance;
	};
}

