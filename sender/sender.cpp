#include <array>
#include <iostream>

#include "../config.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/StreamCopier.h"

int main(int argc, char** argv) {
  Poco::UInt16 receiver_port = 20000;

  if (argc > 1) {  // port of receiver
    try {
      receiver_port = std::stoi(argv[1]);
    } catch (std::exception const& e) {
      std::cout << "Invalid port number " << e.what() << "\n";
    }
  }

  char* receiver_name = const_cast<char*>("localhost");
  if (argc > 2) {  // receiver names
    receiver_name = argv[2];
  }

  // how many kb shall be sent
  constexpr size_t DEFAULT_SEND_KB = 10 * cFaktorKiloByte;
  size_t send_kb = DEFAULT_SEND_KB;
  if (argc > 3) {
    send_kb = std::stoi(argv[3]);
  }

  Poco::Net::SocketAddress sa(receiver_name, receiver_port);
  Poco::Net::StreamSocket ss(sa);

  std::array<uint8_t, cArraySize> data;
  std::array<char, 1024> answer;

  std::srand(std::time(nullptr));

  // so often the buffer must be sent
  size_t LOOP_MAX = send_kb * 1024 / cArraySize;

  for (size_t kb = 0; kb < LOOP_MAX; kb++) {
    for (size_t b = 0; b < data.size(); b++) {
      // generate random data to send
      data[b] = std::rand();
    }

    ss.sendBytes(data.data(), data.size());

    // always wait for the response
    int n = ss.receiveBytes(answer.data(), answer.size());

    // print progress and answer
    std::cout << "Sended " << ((kb + 1) * cArraySize / 1024) << " of " << send_kb << " kb, answer = ";
    for (int i = 0; i < n; i++) std::cout << answer[i];
    std::cout << std::endl;
  }
}