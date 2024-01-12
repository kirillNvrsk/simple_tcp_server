#include "Poco/Net/Socket.h"
#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"

#include <algorithm>
#include <cstring>
#include <iostream>

namespace Net = Poco::Net;

const size_t BUFFER_LENGTH = 256;

class SimpleTCPReverseString : public Net::TCPServerConnection {

public:
  SimpleTCPReverseString(const Net::StreamSocket &s)
      : Net::TCPServerConnection(s) {}

  void run() override {
    Net::StreamSocket &ss = socket();
    
    std::cout << "New connection\n";
    ss.sendBytes("Welcome to POCO TCP server. Enter you string:\n", 47);

    try {
      char buffer[BUFFER_LENGTH] {};
      char reversed[BUFFER_LENGTH] {};
      int n = ss.receiveBytes(buffer, BUFFER_LENGTH);
      
      std::cout << "Received: " << buffer;
      
      while (n > 0) {
        auto eol = std::find(std::begin(buffer), std::end(buffer), '\r');
        size_t cr = eol - buffer;
        std::reverse_copy(std::begin(buffer), eol, std::begin(reversed));
        std::strcpy(reversed+cr, "\r\n\0");
        
        ss.sendBytes(reversed, cr+2);
        n = ss.receiveBytes(buffer, BUFFER_LENGTH);
        std::cout << "Received: " << buffer;
      }
    } catch (Poco::Exception &exc) {
      std::cerr << "EchoConnection: " << exc.displayText() << std::endl;
    }
  }
};

int main() {
  Net::ServerSocket socket(28828);

  Net::TCPServerParams *pParams = new Net::TCPServerParams();
  pParams->setMaxThreads(4);
  pParams->setMaxQueued(4);
  pParams->setThreadIdleTime(100);

  Poco::Net::TCPServer myserver(
      new Poco::Net::TCPServerConnectionFactoryImpl<SimpleTCPReverseString>(), 
      socket,
      pParams);
  myserver.start();

  while (true)
    ;

  return 0;
}