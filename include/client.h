#ifndef CLIENT_H
#define CLIENT_H

#include "common_includes.h"
#include "io_util.h"


class client{

protected:
  int sockfd;//called this due to a necessary method being called socket()
  int port;
  char* host;

public:
  bool active;
  ~client();
  virtual void executeSysCall(std::string command){}

};

class client_tcp : public client{

public:
  client_tcp();
  client_tcp(char* host, int port);
  void executeSysCall(std::string command);
};

class client_udp : public client{
  addrinfo* serv_info;

public:
  client_udp();
  client_udp(char* host, int port);
  void executeSysCall(std::string command);
};

#endif /* CLIENT_H */
