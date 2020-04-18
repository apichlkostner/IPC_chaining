#include <array>
#include <chrono>
#include <iostream>
#include <thread>

#include "../config.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/TCPServer.h"

// pointer to next receiver stream
Poco::Net::StreamSocket* pSocketStream(nullptr);

class ReceiveConnection : public Poco::Net::TCPServerConnection {
 public:
  ReceiveConnection(const Poco::Net::StreamSocket& s) : Poco::Net::TCPServerConnection(s) {}

  void run() {
    Poco::Net::StreamSocket& ss = socket();
    try {
      std::array<char, cArraySize> buffer;
      std::array<char, 1024> answer;

      int n = ss.receiveBytes(buffer.data(), buffer.size());

      while (n > 0) {
        if (pSocketStream != nullptr) {
          // forward to next receiver
          pSocketStream->sendBytes(buffer.data(), n);
          // receive answer == OK before going on
          pSocketStream->receiveBytes(answer.data(), answer.size());
        } else {
          // last receiver in chain, calculate sum and print
          uint64_t sum = 0;
          for (size_t i = 0; i < n; i++) {
            sum += buffer[i];
          }
          std::cout << "->" << sum << std::endl;
        }

        // always send ok back
        ss.sendBytes("OK", 2);

        // and go on with receiving data
        n = ss.receiveBytes(buffer.data(), buffer.size());
      }
    } catch (Poco::Exception& e) {
      std::cerr << "ReceiveConnection: " << e.displayText() << std::endl;
    }
  }
};

int main(int argc, char** argv) {
  Poco::UInt16 server_port = 20000;

  if (argc > 1) {  // own port number
    try {
      server_port = std::stoi(argv[1]);
    } catch (Poco::Exception& e) {
      std::cout << "Invalid port number server_port " << e.displayText() << "\n";
    }
  }
  if (argc > 2) {  // port number for next receiver in the chain
    try {
      Poco::UInt16 send_port = std::stoi(argv[2]);

      if (send_port > 0) {
        std::cout << "Forwarding to port " << send_port << std::endl;
        Poco::Net::SocketAddress sa("localhost", send_port);
        pSocketStream = new Poco::Net::StreamSocket(sa);
      }
    } catch (std::exception const& e) {
      std::cout << "Invalid port number send_port " << e.what() << "\n";
    }
  }

  Poco::Net::TCPServer srv(new Poco::Net::TCPServerConnectionFactoryImpl<ReceiveConnection>(), server_port);

  std::cout << "Starting new server" << std::endl;

  srv.start();

  while (1) std::this_thread::sleep_for(std::chrono::seconds(1));

  srv.stop();
}