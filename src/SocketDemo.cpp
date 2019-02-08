#include "common_includes.h"
#include "server.h"
#include "client.h"

int main(int argc, char *argv[]){

  char* host = NULL;
  int port = -1;
  SocketType type = tcp;//default
  bool isServer = true;//defualt
  parseNetworkArgs(argc, argv, host, port, isServer, type);


  server* server = NULL;
  client* client = NULL;

  if(isServer){
    if(type == tcp){
      server = new server_tcp(host, port);
    }
    else if(type == udp){
      server = new server_udp(host, port);
    }
    server->setMaxConnections(1);
    server->runSysCallServer();
  }
  else{
    if(type == tcp){
      client = new client_tcp(host, port);
    }
    else if(type == udp){
      client = new client_udp(host, port);
    }
    while(client->active){
      client->executeSysCall(keyboardInput());
    }
  }
}
