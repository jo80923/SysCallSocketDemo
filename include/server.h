#ifndef SERVER_H
#define SERVER_H

#include "common_includes.h"
#include "io_util.h"

class server{

protected:
  int connectionsAllowed;//will be used when forking is implemented
  int port;
  char* host;
  int sockfd;

public:

  ~server();
  void setMaxConnections(int connectionsAllowed);//not applicable in all types of sockets
  virtual void runSysCallServer(){}

};

class server_tcp : public server{

  void processCommands(int currentConnection);

public:
  server_tcp();
  server_tcp(char* host, int port);
  void runSysCallServer();
};

class server_udp : public server{

  void processCommands();

public:
  server_udp();
  server_udp(char* host, int port);
  void runSysCallServer();
};

#endif /* SERVER_H */
